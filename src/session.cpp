#include "telemetry/session.hpp"
#include "json.hpp"
#include <iostream>
#include <istream>

using namespace std;
using namespace nlohmann;

Session::Session(asio::ip::tcp::socket socket,
                 unordered_map<string, telemetry::TelemetryPacket> &map,
                 mutex &mutex)
    : socket_(std::move(socket)), read_buffer(MAX_MESSAGE_SIZE),
      device_map(map), map_mutex(mutex) {}

void Session::start() { do_read(); }

void Session::do_read() {
  // keep a shared_ptr in the lambda so the Session lives until it's called
  auto self(shared_from_this());
  asio::async_read_until(
      socket_, read_buffer, "\n",
      // second parameter is the byte count, I don't use it so no name
      [this, self](asio::error_code error, size_t) {
        if (!error) {
          istream stream(&read_buffer);
          string line;
          getline(stream, line);
          try {
            json parsed = json::parse(line);
            telemetry::TelemetryPacket packet = parsed;
            cout << "Device id: " << packet.device_id
                 << ", temp: " << packet.temperature
                 << ", speed: " << packet.speed << "\n";

            lock_guard<mutex> lock(map_mutex);
            device_map[packet.device_id] = packet;
            cout << "Known devices: " << device_map.size() << "\n";
          } catch (const json::exception &e) {
            // json::exception is the base class of all nlohmann errors, so
            // this catches broken JSON (parse_error) and also missing fields
            // (out_of_range) or wrong types (type_error).
            // Before, I only caught parse_error and a packet with a missing
            // field crashed the whole server.
            cout << e.what() << "\n";
          }

          do_read(); // read the next line
        } else if (error == asio::error::eof) {
          // client closed the connection normally
          cout << "Client disconnected\n";
        } else if (error == asio::error::not_found) {
          // buffer got full before a '\n' came
          cerr << "Message too long (limit " << MAX_MESSAGE_SIZE
               << " bytes), closing connection\n";
        } else {
          cerr << "Reading error: " << error.message() << "\n";
        }
        // in the error cases we don't call do_read() again, so the Session
        // is destroyed and the socket closes by itself
      });
}
