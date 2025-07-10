#pragma once

#include <string>

void StartWebSocketServer(std::string_view address = "0.0.0.0:9001");

void BroadcastWebSocketMessage(int delay_ms = 1000);