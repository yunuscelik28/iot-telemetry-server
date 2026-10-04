# IoT Telemetry Server & Client Project

Hello! This is my C++ project where I built an asynchronous TCP server and a client to simulate IoT telemetry data collection. I developed this project to learn about modern C++ concepts and network programming.

## What I Learned and Used

- **Asynchronous Architecture:** I used standalone `ASIO` to create a non-blocking server. It can handle multiple client connections simultaneously without freezing.
- **Thread Safety:** I used `std::mutex` and `std::lock_guard` to prevent race conditions when different clients send data at the same time and try to write to the central storage.
- **Memory Management:** I learned how to use smart pointers (`std::shared_ptr`, `std::enable_shared_from_this`) to safely manage client session lifecycles.
- **JSON Serialization:** I used the `nlohmann/json` library to easily convert simulated hardware data into JSON strings and parse them back into C++ objects.
- **Separation of Concerns:** I structured my project into separate header (`.hpp`) and source (`.cpp`) files for better code organization.

## Project Structure

- **Client (`mclient`)**: Simulates an IoT sensor. It generates random temperature and speed data, converts it to JSON, and sends it to the server.
- **Server (`mserver`)**: An async server that accepts client connections, parses the JSON data, and safely stores it in a central map using a mutex lock.

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

3. **Run the Client:**

```bash
./mclient
```
