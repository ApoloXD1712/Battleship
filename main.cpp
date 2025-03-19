#include "battleship_server.h"
#include "config.h"
#include <iostream>

int main() {
    try {
        boost::asio::io_context ioc;
        BattleshipServer server(ioc, BattleshipConfig::SERVER_PORT);
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
} 