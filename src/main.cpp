#include <iostream>
#include <thread>
#include <chrono>
#include "sensors/encoder.h"
#include "sensors/force.h"
#include "sensors/temperature.h"
#include "shared.h"
#include "monitor/monitor.h"

void displayThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(dataMutex);
        std::cout << "[STATUS] angle: " << sensorData.angle
                  << " deg | temp: " << sensorData.temperature
                  << " C | force: " << sensorData.force
                  << " N\n";
    }
}

int main() {
    std::thread t1(encoderThread);
    std::thread t2(temperatureThread);
    std::thread t3(forceThread);
    std::thread t4(displayThread);
    std::thread t5(monitorThread);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}