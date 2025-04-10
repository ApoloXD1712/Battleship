#pragma once
#include <string>
#include <memory>
#include <vector>
#include "ship.h"

class Game;  // Declaración adelantada


class WebSocketSession;

class Player {
public:
    void set_game(std::shared_ptr<Game> game) { game_ = game; }
    std::shared_ptr<Game> get_game() const { return game_; }
    explicit Player(const std::string& nickname);

    void set_session(std::shared_ptr<WebSocketSession> session);
    std::shared_ptr<WebSocketSession> get_session() const;

    void mark_ready();
    bool is_ready() const;
    std::string nickname() const;

    void send_message(const std::string& message);

    void append_to_buffer(const std::string& data);
    bool has_complete_message() const;
    std::string extract_message();

    void place_random_ships();
    const std::vector<Ship>& get_ships() const;
    void register_hit(int x, int y);
    bool is_defeated() const;
    const std::vector<std::pair<int, int>>& get_hits() const;

private:
    std::string nickname_;
    std::shared_ptr<Game> game_;
    bool ready_ = false;
    std::shared_ptr<WebSocketSession> session_;
    std::string read_buffer_;
    std::vector<Ship> ships_;
    std::vector<std::pair<int, int>> hits_;
    
};