#pragma once

#include "telemetry/data_model.hpp"
#define ASIO_STANDALONE
#include "asio.hpp"
#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>

/*
 * Session Class:
 * Represents one connection with a client.
 * Inherits enable_shared_from_this so it doesn't destroy itself too early.
 * - do_read(): Reads data until '\n', parses JSON, and saves it to the map using a mutex.
 */
class Session : public std::enable_shared_from_this<Session> {
private:
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
