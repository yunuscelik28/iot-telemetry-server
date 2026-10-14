#pragma once
#include <string>

#include "json.hpp"

namespace telemetry {

// One message coming from a sensor. Client fills this, sends it as JSON,
// server turns the JSON back into this struct.
struct TelemetryPacket {
  std::string device_id;
  long long timestamp; // seconds since epoch
  float temperature;
  float speed;
};

// nlohmann macro that writes to_json/from_json for me.
// from_json uses j.at() inside, so a missing field throws an exception
// (found this out when the server crashed on a half packet, see session.cpp)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TelemetryPacket, device_id, timestamp,
                                   temperature, speed)
} // namespace telemetry
