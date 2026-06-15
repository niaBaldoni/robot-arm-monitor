#pragma once
#include <mutex>

struct SensorData {
    float angle       = 0.0;
    float temperature = 0.0;
    float force       = 0.0;
};

extern SensorData sensorData;
extern std::mutex dataMutex;