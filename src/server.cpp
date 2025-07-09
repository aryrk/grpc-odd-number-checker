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

class CheckServiceImpl final : public CheckService::Service
{
public:
    Status IsOddNumber(ServerContext *context, const OddCheckeRequest *request, OddCheckerReply *response) override
    {
        int number = request->number();
        bool is_odd = (number % 2 != 0);
        response->set_isodd(is_odd);

        rapidjson::Document d;
        d.SetObject();
        auto &alloc = d.GetAllocator();
        d.AddMember("number", number, alloc);
        d.AddMember("is_odd", is_odd, alloc);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        std::string json_msg = buffer.GetString();

        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto *ws : clients)
        {
            ws->send(json_msg, uWS::OpCode::TEXT);
        }
        return Status::OK;
    }
};

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

int main(int argc, char **argv)
{
    std::thread grpc_thread(RunServer);

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
    return 0;
}