# IoT Telemetry Server & Client Project

Hi there! Here is my C++ project where I implemented an asynchronous TCP server and a client to process IoT telemetry. I've created this project as a part of learning new C++ features and network programming.

## What I Learnt and Used

- **Asynchronous Design Pattern**: I've utilized standalone `ASIO` framework to build non-blocking server. It can deal with many clients' connections without blocking execution.
- **Concurrency Issues**: I've used `std::mutex` and `std::lock_guard` to ensure safe data write to the central storage by avoiding race condition in case multiple clients will try to store their data at once.
- **Smart Pointers**: I've learnt how to properly use `std::shared_ptr` and `std::enable_shared_from_this` to implement lifecycle management of the client session.
- **JSON Serialization**: I've used the `nlohmann/json` library to serialize the simulated hardware data and store it as JSON object in order to easily deserialize and use in C++.
- **Data Persistence & Graceful Shutdown**: I've implemented signal handling (`asio::signal_set`) to safely catch termination signals, securely shutting down the server and automatically exporting all gathered telemetry data to a `Report.csv` file using the Server's destructor.
- **Encapsulation and Separation of Concerns**: I've organized the source code into separate header (`*.hpp`) and implementation (`*.cpp`) files.

## Project Structure

- **Client (`mclient`)**: Implements an IoT device. Generates random temperature and speed values, then creates JSON message out of these values and sends to the server.
- **Server (`mserver`)**: Async server which accepts connections from clients, parses JSON messages and securely stores them into the central map using mutex lock.

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
