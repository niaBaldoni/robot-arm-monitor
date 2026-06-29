#pragma once
#include <atomic>

class RobotArm {
    public:
        void run();
        float getAngle();
        float getTemperature();
        float getForce();

    private:
        enum class State {
            IDLE,
            APPROACH,
            GRASP,
            TRANSPORT,
            PLACE,
            RETREAT,
            OBSTRUCTED,
            MAX_STATE
        };

        struct InternalState {
            State currentState = State::IDLE;
            float velocity = 0.0f;
            float temperature = 25.0f;
            float force = 0.0f;
            float angle = 0.0f;
            float timeInState = 0.0f;
        };

        void validateState(State state);
        static float targetVelocityForState(State state);
        static float durationForState(State state);
        static State nextState(State current);
        static float moveToward(float current, float target, float maxDelta);
        void tick(float dt);

        InternalState internalState_;

        std::atomic<float> angle_       {0.0f};
        std::atomic<float> temperature_ {25.0f};
        std::atomic<float> force_       {0.0f};
};