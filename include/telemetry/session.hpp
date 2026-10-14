#pragma once

#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

/*
 * Session class
 * One Session = one connected client.
 * It inherits enable_shared_from_this so it can keep itself alive while an
 * async read is waiting (the lambda holds a shared_ptr to it).
 * When we stop reading, the last shared_ptr goes away and the Session +
 * socket are destroyed, which also closes the connection.
 * - do_read(): reads one line, parses the JSON, saves it to the map.
 */
class Session : public std::enable_shared_from_this<Session> {
private:
  // A packet is ~100 bytes, 4096 is more than enough.
  // Without a limit, a client that never sends '\n' can fill up the RAM.
  // static constexpr = one copy for the whole class, so the order of the
  // members below doesn't matter for it.
  static constexpr std::size_t MAX_MESSAGE_SIZE = 4096;

  asio::ip::tcp::socket socket_;
  asio::streambuf read_buffer;
  // references to the map and mutex that live inside Server
  std::unordered_map<std::string, telemetry::TelemetryPacket> &device_map;
  std::mutex &map_mutex;

  void do_read();

public:
  Session(asio::ip::tcp::socket socket,
          std::unordered_map<std::string, telemetry::TelemetryPacket> &map,
          std::mutex &mutex);
  void start();
};
