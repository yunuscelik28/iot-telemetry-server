#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <unordered_map>
#include <mutex>
#include <string>

// The main server class that listens for incoming connections
// It holds the central data map and a mutex to keep it thread-safe
class Server {
private:
  asio::ip::tcp::acceptor acceptor_;
  std::unordered_map<std::string, telemetry::TelemetryPacket> um;
  std::mutex mtx;

  void do_accept();

public:
  Server(asio::io_context &io_context, short port);
};
