#include <iostream>
#include <thread>
#include <chrono>
#include "sensors/temperature.h"

void temperatureThread() {
    float temp = 40.0;
    while (true) {
        temp += (rand() % 3 - 1) * 0.1;
        std::cout << "[TEMP]     temp:  " << temp << " C\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}