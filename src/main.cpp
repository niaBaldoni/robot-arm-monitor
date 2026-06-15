#include <iostream>
#include <thread>
#include <chrono>
#include "sensors/encoder.h"
#include "sensors/force.h"
#include "sensors/temperature.h"

int main() {
    std::thread t1(encoderThread);
    std::thread t2(temperatureThread);
    std::thread t3(forceThread);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}