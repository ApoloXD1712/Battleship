#include "game.h"
#include <sstream>

Game::Game(Player* player1, Player* player2)
    : player1_(player1), player2_(player2), current_turn_(player1) {}

void Game::process_move(Player* player, int x, int y) {
    if (player != current_turn_) {
        player->send_message("NOT_YOUR_TURN\n");
        return;
    }

    Player* opponent = (player == player1_) ? player2_ : player1_;
    bool hit = false;

    for (const auto& ship : opponent->get_ships()) {
        int dx = ship.horizontal ? 1 : 0;
        int dy = ship.horizontal ? 0 : 1;

        for (int i = 0; i < ship.length; ++i) {
            if (ship.x + i * dx == x && ship.y + i * dy == y) {
                hit = true;
                break;
            }
        }

        if (hit) break;
    }

    std::ostringstream response;
    response << (hit ? "HIT" : "MISS") << ":" << x << "," << y << "\n";
    player->send_message(response.str());
    opponent->send_message(response.str());

    // Alternar turno
    current_turn_ = opponent;
}