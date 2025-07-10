#ifndef WEBSOCKETGLOBALS_HPP
#define WEBSOCKETGLOBALS_HPP

#include <uWebSockets/App.h>
#include <vector>
#include <mutex>
#include <rapidjson/document.h>
#include <atomic>

#include <stdbool.h>

struct PerSocketData
{
};

extern std::vector<uWS::WebSocket<false, true, PerSocketData> *> clients;
extern std::mutex clients_mutex;
extern std::atomic<bool> running;
extern rapidjson::Document web_socket_json_data;

#endif