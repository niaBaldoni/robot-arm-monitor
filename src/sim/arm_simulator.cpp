#include "sim/arm_simulator.h"
#include <mutex>

static ArmState armState;
static std::mutex armMutex;

static float targetVelocityForState(ArmTaskState state) {
    switch (state) {
        case ArmTaskState::Idle: return 0.0f;
        case ArmTaskState::Approach: return 20.0f;
        case ArmTaskState::Grasp: return 0.0f;
        case ArmTaskState::Transport: return 10.0f;
        case ArmTaskState::Place: return 0.0f;
        case ArmTaskState::Retreat: return -20.0f;
        default: return 0.0f;
    }
}

static float durationForState(ArmTaskState state) {
    switch (state) {
        case ArmTaskState::Idle: return 2.0f;
        case ArmTaskState::Approach: return 3.0f;
        case ArmTaskState::Grasp: return 1.0f;
        case ArmTaskState::Transport: return 4.0f;
        case ArmTaskState::Place: return 1.0f;
        case ArmTaskState::Retreat: return 3.0f;
        default: return 0.0f;
    }
}

static ArmTaskState nextState(ArmTaskState current) {
    switch (current) {
        case ArmTaskState::Idle: return ArmTaskState::Approach;
        case ArmTaskState::Approach: return ArmTaskState::Grasp;
        case ArmTaskState::Grasp: return ArmTaskState::Transport;
        case ArmTaskState::Transport: return ArmTaskState::Place;
        case ArmTaskState::Place: return ArmTaskState::Retreat;
        case ArmTaskState::Retreat: return ArmTaskState::Idle;
        default: return ArmTaskState::Idle;
    }
}

ArmState getArmStateSnapshot() {
    ArmState temp;
    {
        std::lock_guard<std::mutex> lock(armMutex);
        temp = armState;
    }
    return temp;
}