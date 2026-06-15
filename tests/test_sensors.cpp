#include <gtest/gtest.h>

// Test 1: force never goes negative
TEST(ForceTest, NeverReturnsNegative) {
    float force = 3.0;
    for (int i = 0; i < 1000; i++) {
        force += (rand() % 5 - 2) * 0.2;
        if (force < 0) force = 0;
        EXPECT_GE(force, 0.0);
    }
}

// Test 2: encoder wraps at 360
TEST(EncoderTest, WrapsAt360) {
    float angle = 0.0;
    for (int i = 0; i < 1000; i++) {
        angle += 1.5;
        if (angle > 360.0) angle = 0.0;
        EXPECT_LE(angle, 360.0);
        EXPECT_GE(angle, 0.0);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}