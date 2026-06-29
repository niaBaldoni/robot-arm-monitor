#include "sim/arm_simulator.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <thread>

static constexpr float kInertia = 2.0f;
static constexpr float kMaxAcceleration = 30.0f;
static constexpr float kHeat = 0.025f;
static constexpr float kCool = 0.1f;
static constexpr float ambientTemp = 25.0f;
static constexpr int kTickMs = 10;
static constexpr float kObstructionForce = 80.0f;

void RobotArm::validateState(State state) {
    if (state >= State::MAX_STATE) {
        throw std::invalid_argument("Unknown RobotArm state");
    }
};

float RobotArm::targetVelocityForState(State state) {
    switch (state) {
        case State::IDLE: return 0.0f;
        case State::APPROACH: return 20.0f;
        case State::GRASP: return 0.0f;
        case State::TRANSPORT: return 10.0f;
        case State::PLACE: return 0.0f;
        case State::RETREAT: return -20.0f;
        case State::OBSTRUCTED: return 0.0f;
        default: throw std::invalid_argument("Unknown RobotArm state");
    }
}

float RobotArm::durationForState(State state) {
    switch (state) {
        case State::IDLE: return 2.0f;
        case State::APPROACH: return 3.0f;
        case State::GRASP: return 1.0f;
        case State::TRANSPORT: return 4.0f;
        case State::PLACE: return 1.0f;
        case State::RETREAT: return 5.009f;
        case State::OBSTRUCTED: return 999.9f;
        default: throw std::invalid_argument("Unknown RobotArm state");
    }
}

RobotArm::State RobotArm::nextState(State current) {
    switch (current) {
        case State::IDLE: return State::APPROACH;
        case State::APPROACH: return State::GRASP;
        case State::GRASP: return State::TRANSPORT;
        case State::TRANSPORT: return State::PLACE;
        case State::PLACE: return State::RETREAT;
        case State::RETREAT: return State::IDLE;
        case State::OBSTRUCTED: return State::PLACE;
        default: throw std::invalid_argument("Unknown RobotArm state");
    }
}

float RobotArm::moveToward(float current, float target, float maxDelta) {
    float diff = target - current;
    float clampedDiff = std::clamp(diff, -maxDelta, maxDelta);
    return current + clampedDiff;
}

void RobotArm::tick(float dt) {
    float oldVelocity = internalState_.velocity;
    float maxDelta = kMaxAcceleration * dt;
    internalState_.velocity = moveToward(oldVelocity, targetVelocityForState(internalState_.currentState), maxDelta);

    float currentAcceleration = (internalState_.velocity - oldVelocity) / dt;
    internalState_.force = kInertia * std::abs(currentAcceleration);
    if (internalState_.currentState == State::OBSTRUCTED) {
        internalState_.force += kObstructionForce;
    }

    internalState_.temperature += (kHeat * internalState_.force - kCool * (internalState_.temperature - ambientTemp)) * dt;

    internalState_.angle += internalState_.velocity * dt;
    if (internalState_.angle >= 360.0f) internalState_.angle -= 360.0f;
    if (internalState_.angle < 0.0f) internalState_.angle += 360.0f;

    internalState_.timeInState += dt;

    if (internalState_.currentState == State::OBSTRUCTED) {
        int randChance = rand()%(1000-1 + 1) + 1;
        if (randChance > 999) {
            internalState_.currentState = nextState(internalState_.currentState);
            internalState_.timeInState = 0.0f;
        }
    } else if (internalState_.timeInState >= durationForState(internalState_.currentState)) {
        State next = nextState(internalState_.currentState);
        if (next == State::TRANSPORT && rand() % 4 == 0) {
            next = State::OBSTRUCTED;
        }
        internalState_.currentState = next;
        internalState_.timeInState = 0.0f;
    }

    angle_.store(internalState_.angle);
    temperature_.store(internalState_.temperature);
    force_.store(internalState_.force);
    
}

float RobotArm::getAngle() { 
    return angle_.load(); }
float RobotArm::getTemperature() { 
    return temperature_.load(); }
float RobotArm::getForce() { 
    return force_.load(); }

void RobotArm::run() {
    float dt = float(kTickMs) / 1000.0f;
    while (true) {
        try {
            tick(dt);
        } catch (const std::exception& e) {
            // invalid state — log and stop
            std::cerr << "[FATAL] RobotArm tick failed: " << e.what() << "\n";
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kTickMs));
    }
}