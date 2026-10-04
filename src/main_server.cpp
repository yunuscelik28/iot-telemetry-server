#include "asio/signal_set.hpp"
#include <csignal>
#include <system_error>
#define ASIO_STANDALONE
#include "asio.hpp"
#include "telemetry/server.hpp"
#include <exception>
#include <iostream>

using namespace std;

int main() {
  // Server configuration
  const short SERVER_PORT = 8080;

  /*
   * Async Server Setup:
   * - io_context: The main background engine.
   * - Server: My custom class that listens on the port.
   * - signal_set: Catches Ctrl+C (SIGINT) to stop the engine gracefully instead
   * of crashing.
   * - run(): Starts the loop to process tasks without freezing.
   */
  try {
    asio::io_context io_context;
    Server s(io_context, SERVER_PORT);

    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(
        [&io_context](const asio::error_code &error, int signal_nmbr) {
          if (!error) {
            cout << "\nClose signal received\n";
            io_context.stop();
          }
        });

    io_context.run();
  } catch (std::exception &e) {
    cout << "Server error: " << e.what() << "\n";
  }
  return 0;
}
