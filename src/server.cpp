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

class CheckServiceImpl final : public CheckService::Service
{
public:
    Status IsOddNumber(ServerContext *context, const OddCheckeRequest *request, OddCheckerReply *response) override
    {
        int number = request->number();
        response->set_isodd(number % 2 != 0);
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
    RunServer();
    return 0;
}