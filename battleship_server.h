#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <boost/asio.hpp>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "player.h"
#include "game.h"

class BattleshipServer {
public:
    BattleshipServer(boost::asio::io_context& io_context, short port);

private:
    void start_accept(); // Aceptar conexiones entrantes
    void register_player(std::shared_ptr<Player> player); // Agregar jugador listo
    void match_players(); // Emparejar jugadores listos

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map<std::string, std::shared_ptr<Player>> players_;
    std::vector<std::shared_ptr<Game>> games_;
    std::mutex mutex_;
};

#endif // BATTLESHIP_SERVER_H