#pragma once

#include <string>
#include <boost/asio.hpp>

class Player {
public:
    Player(const std::string& name, boost::asio::ip::tcp::socket socket);

    const std::string& get_name() const;
    bool is_ready() const;
    void mark_ready();

    void append_to_buffer(const std::string& data);
    bool has_complete_message() const;
    std::string extract_message(); // extrae hasta '\n'

    void send_message(const std::string& message);
    boost::asio::ip::tcp::socket& get_socket();
    
    char (&get_board())[10][10];  // acceso al tablero

private:
    std::string nickname_;
    boost::asio::ip::tcp::socket socket_;
    bool ready_;
    char board_[10][10];
    std::string input_buffer_;
};