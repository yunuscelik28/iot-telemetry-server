#include "telemetry/server.hpp"
#include "telemetry/session.hpp"
#include <iostream>

Server::Server(asio::io_context &io_context, short port)
    : acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
  std::cout << "Server is up, waiting for client on port " << port << "...\n";
  do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept([this](asio::error_code er, asio::ip::tcp::socket socket_) {
    if (!er) {
      std::cout << "Client arrived, connection established!\n";
      std::make_shared<Session>(std::move(socket_), um, mtx)->start();
    }
    do_accept();
  });
}
