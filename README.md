# IoT Telemetry Server & Client Project

Hi there! Here is my C++ project where I implemented an asynchronous TCP server and a client to process IoT telemetry. I've created this project as a part of learning new C++ features and network programming.

## What I Learnt and Used

- **Asynchronous Design Pattern**: I've utilized standalone `ASIO` framework to build non-blocking server. It can deal with many clients' connections without blocking execution.
- **Smart Pointers**: I've learnt how to properly use `std::shared_ptr` and `std::enable_shared_from_this` to implement lifecycle management of the client session. When a session stops reading, the last `shared_ptr` goes away and the connection closes by itself.
- **JSON Serialization**: I've used the `nlohmann/json` library to serialize the simulated hardware data and store it as JSON object in order to easily deserialize and use in C++.
- **Error Handling**: The server doesn't crash on bad input anymore. I learnt that nlohmann throws different exceptions (`parse_error`, `out_of_range`, `type_error`) and that catching their base class `json::exception` handles all of them. The client also catches network errors (server down, connection lost) and exits with a proper error code instead of crashing.
- **Protecting the Server**: Every connection has a 4096 byte receive buffer limit (`static constexpr` + `asio::streambuf` max size), so a client that never sends `\n` can't fill up the server's memory.
- **Data Persistence & Graceful Shutdown**: I've implemented signal handling (`asio::signal_set`) to safely catch termination signals, securely shutting down the server and automatically exporting all gathered telemetry data to a `Report.csv` file using the Server's destructor. The client stops cleanly on Ctrl+C too, using a `std::signal` handler that only sets a `sig_atomic_t` flag.
- **Concurrency**: I've used `std::mutex` and `std::lock_guard` around the central device map. Right now `io_context` runs on a single thread so the mutex isn't strictly needed yet. Running it on multiple threads is my next step.
- **CMake**: Compile definitions (`ASIO_STANDALONE`, `_WIN32_WINNT`) are set with `target_compile_definitions` instead of `#define`s spread across files.
- **Encapsulation and Separation of Concerns**: I've organized the source code into separate header (`*.hpp`) and implementation (`*.cpp`) files.

## Project Structure

- **Client (`mclient`)**: Simulates an IoT device. Every 2 seconds it generates random temperature and speed values, turns them into a JSON message and sends it to the server.
- **Server (`mserver`)**: Async server which accepts connections from clients, parses the JSON messages and stores the latest packet of every device in a central map.

```
include/telemetry/   headers (data model, Server, Session)
src/                 server code
client/              client code
external/            asio and nlohmann/json (header-only)
```

## Requirements

- A C++17 compiler (I use g++ 13 on Ubuntu 24.04 / WSL)
- CMake 3.28 or newer
- No extra libraries to install, asio and nlohmann/json are already in `external/`

## How to Run

1. **Build the project:**

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

2. **Run the Server:**

```bash
./mserver
```

3. **Run one or more Clients** (each one in a new terminal):

```bash
./mclient sensor_01
./mclient sensor_02 localhost 8080
```

All arguments are optional: `./mclient [device_id] [server_ip] [server_port]`. The defaults are `sensor_01`, `127.0.0.1` and `8080`.

4. **Stop with Ctrl+C.** When the server stops, it writes the last data of every device to `Report.csv`.

### Example output

```
Server is up, waiting for client on port 8080...
Client arrived, connection established!
Device id: sensor_01, temp: 22.4218, speed: 92.0205
Known devices: 1
Client arrived, connection established!
Device id: sensor_02, temp: 25.9045, speed: 81.3319
Known devices: 2
Client disconnected
^C
Close signal received
Saving data from memory to disk...
Data successfully written to 'Report.csv'!
```

## What I Tested

- Starting the client without a server → prints an error and exits with code 1
- 3 clients at the same time → server shows `Known devices: 3`
- Sending JSON with a missing field → error is logged, server keeps running
- Sending 5000 bytes without `\n` → that connection is closed, server keeps running
- Ctrl+C on a client or the server → both shut down cleanly, `Report.csv` has all devices

## About AI Assistance

I used an AI assistant (Claude) as a code reviewer and mentor while improving this project. It reviewed my code, pointed out bugs, told me what to research and checked my changes after each step. I implemented the fixes myself. It also helped with testing, git commands and a final pass on comments and variable names.
