#include <thread>
#include <chrono>
#include "shared.h"
#include "sim/arm_simulator.h"
#include "sensors/temperature.h"

void temperatureThread(RobotArm& arm) {
    while (true) {
        int randMs = getRandMs();
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.temperature = arm.getTemperature();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randMs));
    }
}