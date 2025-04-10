#include "game.h"
#include "ship.h"
#include <algorithm>
#include <iostream>

Game::Game(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2)
    : player1_(p1), player2_(p2), current_turn_(p1) {
    p1->send_message("MATCH_FOUND\n");
    p2->send_message("MATCH_FOUND\n");
    p1->send_message("YOUR_TURN\n");
    p2->send_message("WAIT_TURN\n");

    p1->place_random_ships();
    p2->place_random_ships();
}

void Game::process_move(std::shared_ptr<Player> player, int x, int y) {
    if (player != current_turn_) {
        player->send_message("NOT_YOUR_TURN\n");
        return;
    }

    std::shared_ptr<Player> opponent = (player == player1_) ? player2_ : player1_;

    // 🔒 Validar si la casilla ya fue atacada
    if (std::find(opponent->get_hits().begin(), opponent->get_hits().end(), std::make_pair(x, y)) != opponent->get_hits().end()) {
        player->send_message("ALREADY_HIT\n");
        return;
    }

    bool hit = false;

    for (const Ship& ship : opponent->get_ships()) {
        int dx = ship.horizontal ? 1 : 0;
        int dy = ship.horizontal ? 0 : 1;

        for (int i = 0; i < ship.length; ++i) {
            int sx = ship.x + i * dx;
            int sy = ship.y + i * dy;

            if (sx == x && sy == y) {
                opponent->register_hit(x, y);
                hit = true;

                 // Enviar mensaje HIT_ME al oponente
                {
                    std::string col = std::string(1, 'a' + y);
                    std::string hit_me = "HIT_ME|" + col + std::to_string(x) + "\n";
                    opponent->send_message(hit_me);
                }

                if (is_sunk(opponent, ship)) {
                    player->send_message("SUNK\n");

                    // 💥 DEBUG: Barco hundido
                    std::cout << "[DEBUG] " << player->nickname()
                              << " hundió un barco de " << ship.length
                              << " posiciones en (" << x << ", " << y << ") contra "
                              << opponent->nickname() << "\n";
                } else {
                    player->send_message("HIT\n");

                    // 🎯 DEBUG: Impacto normal
                    std::cout << "[DEBUG] " << player->nickname()
                              << " le dio a un barco en (" << x << ", " << y << ") contra "
                              << opponent->nickname() << "\n";
                }

                if (opponent->is_defeated()) {
                    player->send_message("YOU_WIN\n");
                    opponent->send_message("YOU_LOSE\n");
                    std::cout << "[DEBUG] " << player->nickname() << " ganó la partida.\n";
                }

                break;
            }
        }
        if (hit) break;
    }

    if (!hit) {
        opponent->register_hit(x, y);
        player->send_message("MISS\n");

        // ❌ DEBUG: Fallo
        std::cout << "[DEBUG] " << player->nickname()
                  << " falló el disparo en (" << x << ", " << y << ") contra "
                  << opponent->nickname() << "\n";
    }

    // Cambiar turno
    current_turn_ = opponent;
    current_turn_->send_message("YOUR_TURN\n");
    player->send_message("WAIT_TURN\n");
}


std::shared_ptr<Player> Game::get_player1() const {
    return player1_;
}

std::shared_ptr<Player> Game::get_player2() const {
    return player2_;
}

bool Game::is_hit(std::shared_ptr<Player> opponent, int x, int y) const {
    const auto& hits = opponent->get_hits();
    return std::find(hits.begin(), hits.end(), std::make_pair(x, y)) != hits.end();
}

bool Game::is_sunk(std::shared_ptr<Player> opponent, const Ship& ship) const {
    int dx = ship.horizontal ? 1 : 0;
    int dy = ship.horizontal ? 0 : 1;

    for (int i = 0; i < ship.length; ++i) {
        int sx = ship.x + i * dx;
        int sy = ship.y + i * dy;

        if (!is_hit(opponent, sx, sy)) {
            return false;
        }
    }
    return true;
}