#include "player.h"
#include <iostream>
#include <random>
#include <algorithm>

Player::Player(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket), ready_(false) {}

void Player::send_message(const std::string& message) {
    boost::asio::write(*socket_, boost::asio::buffer(message));
}

bool Player::is_ready() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return ready_;
}

void Player::mark_ready() {
    std::lock_guard<std::mutex> lock(mtx_);
    ready_ = true;
}

bool Player::has_all_ships_placed() const {
    return ships_.size() >= kTotalShips;
}

const std::vector<Ship>& Player::get_ships() const {
    return ships_;
}

void Player::place_random_ships() {
    ships_.clear();
    static const std::vector<int> ship_lengths = {5, 4, 3, 3, 2};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_dir(0, 1);
    std::uniform_int_distribution<> dist_pos(0, kBoardSize - 1);

    for (int length : ship_lengths) {
        bool placed = false;
        while (!placed) {
            bool horizontal = dist_dir(gen);
            int x = dist_pos(gen);
            int y = dist_pos(gen);

            int dx = horizontal ? 1 : 0;
            int dy = horizontal ? 0 : 1;

            if ((horizontal && x + length > kBoardSize) || (!horizontal && y + length > kBoardSize))
                continue;

            bool overlap = false;
            for (const Ship& ship : ships_) {
                for (int i = 0; i < length; ++i) {
                    int nx = x + i * dx;
                    int ny = y + i * dy;

                    for (int j = 0; j < ship.length; ++j) {
                        int sx = ship.x + j * (ship.horizontal ? 1 : 0);
                        int sy = ship.y + j * (ship.horizontal ? 0 : 1);
                        if (nx == sx && ny == sy) {
                            overlap = true;
                            break;
                        }
                    }
                    if (overlap) break;
                }
                if (overlap) break;
            }

            if (!overlap) {
                ships_.push_back({x, y, horizontal, length});
                placed = true;
            }
        }
    }
}

void Player::register_hit(int x, int y) {
    hits_.emplace_back(x, y);
}

bool Player::is_defeated() const {
    for (const auto& ship : ships_) {
        int dx = ship.horizontal ? 1 : 0;
        int dy = ship.horizontal ? 0 : 1;
        for (int i = 0; i < ship.length; ++i) {
            int sx = ship.x + i * dx;
            int sy = ship.y + i * dy;
            if (std::find(hits_.begin(), hits_.end(), std::make_pair(sx, sy)) == hits_.end()) {
                return false;
            }
        }
    }
    return true;
}