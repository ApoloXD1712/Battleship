#pragma once
#include "player.h"

class Game {
public:
    Game(Player* player1, Player* player2);

    void process_move(Player* player, int x, int y);

    Player* get_player1() const { return player1_; }
    Player* get_player2() const { return player2_; }

private:
    Player* player1_;
    Player* player2_;
    Player* current_turn_;
};