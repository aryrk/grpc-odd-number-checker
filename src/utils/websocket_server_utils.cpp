#include "websocket_server_utils.h"
#include "../globals/server/websocket_globals.h"
#include "../handlers/rapid_json/rapid_json_handlers.h"
#include <uWebSockets/App.h>

void BroadcastWebSocketMessage(int delay_ms)
{
    while (running)
    {
        if (web_socket_json_data.IsNull())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (auto *ws : clients)
            {
                ws->send(ReadRapidJSON(web_socket_json_data), uWS::OpCode::TEXT);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

void StartWebSocketServer(std::string_view address)
{
    // Parse address, expecting format "host:port"
    std::string host = "0.0.0.0";
    int port = 9001;
    std::string addr(address);
    auto pos = addr.find(':');
    if (pos != std::string::npos)
    {
        host = addr.substr(0, pos);
        port = std::stoi(addr.substr(pos + 1));
    }

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
        .listen(host, port, [host, port](auto *token)
                {
            if (token) {
                std::cout << "Listening on ws://" << host << ":" << port << "\n";
            } else {
                std::cerr << "Failed to listen on ws://" << host << ":" << port << "\n";
            } })
        .run();
}