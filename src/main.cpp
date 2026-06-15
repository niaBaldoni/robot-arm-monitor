#include <iostream>
#include <thread>
#include <chrono>

void encoderThread() {
    float angle = 0.0;
    while (true) {
        angle += 1.5;
        if (angle > 360.0) angle = 0.0;
        std::cout << "[ENCODER]  angle: " << angle << " deg\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void temperatureThread() {
    float temp = 40.0;
    while (true) {
        temp += (rand() % 3 - 1) * 0.1;
        std::cout << "[TEMP]     temp:  " << temp << " C\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void forceThread() {
    float force = 3.0;
    while (true) {
        force += (rand() % 5 - 2) * 0.2;
        if (force < 0) force = 0;
        std::cout << "[FORCE]    force: " << force << " N\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

int main() {
    std::thread t1(encoderThread);
    std::thread t2(temperatureThread);
    std::thread t3(forceThread);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}