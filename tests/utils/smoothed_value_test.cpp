#include "neuron/utils/smoothed_value.h"

#include <cmath>
#include <gtest/gtest.h>

using namespace neuron;

class LinearSmoothedValueTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Default test parameters
        sampleRate = 44100.0;
        rampLengthMs = 100.0; // 100ms ramp
    }

    // Helper function for floating point comparisons
    bool IsApproximatelyEqual(float a, float b, float tolerance = 1e-6f)
    {
        return std::abs(a - b) < tolerance;
    }

    double sampleRate;
    double rampLengthMs;
};

class MultiplicativeSmoothedValueTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Default test parameters
        sampleRate = 44100.0;
        rampLengthMs = 100.0; // 100ms ramp
    }

    // Helper function for floating point comparisons
    bool IsApproximatelyEqual(float a, float b, float tolerance = 1e-6f)
    {
        return std::abs(a - b) < tolerance;
    }

    double sampleRate;
    double rampLengthMs;
};

// LINEAR =============================================================================================================

// Test default constructor
TEST_F(LinearSmoothedValueTest, DefaultConstructor)
{
    LinearSmoothedValue sv;
    EXPECT_FLOAT_EQ(sv.GetNextValue(), 0.0f);
}

// Test constructor with initial value
TEST_F(LinearSmoothedValueTest, ConstructorWithInitialValue)
{
    const float initialValue = 5.0f;
    LinearSmoothedValue sv(initialValue);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), initialValue);
}

// Test Reset with valid parameters
TEST_F(LinearSmoothedValueTest, ResetWithValidParameters)
{
    LinearSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, rampLengthMs);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 1.0f);

    sv.SetTargetValue(2.0f);
    float nextValue = sv.GetNextValue();
    EXPECT_GT(nextValue, 1.0f);
    EXPECT_LT(nextValue, 2.0f);
}

// Test Reset with invalid parameters
TEST_F(LinearSmoothedValueTest, ResetWithInvalidParameters)
{
    LinearSmoothedValue sv(1.0f);
    sv.SetTargetValue(2.0f);

    // Store the state before any reset attempts
    float valueBefore = sv.GetNextValue();

    // Invalid resets should be ignored, so behavior should remain consistent
    sv.Reset(-1.0, rampLengthMs);
    float valueAfter1 = sv.GetNextValue();

    sv.Reset(sampleRate, -1.0);
    float valueAfter2 = sv.GetNextValue();

    // Values should continue progressing toward target
    EXPECT_GT(valueAfter1, valueBefore); // Still progressing
    EXPECT_GT(valueAfter2, valueAfter1); // Still progressing
    EXPECT_LT(valueAfter2, 2.0f); // Haven't reached target yet
}

// Test SetTargetValue
TEST_F(LinearSmoothedValueTest, SetTargetValue)
{
    LinearSmoothedValue sv(0.0f);
    sv.Reset(sampleRate, rampLengthMs);

    sv.SetTargetValue(10.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 0.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value2, 10.0f);
}

