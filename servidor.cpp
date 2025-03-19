#include <iostream>
#include <thread>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <mutex>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;

const int BOARD_SIZE = 10;
const int TURN_TIME_LIMIT = 30; // Segundos
const string LOG_FILE = "server_log.txt";

// Clase que representa a un jugador
class Player {
public:
    string nickname;
    ip::tcp::socket socket;
    bool ready;
    char board[BOARD_SIZE][BOARD_SIZE];
    
    Player(string name, ip::tcp::socket sock)
        : nickname(name), socket(std::move(sock)), ready(false) {}
};

// Clase que representa una partida entre dos jugadores
class Game {
public:
    Player* player1;
    Player* player2;
    int turn; // 0 -> player1, 1 -> player2

    Game(Player* p1, Player* p2) : player1(p1), player2(p2), turn(0) {}
    
    void switch_turn() {
        turn = 1 - turn;
    }
};

class BattleshipServer {
public:
    explicit BattleshipServer(io_context& ioc, int port)
        : acceptor_(ioc, ip::tcp::endpoint(ip::tcp::v4(), port)), ioc_(ioc) {
        start_accept();
    }

private:
    io_context& ioc_;
    ip::tcp::acceptor acceptor_;
    vector<thread> threads_;
    unordered_map<int, Player*> players_;
    vector<Game*> games_;
    mutex mtx_;
    int player_count = 0;

    void log_event(const string& event) {
        lock_guard<mutex> lock(mtx_);
        ofstream log(LOG_FILE, ios::app);
        log << chrono::system_clock::to_time_t(chrono::system_clock::now()) << " - " << event << endl;
    }

    void start_accept() {
        auto socket = make_shared<ip::tcp::socket>(acceptor_.get_executor().context());
        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                handle_client(socket);
            }
            start_accept();
        });
    }

    void handle_client(shared_ptr<ip::tcp::socket> socket) {
        lock_guard<mutex> lock(mtx_);
        int player_id = player_count++;
        players_[player_id] = new Player("", move(*socket));
        log_event("Player connected: " + to_string(player_id));
        start_reading(player_id);
    }

    void start_reading(int player_id) {
        auto& player = *players_[player_id];
        auto buffer = make_shared<boost::beast::flat_buffer>();
        async_read(player.socket, *buffer, [this, player_id, buffer](boost::system::error_code ec, size_t) {
            if (!ec) {
                string data(boost::asio::buffer_cast<const char*>(buffer->data()), buffer->size());
                process_message(player_id, data);
            }
        });
    }

    void process_message(int player_id, const string& msg) {
        log_event("Received from Player " + to_string(player_id) + ": " + msg);
        if (msg == "READY") {
            players_[player_id]->ready = true;
            log_event("Player " + to_string(player_id) + " is ready.");
            match_players();
        }
    }
    
    void match_players() {
        vector<int> ready_players;
        for (auto& [id, player] : players_) {
            if (player->ready) {
                ready_players.push_back(id);
            }
        }
        if (ready_players.size() >= 2) {
            Player* p1 = players_[ready_players[0]];
            Player* p2 = players_[ready_players[1]];
            games_.push_back(new Game(p1, p2));
            log_event("Game started between Player " + to_string(ready_players[0]) + " and Player " + to_string(ready_players[1]));
        }
    }
};

int main() {
    try {
        io_context ioc;
        BattleshipServer server(ioc, 8080);
        ioc.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
