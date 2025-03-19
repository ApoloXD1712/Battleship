#include "game.h"

Game::Game(Player* p1, Player* p2) : player1(p1), player2(p2), turn(0) {}

void Game::switch_turn() {
    turn = 1 - turn;
} 