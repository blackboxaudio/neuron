#include <gtest/gtest.h>
#include "neuron.h"
#include <cmath>

using namespace neuron;

class SmoothedValueTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Default test parameters
        sampleRate = 44100.0;
        rampLengthMs = 100.0; // 100ms ramp
    }

    // Helper function for floating point comparisons
    bool IsApproximatelyEqual(float a, float b, float tolerance = 1e-6f) {
        return std::abs(a - b) < tolerance;
    }

    double sampleRate;
    double rampLengthMs;
};

// Test default constructor
TEST_F(SmoothedValueTest, DefaultConstructor) {
    SmoothedValue sv;
    EXPECT_FLOAT_EQ(sv.GetNextValue(), 0.0f);
}

// Test constructor with initial value
TEST_F(SmoothedValueTest, ConstructorWithInitialValue) {
    const float initialValue = 5.0f;
    SmoothedValue sv(initialValue);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), initialValue);
}

// Test Reset with valid parameters
TEST_F(SmoothedValueTest, ResetWithValidParameters) {
    SmoothedValue sv(1.0f);
    sv.Reset(sampleRate, rampLengthMs);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 1.0f);

    sv.SetTargetValue(2.0f);
    float nextValue = sv.GetNextValue();
    EXPECT_GT(nextValue, 1.0f);
    EXPECT_LT(nextValue, 2.0f);
}

// Test Reset with invalid parameters
TEST_F(SmoothedValueTest, ResetWithInvalidParameters) {
    SmoothedValue sv(1.0f);
    sv.SetTargetValue(2.0f);

    // Store the state before any reset attempts
    float valueBefore = sv.GetNextValue();

    // Invalid resets should be ignored, so behavior should remain consistent
    sv.Reset(-1.0, rampLengthMs);
    float valueAfter1 = sv.GetNextValue();

    sv.Reset(sampleRate, -1.0);
    float valueAfter2 = sv.GetNextValue();

    // Values should continue progressing toward target
    EXPECT_GT(valueAfter1, valueBefore);  // Still progressing
    EXPECT_GT(valueAfter2, valueAfter1);  // Still progressing
    EXPECT_LT(valueAfter2, 2.0f);         // Haven't reached target yet
}

// Test SetTargetValue
TEST_F(SmoothedValueTest, SetTargetValue) {
    SmoothedValue sv(0.0f);
    sv.Reset(sampleRate, rampLengthMs);

    sv.SetTargetValue(10.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 0.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value2, 10.0f);
}

// Test GetNextValue reaches target eventually
TEST_F(SmoothedValueTest, GetNextValueReachesTarget) {
    SmoothedValue sv(0.0f);
    sv.Reset(sampleRate, 50.0);
    sv.SetTargetValue(1.0f);

    int expectedSamples = static_cast<int>((50.0 / 1000.0) * sampleRate);

    float lastValue = 0.0f;
    for (int i = 0; i < expectedSamples + 10; ++i) {
        lastValue = sv.GetNextValue();
    }

    EXPECT_FLOAT_EQ(lastValue, 1.0f);
}

// Test smoothing with negative values
TEST_F(SmoothedValueTest, SmoothingWithNegativeValues) {
    SmoothedValue sv(5.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(-5.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_LT(value1, 5.0f);
    EXPECT_LT(value2, value1);
    EXPECT_GT(value1, -5.0f);
}

// Test Skip functionality
TEST_F(SmoothedValueTest, SkipSamples) {
    SmoothedValue sv1(0.0f);
    SmoothedValue sv2(0.0f);

    sv1.Reset(sampleRate, rampLengthMs);
    sv2.Reset(sampleRate, rampLengthMs);

    sv1.SetTargetValue(10.0f);
    sv2.SetTargetValue(10.0f);

    for (int i = 0; i < 100; ++i) {
        sv1.GetNextValue();
    }

    sv2.Skip(100);

    float value1 = sv1.GetNextValue();
    float value2 = sv2.GetNextValue();

    EXPECT_TRUE(IsApproximatelyEqual(value1, value2, 1e-5f));
}

// Test when ramp length is zero
TEST_F(SmoothedValueTest, ZeroRampLength) {
    SmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 0.0);
    sv.SetTargetValue(5.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
}

// Test very short ramp length
TEST_F(SmoothedValueTest, VeryShortRampLength) {
    SmoothedValue sv(0.0f);
    sv.Reset(sampleRate, 0.1);
    sv.SetTargetValue(1.0f);

    float value = 0.0f;
    for (int i = 0; i < 10; ++i) {
        value = sv.GetNextValue();
    }

    EXPECT_FLOAT_EQ(value, 1.0f);
}

// Test multiple target changes
TEST_F(SmoothedValueTest, MultipleTargetChanges) {
    SmoothedValue sv(0.0f);
    sv.Reset(sampleRate, 50.0);

    sv.SetTargetValue(10.0f);
    for (int i = 0; i < 100; ++i) {
        sv.GetNextValue();
    }

    float valueBeforeChange = sv.GetNextValue();
    sv.SetTargetValue(-5.0f);
    float valueAfterChange = sv.GetNextValue();

    EXPECT_NE(valueBeforeChange, valueAfterChange);

    for (int i = 0; i < 3000; ++i) {
        sv.GetNextValue();
    }
    EXPECT_FLOAT_EQ(sv.GetNextValue(), -5.0f);
}

// Test same target value
TEST_F(SmoothedValueTest, SameTargetValue) {
    SmoothedValue sv(5.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(5.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
}

// Test large sample rate
TEST_F(SmoothedValueTest, LargeSampleRate) {
    SmoothedValue sv(0.0f);
    sv.Reset(192000.0, 100.0);
    sv.SetTargetValue(1.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 0.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value1, 0.1f);
}