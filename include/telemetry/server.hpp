#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <unordered_map>
#include <mutex>
#include <string>

/*
 * Server Class:
 * The main listener waiting for clients.
 * It holds the unordered_map (data storage) and mutex (for thread safety).
 * - do_accept(): Waits for clients. When one arrives, it creates a new Session.
 */
class Server {
private:
  asio::ip::tcp::acceptor acceptor_;
  std::unordered_map<std::string, telemetry::TelemetryPacket> um;
  std::mutex mtx;

  void do_accept();

public:
  Server(asio::io_context &io_context, short port);
};
