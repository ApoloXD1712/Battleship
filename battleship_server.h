#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "player.h"
#include "game.h"

class BattleshipServer {
public:
    BattleshipServer(boost::asio::io_context& io_context, short port);
    void start_accept();
    void start_reading(int player_id);
    void process_message(int player_id, const std::string& message);
    void disconnect_player(int player_id);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map<int, std::shared_ptr<Player>> players_;
    std::unordered_map<int, std::shared_ptr<Game>> active_games_;
    int next_player_id_;
    const size_t buffer_size_ = 1024;
    std::mutex mtx_;
};