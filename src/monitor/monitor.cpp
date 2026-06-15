#include <iostream>
#include <thread>
#include <chrono>
#include "monitor/monitor.h"
#include "shared.h"

const float MAX_FORCE       = 7.0;
const float MAX_TEMPERATURE = 40.9;

void monitorThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        std::lock_guard<std::mutex> lock(dataMutex);
        
        if (sensorData.force > MAX_FORCE) {
            std::cout << "[DANGER] Force exceeded: " << sensorData.force << " N\n";
        }
        
        if (sensorData.temperature > MAX_TEMPERATURE) {
            std::cout << "[DANGER] Temperature exceeded: " << sensorData.temperature << " C\n";
        }
    }
}