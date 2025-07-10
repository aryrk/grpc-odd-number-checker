#include "websocket_globals.h"

std::vector<uWS::WebSocket<false, true, PerSocketData> *> clients;
std::mutex clients_mutex;
std::atomic<bool> running{true};
rapidjson::Document web_socket_json_data;
