#include "asio.hpp"
#include "telemetry/server.hpp"
#include <exception>
#include <iostream>

using namespace std;

int main() {
  const unsigned short server_port = 8080;

  /*
   * How the server starts:
   * - io_context: asio's event loop, runs all the async stuff
   * - Server: my class, starts listening on the port
   * - signal_set: catches Ctrl+C (SIGINT) and SIGTERM so I can stop the loop
   *   myself instead of the program just getting killed
   * - run(): blocks here and handles events until stop() is called
   */
  try {
    asio::io_context io_context;
    Server server(io_context, server_port);

    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&io_context](const asio::error_code &error, int) {
      if (!error) {
        cout << "\nClose signal received\n";
        io_context.stop();
      }
    });

    io_context.run();
    // when we leave this scope, ~Server() runs and saves Report.csv
  } catch (const std::exception &e) {
    cerr << "Server error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
