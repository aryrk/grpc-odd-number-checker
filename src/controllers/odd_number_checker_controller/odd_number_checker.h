#pragma once
#include <grpc++/grpc++.h>
#include "number_checker.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using number_checker::NumberCheckerService;
using number_checker::OddCheckerReply;
using number_checker::OddCheckerRequest;

class CheckServiceImpl final : public NumberCheckerService::Service
{
public:
    Status IsOddNumber(ServerContext *context, const OddCheckerRequest *request, OddCheckerReply *response) override;
    Status ClearCache(ServerContext *context, const google::protobuf::Empty *request, google::protobuf::Empty *response) override;
};