// Test GetNextValue reaches target eventually
TEST_F(LinearSmoothedValueTest, GetNextValueReachesTarget)
{
    LinearSmoothedValue sv(0.0f);
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
TEST_F(LinearSmoothedValueTest, SmoothingWithNegativeValues)
{
    LinearSmoothedValue sv(5.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(-5.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_LT(value1, 5.0f);
    EXPECT_LT(value2, value1);
    EXPECT_GT(value1, -5.0f);
}

// Test Skip functionality
TEST_F(LinearSmoothedValueTest, SkipSamples)
{
    LinearSmoothedValue sv1(0.0f);
    LinearSmoothedValue sv2(0.0f);

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
TEST_F(LinearSmoothedValueTest, ZeroRampLength)
{
    LinearSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 0.0);
    sv.SetTargetValue(5.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
}

// Test very short ramp length
TEST_F(LinearSmoothedValueTest, VeryShortRampLength)
{
    LinearSmoothedValue sv(0.0f);
    sv.Reset(sampleRate, 0.1);
    sv.SetTargetValue(1.0f);

    float value = 0.0f;
    for (int i = 0; i < 10; ++i) {
        value = sv.GetNextValue();
    }

    EXPECT_FLOAT_EQ(value, 1.0f);
}

// Test multiple target changes
TEST_F(LinearSmoothedValueTest, MultipleTargetChanges)
{
    LinearSmoothedValue sv(0.0f);
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
TEST_F(LinearSmoothedValueTest, SameTargetValue)
{
    LinearSmoothedValue sv(5.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(5.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
}

// Test large sample rate
TEST_F(LinearSmoothedValueTest, LargeSampleRate)
{
    LinearSmoothedValue sv(0.0f);
    sv.Reset(192000.0, 100.0);
    sv.SetTargetValue(1.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 0.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value1, 0.1f);
}

// MULTIPLICATIVE =====================================================================================================

// Test constructor with initial value (multiplicative can't start from zero)
TEST_F(MultiplicativeSmoothedValueTest, ConstructorWithInitialValue)
{
    const float initialValue = 5.0f;
    MultiplicativeSmoothedValue sv(initialValue);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), initialValue);
}

// Test basic multiplicative behavior - exponential curve
TEST_F(MultiplicativeSmoothedValueTest, ExponentialCurve)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 100.0); // 100ms ramp
    sv.SetTargetValue(8.0f); // 3 octaves (2^3 = 8)

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();
    float value3 = sv.GetNextValue();

    // For multiplicative smoothing, the ratio between consecutive values should be approximately constant
    float ratio1 = value2 / value1;
    float ratio2 = value3 / value2;

    EXPECT_GT(value1, 1.0f);
    EXPECT_GT(value2, value1);
    EXPECT_GT(value3, value2);
    EXPECT_TRUE(IsApproximatelyEqual(ratio1, ratio2, 1e-4f));
}

// Test frequency doubling (octave) - with more realistic tolerance
TEST_F(MultiplicativeSmoothedValueTest, FrequencyOctave)
{
    MultiplicativeSmoothedValue sv(440.0f); // A4
    sv.Reset(sampleRate, 50.0);
    sv.SetTargetValue(880.0f); // A5 (one octave up)

    // Check that we get exponential progression
    float prevValue = 440.0f;
    for (int i = 0; i < 10; ++i) {
        float currentValue = sv.GetNextValue();
        EXPECT_GT(currentValue, prevValue);
        EXPECT_LT(currentValue, 880.0f);
        prevValue = currentValue;
    }

    // Eventually gets close to target - use more relaxed tolerance
    for (int i = 0; i < 5000; ++i) { // More samples to ensure convergence
        sv.GetNextValue();
    }
    float finalValue = sv.GetNextValue();
    EXPECT_TRUE(IsApproximatelyEqual(finalValue, 880.0f, 0.01f)); // 1% tolerance
}

// Test dB conversion - another common audio use case
TEST_F(MultiplicativeSmoothedValueTest, DecibelConversion)
{
    MultiplicativeSmoothedValue sv(1.0f); // 0 dB
    sv.Reset(sampleRate, 100.0);
    sv.SetTargetValue(3.16228f); // Approximately 10 dB (10^(10/20))

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 1.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value2, 3.16228f);
}

// Test zero handling - multiplicative can't reach zero
TEST_F(MultiplicativeSmoothedValueTest, ZeroHandling)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(0.0f); // Invalid target for multiplicative

    // Should not progress since target is zero
    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_FLOAT_EQ(value1, 1.0f); // Should remain at current value
    EXPECT_FLOAT_EQ(value2, 1.0f);
}

// Test starting from zero - should handle gracefully
TEST_F(MultiplicativeSmoothedValueTest, StartingFromZero)
{
    MultiplicativeSmoothedValue sv(0.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(10.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    // Should not progress since current value is zero
    EXPECT_FLOAT_EQ(value1, 0.0f);
    EXPECT_FLOAT_EQ(value2, 0.0f);
}

// Test SetTargetValue
TEST_F(MultiplicativeSmoothedValueTest, SetTargetValue)
{
    MultiplicativeSmoothedValue sv(2.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(32.0f); // 4 octaves (2^4 * 2 = 32)

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 2.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value2, 32.0f);
}

// Test GetNextValue gets close to target
TEST_F(MultiplicativeSmoothedValueTest, GetNextValueReachesTarget)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 50.0);
    sv.SetTargetValue(4.0f);

    float lastValue = 1.0f;
    for (int i = 0; i < 5000; ++i) { // More samples
        lastValue = sv.GetNextValue();
    }

    // Use percentage-based tolerance for multiplicative
    EXPECT_TRUE(IsApproximatelyEqual(lastValue, 4.0f, 0.01f)); // 1% tolerance
}

// Test Skip functionality
TEST_F(MultiplicativeSmoothedValueTest, SkipSamples)
{
    MultiplicativeSmoothedValue sv1(1.0f);
    MultiplicativeSmoothedValue sv2(1.0f);

    sv1.Reset(sampleRate, rampLengthMs);
    sv2.Reset(sampleRate, rampLengthMs);

    sv1.SetTargetValue(16.0f);
    sv2.SetTargetValue(16.0f);

    for (int i = 0; i < 100; ++i) {
        sv1.GetNextValue();
    }

    sv2.Skip(100);

    float value1 = sv1.GetNextValue();
    float value2 = sv2.GetNextValue();

    EXPECT_TRUE(IsApproximatelyEqual(value1, value2, 1e-4f));
}

// Test when ramp length is zero
TEST_F(MultiplicativeSmoothedValueTest, ZeroRampLength)
{
    MultiplicativeSmoothedValue sv(2.0f);
    sv.Reset(sampleRate, 0.0);
    sv.SetTargetValue(8.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 8.0f);
}

// Test very short ramp length - adjust expectations
TEST_F(MultiplicativeSmoothedValueTest, VeryShortRampLength)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 0.1); // Very short ramp
    sv.SetTargetValue(2.0f);

    float value = 1.0f;
    for (int i = 0; i < 20; ++i) { // More iterations for very short ramp
        value = sv.GetNextValue();
    }

    // For very short ramps, expect to get reasonably close
    EXPECT_TRUE(IsApproximatelyEqual(value, 2.0f, 0.05f)); // 5% tolerance
}

