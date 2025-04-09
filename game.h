#pragma once

#include <memory>
#include "ship.h"
#include "player.h"

class Game {
public:
    Game(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2);

    void process_move(std::shared_ptr<Player> player, int x, int y);
    std::shared_ptr<Player> get_player1() const;
    std::shared_ptr<Player> get_player2() const;

private:
    std::shared_ptr<Player> player1_;
    std::shared_ptr<Player> player2_;
    std::shared_ptr<Player> current_turn_;

    bool is_hit(std::shared_ptr<Player> opponent, int x, int y) const;
    bool is_sunk(std::shared_ptr<Player> opponent, const Ship& ship) const;
};