#include <thread>
#include <chrono>
#include "shared.h"
#include "sim/arm_simulator.h"
#include "sensors/encoder.h"

void encoderThread(RobotArm& arm) {
    while (true) {
        int randMs = getRandMs();
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.angle = arm.getAngle();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randMs));
    }
}