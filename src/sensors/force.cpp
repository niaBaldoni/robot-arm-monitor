#include <thread>
#include <chrono>
#include "shared.h"
#include "sim/arm_simulator.h"
#include "sensors/force.h"

void forceThread() {
    while (true) {
        int randMs = getRandMs();
        ArmState snapshot = getArmStateSnapshot();
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.force = snapshot.force;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randMs));
    }
}