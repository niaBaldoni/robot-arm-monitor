#include <iostream>
#include <format>
#include <thread>
#include <chrono>
#include "monitor/monitor.h"
#include "shared.h"

const float MAX_FORCE       = 70;
const float MAX_TEMPERATURE = 60;
const float MAX_VELOCITY     = 30.0; // degrees per second

void monitorThread() {
    float previousAngle = 0.0;
    auto previousTime = std::chrono::steady_clock::now();

    bool forceAlerted    = false;
    bool tempAlerted     = false;
    bool velocityAlerted = false;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - previousTime).count();
        
        std::lock_guard<std::mutex> lock(dataMutex);

        float angleDiff = sensorData.angle - previousAngle;
        if (angleDiff < -180.0) angleDiff += 360.0;
        if (angleDiff > 180.0)  angleDiff -= 360.0;
        float velocity = std::abs(angleDiff) / elapsed;
        
        if (sensorData.force > MAX_FORCE) {
            if (!forceAlerted) {
                std::cout << std::format("[DANGER] Force exceeded: {:.1f} N\n", sensorData.force);
                forceAlerted = true;
            }
        } else {
            forceAlerted = false;
        }
        
        if (sensorData.temperature > MAX_TEMPERATURE) {
            if (!tempAlerted) {
                std::cout << std::format ("[DANGER] Temperature exceeded: {:.1f} C\n", sensorData.temperature);
                tempAlerted = true;
            }
        } else {
            tempAlerted = false;
        }

        if (velocity > MAX_VELOCITY) {
            if (!velocityAlerted) {
                std::cout << std::format ("[DANGER] Velocity exceeded: {:.1f} deg/s\n", velocity);
                velocityAlerted = true;
            }
        } else {
            velocityAlerted = false;
        }

        previousAngle = sensorData.angle;
        previousTime = now;
    }
}