#pragma once
#include <string>

#include "../../external/json.hpp"

using namespace std;
namespace telemetry {

struct TelemetryPacket {
  string device_id;
  long long timestamp;
  float temperature;
  float speed;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TelemetryPacket, device_id, timestamp,
                                   temperature, speed)
} // namespace telemetry
