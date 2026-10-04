#include "telemetry/server.hpp"
#include "telemetry/session.hpp"
#include <fstream>
#include <iostream>

Server::Server(asio::io_context &io_context, short port)
    : acceptor_(io_context,
                asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
  std::cout << "Server is up, waiting for client on port " << port << "...\n";
  do_accept();
}

Server::~Server() {
  std::cout << "Saving data from memory to disk...\n";

  std::ofstream reportFile("Report.csv");
  if (reportFile.is_open()) {
    reportFile << "DeviceID,Temperature,Speed,Timestamp\n";

    std::lock_guard<std::mutex> lock(mtx);
    for (const auto &pair : um) {
      const auto &deviceData = pair.second;
      reportFile << deviceData.device_id << "," << deviceData.temperature << ","
                 << deviceData.speed << "," << deviceData.timestamp << "\n";
    }

    reportFile.close();
    std::cout << "Data successfully written to 'Report.csv'!\n";
  } else {
    std::cerr << "Error: File could not be created or opened!\n";
  }
}

void Server::do_accept() {
  acceptor_.async_accept(
      [this](asio::error_code er, asio::ip::tcp::socket socket_) {
        if (!er) {
          std::cout << "Client arrived, connection established!\n";
          std::make_shared<Session>(std::move(socket_), um, mtx)->start();
        }
        do_accept();
      });
}
