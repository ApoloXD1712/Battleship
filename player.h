#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>

struct Ship {
    int x, y;
    bool horizontal;
    int length;
};

class Player {
public:
    Player(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    void send_message(const std::string& message);

    bool is_ready() const;
    bool is_defeated() const;
    void mark_ready();
    void register_hit(int x, int y);

    bool has_all_ships_placed() const;
    void place_random_ships();

    const std::vector<Ship>& get_ships() const;

    // Métodos para comunicación (socket y buffer)
    boost::asio::ip::tcp::socket& get_socket();
    void append_to_buffer(const std::string& data);
    bool has_complete_message() const;
    std::string extract_message();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    mutable std::mutex mtx_;
    bool ready_;
    std::vector<Ship> ships_;
    std::string read_buffer_;
    std::vector<std::pair<int, int>> hits_;

    static constexpr int kBoardSize = 10;
    static constexpr int kTotalShips = 5;
};

#endif