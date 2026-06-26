#pragma once
#include <cstdlib>
#include <mutex>

inline int getRandMs() {
    return rand()%(110-95 + 1) + 95;
}

struct SensorData {
    float angle       = 0.0;
    float temperature = 0.0;
    float force       = 0.0;
};

extern SensorData sensorData;
extern std::mutex dataMutex;