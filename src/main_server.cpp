#define ASIO_STANDALONE
#include "asio.hpp"
#include "telemetry/server.hpp"
#include <exception>
#include <iostream>

int main() {
  // Server configuration
  const short SERVER_PORT = 8080;

  // Setup the async server
  // io_context handles the background tasks
  // run() keeps the program alive and listening
  try {
    asio::io_context io_context;
    Server s(io_context, SERVER_PORT);

    io_context.run();
  } catch (std::exception &e) {
    std::cout << "Server error: " << e.what() << "\n";
  }
  return 0;
}
