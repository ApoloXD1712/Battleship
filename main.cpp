#include <iostream>
#include <boost/asio.hpp>
#include "battleship_server.h"

int main() {
    try {
        const short port = 8080;
        boost::asio::io_context io_context;

        std::cout << "===============================" << std::endl;
        std::cout << "   🛡️  Battleship Server 🛡️     " << std::endl;
        std::cout << "   🚀 Listening on port " << port << "..." << std::endl;
        std::cout << "===============================" << std::endl;

        BattleshipServer server(io_context, port);
        io_context.run();

    } catch (const std::exception& e) {
        std::cerr << "❌ Error al iniciar el servidor: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}