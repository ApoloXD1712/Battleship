#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <unordered_map>
#include <thread>

using namespace boost::asio;
using namespace boost::beast;
using namespace std;

class BattleshipServer {
public:
    explicit BattleshipServer(io_context& ioc, int port) : acceptor_(ioc, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        start_accept();
    }

private:
    ip::tcp::acceptor acceptor_;
    unordered_map<int, ip::tcp::socket> players_;
    int player_count = 0;

    void start_accept() {
        auto socket = std::make_shared<ip::tcp::socket>(acceptor_.get_executor().context());
        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                handle_client(socket);
            }
            start_accept();
        });
    }

    void handle_client(std::shared_ptr<ip::tcp::socket> socket) {
        int player_id = player_count++;
        players_[player_id] = std::move(*socket);
        cout << "Player " << player_id << " connected." << endl;
        start_reading(player_id);
    }

    void start_reading(int player_id) {
        auto& socket = players_[player_id];
        auto buffer = std::make_shared<boost::beast::flat_buffer>();
        async_read(socket, *buffer, [this, player_id, buffer](boost::system::error_code ec, size_t) {
            if (!ec) {
                string data(boost::asio::buffer_cast<const char*>(buffer->data()), buffer->size());
                cout << "Received from Player " << player_id << ": " << data << endl;
                // Process the data and respond
            }
        });
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
