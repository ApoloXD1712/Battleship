#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <functional>

class Player;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    explicit WebSocketSession(tcp::socket socket);

    void start();                                      // Inicia la sesión
    void send(const std::string& message);             // Envía un mensaje al cliente
    void set_match_callback(std::function<void(std::shared_ptr<Player>)> callback);

    std::shared_ptr<Player> get_player() const;        // Devuelve el jugador asociado
    void set_player(std::shared_ptr<Player> player);   // Asocia un jugador a esta sesión

private:
    websocket::stream<tcp::socket> ws_;                // WebSocket de Beast
    boost::beast::flat_buffer buffer_;                 // Buffer de lectura
    std::mutex write_mutex_;                           // Mutex para cola de escritura
    std::queue<std::string> write_queue_;              // Cola de mensajes pendientes

    std::shared_ptr<Player> player_;                   // Jugador asociado a esta sesión
    std::function<void(std::shared_ptr<Player>)> match_callback_;  // Callback al estar listo

    void on_accept(boost::system::error_code ec);      // Handshake
    void do_read();                                    // Leer mensaje
    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
    void do_write();                                   // Escribir mensaje
    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);

    void handle_message(const std::string& msg);       // Procesar comandos
};