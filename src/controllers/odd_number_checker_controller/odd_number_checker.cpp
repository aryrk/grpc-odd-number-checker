#include "odd_number_checker.h"

#include "../../globals/server/websocket_globals.h"
#include "../../handlers/rapid_json/rapid_json_handlers.h"

Status CheckServiceImpl::IsOddNumber(ServerContext *context, const OddCheckerRequest *request, OddCheckerReply *response)
{
    int number = request->number();
    bool is_odd = (number % 2 != 0);
    response->set_is_odd(is_odd);

    WriteRapidJSON(web_socket_json_data, "number", number);
    WriteRapidJSON(web_socket_json_data, "isOdd", is_odd);

    return Status::OK;
}

Status CheckServiceImpl::ClearCache(ServerContext *context, const google::protobuf::Empty *request, google::protobuf::Empty *response)
{
    web_socket_json_data.SetNull();
    return Status::OK;
}