#pragma once

#include "telemetry/data_model.hpp"
#define ASIO_STANDALONE
#include "asio.hpp"
#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>

// Represents a single client connection
// We use enable_shared_from_this to keep it alive during async operations
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
