#include "player.h"
#include "websocket_session.h"
#include <random>
#include <algorithm>

Player::Player(const std::string& nickname)
    : nickname_(nickname), ready_(false) {}

void Player::set_session(std::shared_ptr<WebSocketSession> session) {
    session_ = session;
}

std::shared_ptr<WebSocketSession> Player::get_session() const {
    return session_;
}

void Player::mark_ready() {
    ready_ = true;
}

bool Player::is_ready() const {
    return ready_;
}

std::string Player::nickname() const {
    return nickname_;
}

void Player::send_message(const std::string& message) {
    if (session_) {
        session_->send(message);
    }
}

void Player::append_to_buffer(const std::string& data) {
    read_buffer_ += data;
}

bool Player::has_complete_message() const {
    return read_buffer_.find('\n') != std::string::npos;
}

std::string Player::extract_message() {
    size_t pos = read_buffer_.find('\n');
    std::string message = read_buffer_.substr(0, pos);
    read_buffer_.erase(0, pos + 1);
    return message;
}


void Player::place_random_ships() {
    ships_.clear();
    static const std::vector<int> ship_lengths = {5, 4, 3, 3, 2};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_dir(0, 1);
    std::uniform_int_distribution<> dist_pos(0, 9);

    for (int length : ship_lengths) {
        bool placed = false;
        while (!placed) {
            bool horizontal = dist_dir(gen);
            int x = dist_pos(gen);
            int y = dist_pos(gen);

            int dx = horizontal ? 1 : 0;
            int dy = horizontal ? 0 : 1;

            if ((horizontal && x + length > 10) || (!horizontal && y + length > 10))
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

const std::vector<Ship>& Player::get_ships() const {
    return ships_;
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

const std::vector<std::pair<int, int>>& Player::get_hits() const {
    return hits_;
}