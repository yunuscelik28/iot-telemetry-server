#pragma once

#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

/*
 * Server class
 * Listens on a port and waits for clients.
 * Keeps the last packet of every device in a map (key = device_id).
 * - do_accept(): waits for a client, makes a new Session for it,
 *   then calls itself again to wait for the next one.
 * - ~Server(): when the server is closing, writes the map to Report.csv
 */
class Server {
private:
  asio::ip::tcp::acceptor acceptor_;
  std::unordered_map<std::string, telemetry::TelemetryPacket> device_map;
  // io_context runs on one thread for now so this isn't really needed yet,
  // but I'll need it when I try running io_context on multiple threads
  std::mutex map_mutex;

  void do_accept();

public:
  Server(asio::io_context &io_context, unsigned short port);

  ~Server();
};
