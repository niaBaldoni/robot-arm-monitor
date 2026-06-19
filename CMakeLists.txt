cmake_minimum_required(VERSION 3.20)
project(robot_arm_monitor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot_arm_monitor
    src/main.cpp
    src/monitor/monitor.cpp
    src/sensors/encoder.cpp
    src/sensors/temperature.cpp
    src/sensors/force.cpp
    src/sim/arm_simulator.cpp
    src/shared.cpp
)

target_include_directories(robot_arm_monitor PRIVATE include)

# Tests
find_package(GTest REQUIRED)
enable_testing()

add_executable(run_tests tests/test_sensors.cpp)
target_link_libraries(run_tests GTest::GTest GTest::Main)
target_include_directories(run_tests PRIVATE include)

add_test(NAME SensorTests COMMAND run_tests)