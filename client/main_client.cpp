#include "asio.hpp"
#include "telemetry/data_model.hpp"
#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std;
using asio::ip::tcp;

// Ctrl+C sets this to 0 and the main loop stops.
// Inside a signal handler you can't safely do much (not even cout),
// so the handler only changes this flag. That's why it's sig_atomic_t
// and not a normal bool.
volatile std::sig_atomic_t is_sensor_on = 1;

void handle_signal(int) { is_sensor_on = 0; }

int main(int argc, char *argv[]) {
  signal(SIGINT, handle_signal);
  try {
    // usage: ./mclient [device_id] [server_ip] [server_port]
    string device_id = "sensor_01";
    string server_ip = "127.0.0.1";
    string server_port = "8080"; // string, because resolve() takes a string

    if (argc > 1) {
      device_id = argv[1];
    }
    if (argc > 2) {
      server_ip = argv[2];
    }
    if (argc > 3) {
      server_port = argv[3];
    }

    // random numbers to fake the sensor values
    random_device seed;
    mt19937 generator(seed());
    uniform_real_distribution<float> temp_dist(20.0, 30.0);
    uniform_real_distribution<float> speed_dist(60.0, 120.0);

    /*
     * Network setup:
     * - io_context: asio's main object, everything needs it
     * - resolver: turns "localhost" or an IP + port into endpoints
     * - asio::connect: tries the endpoints one by one until one works
     * If the server is not running, connect throws and we end up in catch.
     */
    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::socket socket(io_context);
    auto endpoints = resolver.resolve(server_ip, server_port);
    asio::connect(socket, endpoints);

    while (is_sensor_on) {
      auto now = chrono::system_clock::now();

      telemetry::TelemetryPacket packet;
      packet.device_id = device_id;
      packet.timestamp =
          chrono::duration_cast<chrono::seconds>(now.time_since_epoch())
              .count();
      packet.temperature = temp_dist(generator);
      packet.speed = speed_dist(generator);

      // struct -> JSON -> one line of text. The server reads until '\n'
      // so every message has to end with it.
      nlohmann::json packet_json = packet;
      string message = packet_json.dump() + "\n";
      // if the server is gone this throws, and we go to catch
      asio::write(socket, asio::buffer(message));

      this_thread::sleep_for(chrono::seconds(2));
    }
    std::cout << "Sensor shutting down...\n";
  } catch (const std::exception &e) {
    cerr << "Client error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
