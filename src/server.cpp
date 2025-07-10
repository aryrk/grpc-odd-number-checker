#include "globals/server/websocket_globals.h"
#include "controllers/odd_number_checker_controller/odd_number_checker.h"
#include "utils/grpc_server_utils.h"
#include "utils/websocket_server_utils.h"
#include <atomic>
#include <thread>

int main(int argc, char **argv)
{
    std::atomic<bool> running_local{true};

    std::thread grpc_thread([&]()
                            { RunGrpcServer("0.0.0.0:50051"); });
    std::thread websocket_thread([&]()
                                 { BroadcastWebSocketMessage(1000); });
    std::thread ws_server_thread([&]()
                                 { StartWebSocketServer("9001"); });

    std::cin.get();
    running_local = false;

    grpc_thread.join();
    websocket_thread.join();
    ws_server_thread.join();
    return 0;
}