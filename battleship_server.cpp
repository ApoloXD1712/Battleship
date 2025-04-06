#include "battleship_server.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <mutex>

BattleshipServer::BattleshipServer(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), next_player_id_(0) {
    start_accept();
}

void BattleshipServer::start_accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor().context());
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            int player_id = next_player_id_++;
            std::string player_name = "Player" + std::to_string(player_id);
            auto player = std::make_shared<Player>(player_name, std::move(*socket));

            {
                std::lock_guard<std::mutex> lock(mtx_);
                players_[player_id] = player;
            }

            player->send_message("WELCOME " + player_name + "\n");
            start_reading(player_id);
        }
        start_accept();
    });
}

void BattleshipServer::start_reading(int player_id) {
    auto& player = *players_[player_id];
    auto buffer = std::make_shared<std::vector<char>>(buffer_size_);

    player.get_socket().async_read_some(boost::asio::buffer(*buffer),
        [this, player_id, buffer](boost::system::error_code ec, size_t bytes_transferred) {
            if (!ec) {
                auto& player = *players_[player_id];
                player.append_to_buffer(std::string(buffer->data(), bytes_transferred));

                while (player.has_complete_message()) {
                    std::string message = player.extract_message();
                    process_message(player_id, message);
                }
                start_reading(player_id);
            } else {
                disconnect_player(player_id);
            }
        });
}

void BattleshipServer::process_message(int player_id, const std::string& message) {
    auto& player = players_[player_id];

    if (message == "READY") {
        player->mark_ready();

        for (const auto& [other_id, p] : players_) {
            if (other_id != player_id && p->is_ready() && !active_games_.count(other_id)) {
                auto session = std::make_shared<Game>(player.get(), p.get());

                active_games_[player_id] = session;
                active_games_[other_id] = session;

                player->send_message("MATCH_FOUND\n");
                p->send_message("MATCH_FOUND\n");
                break;
            }
        }
    }
    else if (message.rfind("FIRE:", 0) == 0) {
        size_t pos = message.find(':');
        std::string coords = message.substr(pos + 1);
        int x = std::stoi(coords.substr(0, coords.find(',')));
        int y = std::stoi(coords.substr(coords.find(',') + 1));

        if (active_games_.count(player_id)) {
            active_games_[player_id]->process_move(player.get(), x, y);
        } else {
            player->send_message("ERROR: No estas en una partida activa.\n");
        }
    }
    else {
        player->send_message("ERROR: Comando no reconocido.\n");
    }
}

void BattleshipServer::disconnect_player(int player_id) {
    std::lock_guard<std::mutex> lock(mtx_);

    if (players_.count(player_id)) {
        auto player = players_[player_id];

        boost::system::error_code ec;
        player->get_socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        player->get_socket().close();

        for (auto it = active_games_.begin(); it != active_games_.end(); ) {
            auto& session = it->second;
            if (session->get_player1() == player.get() || session->get_player2() == player.get()) {
                Player* other = (session->get_player1() == player.get()) ? session->get_player2() : session->get_player1();
                if (other) other->send_message("OPPONENT_DISCONNECTED\n");
                it = active_games_.erase(it);
            } else {
                ++it;
            }
        }

        players_.erase(player_id);
    }
} 