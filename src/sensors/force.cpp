#include <thread>
#include <chrono>
#include "shared.h"
#include "sim/arm_simulator.h"
#include "sensors/force.h"

void forceThread(RobotArm& arm) {
    while (true) {
        int randMs = getRandMs();
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.force = arm.getForce();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randMs));
    }
}