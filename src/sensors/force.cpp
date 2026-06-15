#include <iostream>
#include <thread>
#include <chrono>
#include "sensors/force.h"
#include "shared.h"

void forceThread() {
    float force = 3.0;
    while (true) {
        force += (rand() % 5 - 2) * 0.2;
        if (force < 0) force = 0;
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.force = force;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}