#pragma once

#include "player.h"

class Game {
public:
    Game(Player* p1, Player* p2);

    Player* get_player1() const;
    Player* get_player2() const;

    bool is_player_turn(Player* player) const;
    void process_move(Player* player, int x, int y);
    bool has_ended() const;

private:
    Player* player1_;
    Player* player2_;
    Player* current_turn_;
    bool game_over_;

    bool apply_shot(Player* attacker, Player* defender, int x, int y);
    bool check_victory(Player* defender);
    void end_game(Player* winner, Player* loser);
};