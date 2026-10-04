#define ASIO_STANDALONE
#include "asio.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/address.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/registered_buffer.hpp"
#include "asio/write.hpp"
#include "telemetry/data_model.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std;
using asio::ip::tcp;

int main() {
  // Target server information
  const std::string SERVER_IP = "127.0.0.1";
  const short SERVER_PORT = 8080;

  // Set up random number generation for simulating sensor data
  random_device rng;
  mt19937 mt(rng());
  uniform_real_distribution<float> dist(20.0, 30.0);
  uniform_real_distribution<float> speed_dist(60.0, 120.0);

  // Setup ASIO networking stuff
  // io_context is the core engine, socket connects to our server
  asio::io_context io_context;
  tcp::resolver resolver(io_context);
  tcp::socket socket(io_context);
  tcp::endpoint endpoint(asio::ip::make_address(SERVER_IP), SERVER_PORT);
  socket.connect(endpoint);

  bool is_sensor_on = true;
  while (is_sensor_on) {
    // Get the current system time for the timestamp
    auto timepoint = chrono::system_clock::now();

    // Populate the telemetry packet and serialize it to a JSON string
    telemetry::TelemetryPacket my_packet;
    my_packet.device_id = "sensor_01";
    my_packet.timestamp =
        chrono::duration_cast<chrono::seconds>(timepoint.time_since_epoch())
            .count();
    my_packet.temperature = dist(mt);
    my_packet.speed = speed_dist(mt);

    nlohmann::json json_object = my_packet;

    string sending_txt = json_object.dump();
    string sending_msg = sending_txt + "\n";
    asio::write(socket, asio::buffer(sending_msg));

    this_thread::sleep_for(chrono::seconds(2));
  }

  return 0;
}
