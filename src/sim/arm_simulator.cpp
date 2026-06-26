#include "sim/arm_simulator.h"
#include <mutex>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <thread>
#include <cstdlib>

static ArmState armState;
static std::mutex armMutex;
static constexpr float kInertia = 2.0f;
static constexpr float kMaxAcceleration = 30.0f;
static constexpr float kHeat = 0.025f;
static constexpr float kCool = 0.1f;
static constexpr float ambientTemp = 25.0f;
static constexpr int kTickMs = 10;
static constexpr float kObstructionForce = 80.0f;

static float targetVelocityForState(ArmTaskState state) {
    switch (state) {
        case ArmTaskState::Idle: return 0.0f;
        case ArmTaskState::Approach: return 20.0f;
        case ArmTaskState::Grasp: return 0.0f;
        case ArmTaskState::Transport: return 10.0f;
        case ArmTaskState::Place: return 0.0f;
        case ArmTaskState::Retreat: return -20.0f;
        case ArmTaskState::Obstructed: return 0.0f;
        default: throw std::invalid_argument("Unknown ArmTaskState");
    }
}

static float durationForState(ArmTaskState state) {
    switch (state) {
        case ArmTaskState::Idle: return 2.0f;
        case ArmTaskState::Approach: return 3.0f;
        case ArmTaskState::Grasp: return 1.0f;
        case ArmTaskState::Transport: return 4.0f;
        case ArmTaskState::Place: return 1.0f;
        case ArmTaskState::Retreat: return 5.009f;
        case ArmTaskState::Obstructed: return 999.9f;
        default: throw std::invalid_argument("Unknown ArmTaskState");
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
        case ArmTaskState::Obstructed: return ArmTaskState::Place;
        default: throw std::invalid_argument("Unknown ArmTaskState");
    }
}

static float moveToward(float current, float target, float maxDelta) {
    float diff = target - current;
    float clampedDiff = std::clamp(diff, -maxDelta, maxDelta);
    return current + clampedDiff;
}

static void validateState(ArmTaskState state) {
    if (state >= ArmTaskState::MAX_STATE) {
        throw std::invalid_argument("Unknown ArmTaskState");
    }
}

static void tick(ArmState& state, float dt) {

    validateState(state.currentState);

    float oldVelocity = state.currentVelocity;
    float maxDelta = kMaxAcceleration * dt;
    state.currentVelocity = moveToward(oldVelocity, targetVelocityForState(state.currentState), maxDelta);

    float currentAcceleration = (state.currentVelocity - oldVelocity) / dt;
    state.force = kInertia * std::abs(currentAcceleration);
    if (state.currentState == ArmTaskState::Obstructed) {
        state.force += kObstructionForce;
    }

    state.temperature += (kHeat * state.force - kCool * (state.temperature - ambientTemp)) * dt;

    state.angle += state.currentVelocity * dt;
    if (state.angle >= 360.0f) state.angle -= 360.0f;
    if (state.angle < 0.0f) state.angle += 360.0f;

    state.timeInState += dt;

    if (state.currentState == ArmTaskState::Obstructed) {
        int randChance = rand()%(1000-1 + 1) + 1;
        if (randChance > 999) {
            state.currentState = nextState(state.currentState);
            state.timeInState = 0.0f;
        }
    } else if (state.timeInState >= durationForState(state.currentState)) {
        ArmTaskState next = nextState(state.currentState);
        if (next == ArmTaskState::Transport && rand() % 2 == 0) {
            next = ArmTaskState::Obstructed;
        }
        state.currentState = next;
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