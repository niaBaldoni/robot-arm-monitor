#pragma once

enum class ArmTaskState {
    Idle,
    Approach,
    Grasp,
    Transport,
    Place,
    Retreat
};

struct ArmState {
    ArmTaskState currentState = ArmTaskState::Idle;
    float angle       = 0.0;
    float currentVelocity = 0.0;
    float temperature = 25.0;
    float force       = 0.0;
    float timeInState = 0.0;
};

void armSimulatorThread();

ArmState getArmStateSnapshot();