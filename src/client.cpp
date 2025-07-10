#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include "number_checker.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using number_checker::NumberCheckerService;
using number_checker::OddCheckerReply;
using number_checker::OddCheckerRequest;

class CheckServiceClient
{
public:
    CheckServiceClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(NumberCheckerService::NewStub(channel)) {}
    bool IsOddNumber(int number)
    {
        OddCheckerRequest request;
        request.set_number(number);
        OddCheckerReply response;
        grpc::ClientContext context;
        grpc::Status status = stub_->IsOddNumber(&context, request, &response);
        if (status.ok())
        {
            return response.is_odd();
        }
        else
        {
            std::cerr << "RPC failed: " << status.error_message() << std::endl
                      << "Error code: " << status.error_code() << std::endl;
            return false;
        }
    }
    void ClearCache()
    {
        google::protobuf::Empty request;
        google::protobuf::Empty response;
        grpc::ClientContext context;
        grpc::Status status = stub_->ClearCache(&context, request, &response);
        if (!status.ok())
        {
            std::cerr << "RPC failed: " << status.error_message() << std::endl
                      << "Error code: " << status.error_code() << std::endl;
        }
    }

private:
    std::unique_ptr<NumberCheckerService::Stub> stub_;
};

void RunNumberChecker(CheckServiceClient &client)
{
    int number;
    std::cout << "Enter a number to check if it is odd: ";
    std::cin >> number;

    bool is_odd = client.IsOddNumber(number);
    std::cout << "The number " << number << (is_odd ? " is odd." : " is not odd.") << std::endl;
}

int main(int argc, char **argv)
{
    std::string server_address("localhost:50051");
    CheckServiceClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    while (true)
    {
        int choice;
        std::cout << "1. Check if a number is odd\n";
        std::cout << "2. Clear cache (delete websocket data)\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            RunNumberChecker(client);
            break;
        case 2:
            client.ClearCache();
            std::cout << "Cache cleared." << std::endl;
            break;
        case 3:
            std::cout << "Exiting..." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    }
    return 0;
}