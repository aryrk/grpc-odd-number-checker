#include <uWebSockets/App.h>
#include <atomic>
#include <vector>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <iostream>
#include <string>
#include <stdbool.h>

#include <grpc++/grpc++.h>
#include "numberchecker.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using numberchecker::CheckService;
using numberchecker::OddCheckeRequest;
using numberchecker::OddCheckerReply;

struct PerSocketData
{
};

std::vector<uWS::WebSocket<false, true, PerSocketData> *> clients;
std::mutex clients_mutex;
std::atomic<bool> running{true};

rapidjson::Document WebSocketJSONData;

void writeRapidJSON(int number, bool is_odd)
{
    WebSocketJSONData.SetObject();
    auto &alloc = WebSocketJSONData.GetAllocator();
    WebSocketJSONData.AddMember("number", number, alloc);
    WebSocketJSONData.AddMember("is_odd", is_odd, alloc);
}
class CheckServiceImpl final : public CheckService::Service
{
public:
    Status IsOddNumber(ServerContext *context, const OddCheckeRequest *request, OddCheckerReply *response) override
    {
        int number = request->number();
        bool is_odd = (number % 2 != 0);
        response->set_isodd(is_odd);

        writeRapidJSON(number, is_odd);

        return Status::OK;
    }
    Status clearCache(ServerContext *context, const google::protobuf::Empty *request, google::protobuf::Empty *response) override
    {
        WebSocketJSONData.SetNull();
        return Status::OK;
    }
};

std::string readRapidJSON()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    WebSocketJSONData.Accept(writer);
    return buffer.GetString();
}

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    CheckServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

void broadcastMessage()
{
    while (running)
    {

        if (WebSocketJSONData.IsNull())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (auto *ws : clients)
            {
                ws->send(readRapidJSON(), uWS::OpCode::TEXT);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char **argv)
{
    std::thread grpc_thread(RunServer);
    std::thread websocket_thread(broadcastMessage);

    uWS::App()
        .ws<PerSocketData>("/*", {.open = [](auto *ws)
                                  {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(ws);
                std::cout << "Client connected\n"; },
                                  .close = [](auto *ws, int, std::string_view)
                                  {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.erase(std::remove(clients.begin(), clients.end(), ws), clients.end());
                std::cout << "Client disconnected\n"; }})
        .listen(9001, [](auto *token)
                {
            if (token) std::cout << "Server listening on port 9001 (ws://localhost:9001)\n";
            else std::cout << "Failed to listen on port 9001\n"; })
        .run();

    running = false;

    grpc_thread.join();
    websocket_thread.join();
    return 0;
}