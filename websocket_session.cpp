#include "websocket_session.h"
#include "player.h"
#include <iostream>

WebSocketSession::WebSocketSession(tcp::socket socket)
    : ws_(std::move(socket)) {}

void WebSocketSession::start() {
    ws_.async_accept(
        std::bind(&WebSocketSession::on_accept, shared_from_this(), std::placeholders::_1));
}

void WebSocketSession::on_accept(boost::system::error_code ec) {
    if (ec) {
        std::cerr << "Accept error: " << ec.message() << std::endl;
        return;
    }
    do_read();
}

void WebSocketSession::do_read() {
    ws_.async_read(buffer_,
        std::bind(&WebSocketSession::on_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void WebSocketSession::on_read(boost::system::error_code ec, std::size_t) {
    if (ec) {
        std::cerr << "Read error: " << ec.message() << std::endl;
        return;
    }

    std::string message = boost::beast::buffers_to_string(buffer_.data());
    buffer_.consume(buffer_.size());

    handle_message(message);
    do_read();
}

void WebSocketSession::handle_message(const std::string& msg) {
    std::cout << "[WS] Mensaje recibido: " << msg << std::endl;

    if (msg.rfind("NICKNAME:", 0) == 0) {
        std::string nickname = msg.substr(9);
        auto new_player = std::make_shared<Player>(nickname);
        new_player->set_session(shared_from_this());
        set_player(new_player);
        send("NICK_OK\n");
    } else if (msg == "READY") {
        if (player_) {
            player_->mark_ready();
            send("WAITING_FOR_OPPONENT\n");
            if (match_callback_) match_callback_(player_);
        } else {
            send("ERROR: Define un NICKNAME primero\n");
        }
    } else if (msg.rfind("FIRE:", 0) == 0) {
        if (player_ && player_->get_game()) {
            std::string coord = msg.substr(5);  // quita "FIRE:"
            if (coord.length() >= 2) {
                char col_char = tolower(coord[0]);
                int row = coord[1] - '0';
                int col = col_char - 'a';
    
                if (row >= 0 && col >= 0 && row < 10 && col < 10) {  // puedes ajustar el tamaño del tablero aquí
                    player_->get_game()->process_move(player_, row, col);
                } else {
                    send("ERROR: Coordenada fuera de rango\\n");
                }
            } else {
                send("ERROR: Formato inválido de coordenada\\n");
            }
        } else {
            send("ERROR: No estás en una partida activa\\n");
        }
    }
     else {
        send("ERROR: Comando no reconocido\n");
    }
}

void WebSocketSession::send(const std::string& message) {
    std::lock_guard<std::mutex> lock(write_mutex_);
    write_queue_.push(message);
    if (write_queue_.size() == 1) {
        do_write();
    }
}

void WebSocketSession::do_write() {
    if (write_queue_.empty()) return;

    ws_.async_write(
        boost::asio::buffer(write_queue_.front()),
        std::bind(&WebSocketSession::on_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void WebSocketSession::on_write(boost::system::error_code ec, std::size_t) {
    if (ec) {
        std::cerr << "Write error: " << ec.message() << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(write_mutex_);
    write_queue_.pop();
    if (!write_queue_.empty()) {
        do_write();
    }
}

void WebSocketSession::set_match_callback(std::function<void(std::shared_ptr<Player>)> callback) {
    match_callback_ = std::move(callback);
}

std::shared_ptr<Player> WebSocketSession::get_player() const {
    return player_;
}

void WebSocketSession::set_player(std::shared_ptr<Player> player) {
    player_ = player;
}