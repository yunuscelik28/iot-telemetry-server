#pragma once

#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

/*
 * Session Class:
 * Represents one connection with a client.
 * Inherits enable_shared_from_this so it doesn't destroy itself too early.
 * - do_read(): Reads data until '\n', parses JSON, and saves it to the map
 * using a mutex.
 */
class Session : public std::enable_shared_from_this<Session> {
private:
  static constexpr std::size_t MAX_MESSAGE_SIZE = 4096;

  asio::ip::tcp::socket socket_;
  asio::streambuf receiving_data;
  std::unordered_map<std::string, telemetry::TelemetryPacket> &um_rf;
  std::mutex &mtx_rf;

  void do_read();

public:
  Session(asio::ip::tcp::socket socket,
          std::unordered_map<std::string, telemetry::TelemetryPacket> &map,
          std::mutex &mutex);
  void start();
};
