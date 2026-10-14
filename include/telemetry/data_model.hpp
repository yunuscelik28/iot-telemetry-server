#pragma once
#include <string>

#include "json.hpp"

namespace telemetry {

struct TelemetryPacket {
  std::string device_id;
  long long timestamp;
  float temperature;
  float speed;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TelemetryPacket, device_id, timestamp,
                                   temperature, speed)
} // namespace telemetry
