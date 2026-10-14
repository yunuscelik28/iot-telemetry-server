#include "telemetry/server.hpp"
#include "telemetry/session.hpp"
#include <fstream>
#include <iostream>

Server::Server(asio::io_context &io_context, unsigned short port)
    : acceptor_(io_context,
                asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
  std::cout << "Server is up, waiting for client on port " << port << "...\n";
  do_accept();
}

Server::~Server() {
  std::cout << "Saving data from memory to disk...\n";

  std::ofstream report_file("Report.csv");
  if (report_file.is_open()) {
    report_file << "DeviceID,Temperature,Speed,Timestamp\n";

    std::lock_guard<std::mutex> lock(map_mutex);
    // C++17 structured binding, splits the pair into key and value
    for (const auto &[id, packet] : device_map) {
      report_file << id << "," << packet.temperature << "," << packet.speed
                  << "," << packet.timestamp << "\n";
    }

    report_file.close();
    std::cout << "Data successfully written to 'Report.csv'!\n";
  } else {
    std::cerr << "Error: File could not be created or opened!\n";
  }
}

void Server::do_accept() {
  acceptor_.async_accept(
      [this](asio::error_code error, asio::ip::tcp::socket new_socket) {
        if (!error) {
          std::cout << "Client arrived, connection established!\n";
          // make_shared because Session uses shared_from_this()
          std::make_shared<Session>(std::move(new_socket), device_map,
                                    map_mutex)
              ->start();
        }
        // wait for the next client
        do_accept();
      });
}
