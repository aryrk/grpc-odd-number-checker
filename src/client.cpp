#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include "numberchecker.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using numberchecker::CheckService;
using numberchecker::OddCheckeRequest;
using numberchecker::OddCheckerReply;

class CheckServiceClient
{
public:
    CheckServiceClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(CheckService::NewStub(channel)) {}
    bool IsOddNumber(int number)
    {
        OddCheckeRequest request;
        request.set_number(number);
        OddCheckerReply response;
        grpc::ClientContext context;
        grpc::Status status = stub_->IsOddNumber(&context, request, &response);
        if (status.ok())
        {
            return response.isodd();
        }
        else
        {
            std::cerr << "RPC failed: " << status.error_message() << std::endl
                      << "Error code: " << status.error_code() << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<CheckService::Stub> stub_;
};

int main(int argc, char **argv)
{
    std::string server_address("localhost:50051");
    CheckServiceClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    while (true)
    {
        int number;
        std::cout << "Enter a number to check if it is odd: ";
        std::cin >> number;

        bool is_odd = client.IsOddNumber(number);
        std::cout << "The number " << number << (is_odd ? " is odd." : " is not odd.") << std::endl;
    
        std::cout << "Press Enter to continue or Ctrl+C to exit..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
        std::cin.get();
    }
    return 0;
}