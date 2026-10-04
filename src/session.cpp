#include "telemetry/session.hpp"
#include "json.hpp"
#include <iostream>
#include <istream>

using namespace std;
using namespace nlohmann;

Session::Session(asio::ip::tcp::socket socket,
                 unordered_map<string, telemetry::TelemetryPacket> &map, 
                 mutex &mutex)
    : socket_(std::move(socket)), um_rf(map), mtx_rf(mutex) {}

void Session::start() { 
    do_read(); 
}

void Session::do_read() {
    auto self(shared_from_this());
    asio::async_read_until(socket_, receiving_data, "\n",
                           [this, self](asio::error_code er, size_t length) {
                             if (!er) {
                               istream is(&receiving_data);
                               string data;
                               getline(is, data);
                               try {
                                   // Try to parse the incoming string as JSON
                                   json myjson = json::parse(data);
                                   telemetry::TelemetryPacket myTel = myjson;
                                   cout << "Received temp: " << myTel.temperature << "\n";
                                   
                                   // Lock the map so multiple clients don't crash the server when writing
                                   lock_guard<mutex> lock(mtx_rf);
                                   um_rf[myTel.device_id] = myTel;
                                   cout << "Total devices connected: " << um_rf.size() << "\n";
                               } catch (json::parse_error& e) {
                                   // If bad data (not JSON) comes in, don't crash! Just print and ignore.
                                   cout << "Oops, received invalid JSON data. Ignoring it...\n";
                               }

                               do_read();
                             }
                           });
}
