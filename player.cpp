#include "player.h"

Player::Player(std::string name, boost::asio::ip::tcp::socket sock)
    : nickname(name), socket(std::move(sock)), ready(false) {}