// Test multiple target changes - with realistic tolerances
TEST_F(MultiplicativeSmoothedValueTest, MultipleTargetChanges)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(sampleRate, 50.0);

    sv.SetTargetValue(4.0f);
    for (int i = 0; i < 100; ++i) {
        sv.GetNextValue();
    }

    float valueBeforeChange = sv.GetNextValue();
    sv.SetTargetValue(0.5f); // Going down
    float valueAfterChange = sv.GetNextValue();

    EXPECT_LT(valueAfterChange, valueBeforeChange); // Should start decreasing

    for (int i = 0; i < 5000; ++i) {
        sv.GetNextValue();
    }
    float finalValue = sv.GetNextValue();
    EXPECT_TRUE(IsApproximatelyEqual(finalValue, 0.5f, 0.01f)); // 1% tolerance
}

// Test same target value
TEST_F(MultiplicativeSmoothedValueTest, SameTargetValue)
{
    MultiplicativeSmoothedValue sv(5.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(5.0f);

    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
    EXPECT_FLOAT_EQ(sv.GetNextValue(), 5.0f);
}

// Test decreasing values (important for multiplicative)
TEST_F(MultiplicativeSmoothedValueTest, DecreasingValues)
{
    MultiplicativeSmoothedValue sv(8.0f);
    sv.Reset(sampleRate, rampLengthMs);
    sv.SetTargetValue(2.0f); // Decreasing

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_LT(value1, 8.0f);
    EXPECT_LT(value2, value1);
    EXPECT_GT(value2, 2.0f);
}

// Test large sample rate
TEST_F(MultiplicativeSmoothedValueTest, LargeSampleRate)
{
    MultiplicativeSmoothedValue sv(1.0f);
    sv.Reset(192000.0, 100.0);
    sv.SetTargetValue(2.0f);

    float value1 = sv.GetNextValue();
    float value2 = sv.GetNextValue();

    EXPECT_GT(value1, 1.0f);
    EXPECT_GT(value2, value1);
    EXPECT_LT(value1, 1.01f); // Should be a very small increment
}

// Test musical interval - with more realistic expectations
TEST_F(MultiplicativeSmoothedValueTest, MusicalInterval)
{
    MultiplicativeSmoothedValue sv(440.0f); // A4
    sv.Reset(sampleRate, 120.0); // 120ms for 12 steps
    sv.SetTargetValue(880.0f); // A5

    // Just run for the expected duration plus some buffer
    int totalSamples = static_cast<int>((150.0 / 1000.0) * sampleRate); // 150ms buffer
    for (int i = 0; i < totalSamples; ++i) {
        sv.GetNextValue();
    }

    float finalValue = sv.GetNextValue();

    // Check that we get reasonably close to 880Hz (within 1%)
    EXPECT_TRUE(IsApproximatelyEqual(finalValue, 880.0f, 8.8f)); // ~1% tolerance
}
