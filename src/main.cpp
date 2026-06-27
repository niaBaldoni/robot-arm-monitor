#include <iostream>
#include <format>
#include <thread>
#include <chrono>
#include "shared.h"
#include "monitor/monitor.h"
#include "sensors/encoder.h"
#include "sensors/force.h"
#include "sensors/temperature.h"
#include "sim/arm_simulator.h"

void displayThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(dataMutex);
        std::cout << std::format("[STATUS] angle: {:.1f} deg | temp: {:.1f} C | force: {:.1f} N\n", 
            sensorData.angle, sensorData.temperature, sensorData.force);
    }
}

int main() {
    std::jthread t0(armSimulatorThread);
    std::jthread t1(encoderThread);
    std::jthread t2(temperatureThread);
    std::jthread t3(forceThread);
    std::jthread t4(displayThread);
    std::jthread t5(monitorThread);

    return 0;
}