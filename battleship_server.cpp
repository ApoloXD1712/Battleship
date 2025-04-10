#include "battleship_server.h"
#include "websocket_session.h"
#include "player.h"
#include "game.h"

#include <iostream>

BattleshipServer::BattleshipServer(boost::asio::io_context& io_context, short port)
    : io_context_(io_context), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    start_accept();
}

void BattleshipServer::start_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                auto session = std::make_shared<WebSocketSession>(std::move(socket));

                // Callback que se llama cuando el jugador envía READY
                session->set_match_callback([this](std::shared_ptr<Player> player) {
                    register_player(player);
                });

                session->start();
            } else {
                std::cerr << "Error aceptando conexión: " << ec.message() << std::endl;
            }

            // Aceptar siguiente conexión
            start_accept();
        });
}

void BattleshipServer::register_player(std::shared_ptr<Player> player) {
    std::lock_guard<std::mutex> lock(mutex_);
    players_[player->nickname()] = player;
    match_players();
}


void BattleshipServer::match_players() {
    while (ready_players.size() >= 2) {
        auto p1 = ready_players.front();
        ready_players.pop_front();

        auto p2 = ready_players.front();
        ready_players.pop_front();

        // Crear el juego con los dos jugadores
        auto game = std::make_shared<Game>(p1, p2);

        // Asignar el juego a cada jugador
        p1->set_game(game);
        p2->set_game(game);

        std::cout << "Game started between " << p1->nickname() << " and " << p2->nickname() << std::endl;

        // Enviar mensajes de inicio de partida
        p1->send_message("MATCH_FOUND\\nYOUR_TURN\\n");
        p2->send_message("MATCH_FOUND\\nWAIT_TURN\\n");
    }
}
