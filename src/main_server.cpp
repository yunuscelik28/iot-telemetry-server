#include "asio.hpp"
#include "asio/signal_set.hpp"
#include "telemetry/server.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <system_error>

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
