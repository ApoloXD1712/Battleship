#pragma once
#include "player.h"

class Game {
public:
    Player* player1;
    Player* player2;
    int turn; // 0 -> player1, 1 -> player2

    Game(Player* p1, Player* p2);
    void switch_turn();
}; 