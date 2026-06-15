cmake_minimum_required(VERSION 3.20)
project(robot_arm_monitor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot_arm_monitor
    src/main.cpp
    src/sensors/encoder.cpp
    src/sensors/temperature.cpp
    src/sensors/force.cpp
    src/shared.cpp
)

target_include_directories(robot_arm_monitor PRIVATE include)