#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

/*
 * Server Class:
 * The main listener waiting for clients.
 * It holds the unordered_map (data storage) and mutex (for thread safety).
 * - do_accept(): Waits for clients. When one arrives, it creates a new Session.
 * - ~Server(): Destructor that locks the data and exports everything to
 * Report.csv before shutting down.
 */
class Server {
private:
  asio::ip::tcp::acceptor acceptor_;
  std::unordered_map<std::string, telemetry::TelemetryPacket> um;
  std::mutex mtx;

  void do_accept();

public:
  Server(asio::io_context &io_context, short port);

  ~Server();
};
