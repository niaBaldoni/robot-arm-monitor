#include <thread>
#include <chrono>
#include <cstdlib>
#include "shared.h"
#include "sim/arm_simulator.h"
#include "sensors/temperature.h"

void temperatureThread() {
    while (true) {
        int randMs = rand()%(110-95 + 1) + 95;
        ArmState snapshot = getArmStateSnapshot();
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.temperature = snapshot.temperature;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randMs));
    }
}