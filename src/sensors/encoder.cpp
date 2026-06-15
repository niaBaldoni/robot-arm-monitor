#include <iostream>
#include <thread>
#include <chrono>
#include "sensors/encoder.h"
#include "shared.h"

void encoderThread() {
    float angle = 0.0;
    int counter = 0;

    while (true) {
        angle += 1.5;
        if (angle > 360.0) angle = 0.0;

        // every 300 iterations (~30 seconds) simulate a sudden lurch
        if (++counter % 300 == 0) {
            angle += 90.0;
            if (angle > 360.0) angle -= 360.0;
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.angle = angle;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}