#include "game.h"

Game::Game(Player* p1, Player* p2)
    : player1_(p1), player2_(p2), current_turn_(p1), game_over_(false) {}

Player* Game::get_player1() const { return player1_; }
Player* Game::get_player2() const { return player2_; }

bool Game::is_player_turn(Player* player) const {
    return current_turn_ == player;
}

bool Game::has_ended() const {
    return game_over_;
}

void Game::process_move(Player* player, int x, int y) {
    if (game_over_) return;
    if (!is_player_turn(player)) {
        player->send_message("NOT_YOUR_TURN\n");
        return;
    }

    Player* opponent = (player == player1_) ? player2_ : player1_;
    bool hit = apply_shot(player, opponent, x, y);

    // Informar a ambos jugadores del resultado
    player->send_message(hit ? "HIT\n" : "MISS\n");
    opponent->send_message("OPPONENT_FIRED:" + std::to_string(x) + "," + std::to_string(y) + ":" + (hit ? "HIT\n" : "MISS\n"));

    if (check_victory(opponent)) {
        end_game(player, opponent);
    } else {
        // Cambiar turno
        current_turn_ = opponent;
    }
}

bool Game::apply_shot(Player* attacker, Player* defender, int x, int y) {
    char(&board)[10][10] = defender->get_board();
    if (board[x][y] == 'S') {
        board[x][y] = 'X'; // impactado
        return true;
    } else if (board[x][y] == '.') {
        board[x][y] = 'O'; // agua
        return false;
    }
    return false; // ya disparado
}

bool Game::check_victory(Player* defender) {
    char(&board)[10][10] = defender->get_board();
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            if (board[i][j] == 'S')
                return false; // todavía hay barcos
    return true;
}

void Game::end_game(Player* winner, Player* loser) {
    game_over_ = true;
    winner->send_message("VICTORY\n");
    loser->send_message("DEFEAT\n");
}