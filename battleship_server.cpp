#include "battleship_server.h"
#include <fstream>
#include <chrono>
#include <iostream>

BattleshipServer::BattleshipServer(boost::asio::io_context& ioc, int port)
    : acceptor_(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , ioc_(ioc)
    , log_file_(BattleshipConfig::LOG_FILE)
    , buffer_size_(BattleshipConfig::BUFFER_SIZE)
    , turn_time_limit_(BattleshipConfig::TURN_TIME_LIMIT) {
    start_accept();
}

BattleshipServer::~BattleshipServer() {
    cleanup_resources();
}

void BattleshipServer::cleanup_resources() {
    std::lock_guard<std::mutex> lock(mtx_);
    
    // Limpiar juegos
    for (auto game : games_) {
        delete game;
    }
    games_.clear();
    
    // Limpiar jugadores
    for (auto& [id, player] : players_) {
        delete player;
    }
    players_.clear();
}

void BattleshipServer::log_event(const std::string& event) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ofstream log(log_file_, std::ios::app);
    log << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << " - " << event << std::endl;
}

void BattleshipServer::start_accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor().context());
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            handle_client(socket);
        }
        start_accept();
    });
}

void BattleshipServer::handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    std::lock_guard<std::mutex> lock(mtx_);
    int player_id = player_count++;
    players_[player_id] = new Player("", std::move(*socket));
    log_event("Player connected: " + std::to_string(player_id));
    start_reading(player_id);
}

void BattleshipServer::start_reading(int player_id) {
    auto& player = *players_[player_id];
    auto buffer = std::make_shared<std::vector<char>>(buffer_size_);
    player.socket.async_read_some(boost::asio::buffer(*buffer),
        [this, player_id, buffer](boost::system::error_code ec, size_t bytes_transferred) {
            if (!ec) {
                std::string data(buffer->begin(), buffer->begin() + bytes_transferred);
                process_message(player_id, data);
            } else if (ec != boost::asio::error::operation_aborted) {
                // Manejar desconexión
                log_event("Player " + std::to_string(player_id) + " disconnected");
                cleanup_resources();
            }
            start_reading(player_id);
        });
}

void BattleshipServer::process_message(int player_id, const std::string& msg) {
    log_event("Received from Player " + std::to_string(player_id) + ": " + msg);
    if (msg == "READY") {
        players_[player_id]->ready = true;
        log_event("Player " + std::to_string(player_id) + " is ready.");
        match_players();
    }
}

void BattleshipServer::match_players() {
    std::vector<int> ready_players;
    for (auto& [id, player] : players_) {
        if (player->ready) {
            ready_players.push_back(id);
        }
    }
    if (ready_players.size() >= 2) {
        Player* p1 = players_[ready_players[0]];
        Player* p2 = players_[ready_players[1]];
        games_.push_back(new Game(p1, p2));
        log_event("Game started between Player " + std::to_string(ready_players[0]) + " and Player " + std::to_string(ready_players[1]));
    }
} 