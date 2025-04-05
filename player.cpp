#include "player.h"
#include <boost/asio.hpp>
#include <boost/asio/write.hpp>

Player::Player(const std::string& name, boost::asio::ip::tcp::socket socket)
    : nickname_(name), socket_(std::move(socket)), ready_(false) {
    // Inicializa el tablero vacío
    for (auto& row : board_)
        std::fill(std::begin(row), std::end(row), '.');
}

const std::string& Player::get_name() const {
    return nickname_;
}

bool Player::is_ready() const {
    return ready_;
}

void Player::mark_ready() {
    ready_ = true;
}

void Player::append_to_buffer(const std::string& data) {
    input_buffer_ += data;
}

bool Player::has_complete_message() const {
    return input_buffer_.find('\n') != std::string::npos;
}

std::string Player::extract_message() {
    size_t pos = input_buffer_.find('\n');
    if (pos == std::string::npos) return "";

    std::string msg = input_buffer_.substr(0, pos);
    input_buffer_.erase(0, pos + 1);
    return msg;
}

void Player::send_message(const std::string& message) {
    boost::asio::write(socket_, boost::asio::buffer(message));
}

boost::asio::ip::tcp::socket& Player::get_socket() {
    return socket_;
}

char (&Player::get_board())[10][10] {
    return board_;
}
