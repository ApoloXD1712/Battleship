#pragma once
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "player.h"
#include "game.h"
#include "config.h"

class BattleshipServer {
public:
    explicit BattleshipServer(boost::asio::io_context& ioc, int port);
    ~BattleshipServer();

private:
    boost::asio::io_context& ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::thread> threads_;
    std::unordered_map<int, Player*> players_;
    std::vector<Game*> games_;
    std::mutex mtx_;
    int player_count = 0;

    // Constantes de configuración
    const std::string log_file_;
    const int buffer_size_;
    const int turn_time_limit_;

    void log_event(const std::string& event);
    void start_accept();
    void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void start_reading(int player_id);
    void process_message(int player_id, const std::string& msg);
    void match_players();
    void cleanup_resources();
}; 