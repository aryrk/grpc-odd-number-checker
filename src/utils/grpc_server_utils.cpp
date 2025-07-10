#include "grpc_server_utils.h"
#include "../controllers/odd_number_checker_controller/odd_number_checker.h"

void RunGrpcServer(std::string server_address)
{
    CheckServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}