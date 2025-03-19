#pragma once
#include <string>
#include <boost/asio.hpp>

class Player {
public:
    std::string nickname;
    boost::asio::ip::tcp::socket socket;
    bool ready;
    char board[10][10];  // BOARD_SIZE = 10
    
    Player(std::string name, boost::asio::ip::tcp::socket sock);
}; 