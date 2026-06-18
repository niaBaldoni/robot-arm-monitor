#include "sim/arm_simulator.h"
#include <mutex>
#include <cmath>
#include <algorithm>
#include <thread>

static ArmState armState;
static std::mutex armMutex;
static constexpr float kInertia = 2.0f;
static constexpr float kMaxAcceleration = 30.0f;
static constexpr float kHeat = 0.5f;
static constexpr float kCool = 0.1f;
static constexpr float ambientTemp = 25.0f;
static constexpr int kTickMs = 10;

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

static float moveToward(float current, float target, float maxDelta) {
    float diff = target - current;
    float clampedDiff = std::clamp(diff, -maxDelta, maxDelta);
    return current + clampedDiff;
}

static void tick(ArmState& state, float dt) {
    float oldVelocity = state.currentVelocity;
    float maxDelta = kMaxAcceleration * dt;
    state.currentVelocity = moveToward(oldVelocity, targetVelocityForState(state.currentState), maxDelta);

    float currentAcceleration = (state.currentVelocity - oldVelocity) / dt;
    state.force = kInertia * std::abs(currentAcceleration);

    state.temperature += (kHeat * state.force - kCool * (state.temperature - ambientTemp)) * dt;

    state.angle += state.currentVelocity * dt;
    if (state.angle >= 360.0f) state.angle -= 360.0f;
    if (state.angle < 0.0f) state.angle += 360.0f;

    state.timeInState += dt;
    if (state.timeInState >= durationForState(state.currentState)) {
        state.currentState = nextState(state.currentState);
        state.timeInState = 0.0f;
    }
}

void armSimulatorThread() {
    float dt = float(kTickMs) / 1000;

    while(true) {
        {
            std::lock_guard<std::mutex> lock(armMutex);
            tick(armState, dt);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kTickMs));
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