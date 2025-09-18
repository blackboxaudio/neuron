#include <gtest/gtest.h>

#include "neuron.h"

using namespace neuron;

TEST(arithmetic_suite, clamp_test)
{
    EXPECT_EQ(0.0f, clamp<float>(0.0f, -1.0f, 1.0f));
    EXPECT_EQ(1.0f, clamp<float>(2.0f, -1.0f, 1.0f));
    EXPECT_EQ(-1.0f, clamp<float>(-2.0f, -1.0f, 1.0f));

    EXPECT_EQ(1, clamp<int>(1, -1, 1));
    EXPECT_EQ(-1, clamp<int>(-1, -1, 1));

    EXPECT_EQ(1, clamp<double>(1, 1, 1));
    EXPECT_EQ(1, clamp<double>(0, 1, -1));
}

TEST(arithmetic_suite, map_test)
{
    EXPECT_EQ(5.5f, map<float>(0.5f, 1.0f, 10.0f));

    EXPECT_EQ(0.25f, map<float>(0.5f, 0.0f, 1.0f, Mapping::EXP));
    EXPECT_EQ(3.1622777f, map<float>(0.5f, 1.0f, 10.0f, Mapping::LOG));
    EXPECT_EQ(0.5f, map<float>(0.5f, 0.0f, 1.0f, Mapping::LINEAR));

    EXPECT_EQ(1, map<int>(0, 1, 100, Mapping::LINEAR));
    EXPECT_EQ(100, map<int>(1, 1, 100, Mapping::LINEAR));

    EXPECT_EQ(1.0f, map(0.0f, 1.0f, 1.0f));

    EXPECT_EQ(0.0f, map(0.5f, 0.0f, 10.0f, Mapping::LOG));
}

TEST(arithmetic_suite, tanh_test)
{
    EXPECT_NEAR(tanh(0.0f), 0.0f, 1e-5f);

    EXPECT_NEAR(tanh(1.0f), std::tanh(1.0f), 1e-1f);
    EXPECT_NEAR(tanh(-1.0f), std::tanh(-1.0f), 1e-1f);

    EXPECT_NEAR(tanh(0.5f), std::tanh(0.5f), 1e-1f);
    EXPECT_NEAR(tanh(-0.5f), std::tanh(-0.5f), 1e-1f);

    EXPECT_NEAR(tanh(0.001f), std::tanh(0.001f), 1e-5f);
}

TEST(arithmetic_suite, exp_test)
{
    EXPECT_NEAR(exp(0.5f), std::exp(0.5f), 1e-5f);
    EXPECT_NEAR(exp(-0.5f), std::exp(-0.5f), 1e-5f);

    EXPECT_NEAR(exp(0.0f), 1.0f, 1e-5f);

    EXPECT_NEAR(exp(4.0f), std::exp(4.0f), 1.0f);
    EXPECT_NEAR(exp(-6.0f), std::exp(-6.0f), 1e-2f);

    EXPECT_NEAR(exp(1e-6), std::exp(1e-6f), 1e-5f);
}

TEST(arithmetic_suite, isApproximatelyEqual_test)
{
    // Test identical values
    EXPECT_TRUE(isApproximatelyEqual(0.0f, 0.0f));
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f));
    EXPECT_TRUE(isApproximatelyEqual(-1.0f, -1.0f));

    // Test with double precision
    EXPECT_TRUE(isApproximatelyEqual(0.0, 0.0));
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0));
    EXPECT_TRUE(isApproximatelyEqual(-1.0, -1.0));

    // Test integer types (exact equality)
    EXPECT_TRUE(isApproximatelyEqual(0, 0));
    EXPECT_TRUE(isApproximatelyEqual(5, 5));
    EXPECT_TRUE(isApproximatelyEqual(-3, -3));

    // Test values well within default relative epsilon for float (1e-5f)
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f + 1e-6f));   // Well within 1e-5f * 1.0f
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f - 1e-6f));
    EXPECT_TRUE(isApproximatelyEqual(10.0f, 10.0f + 5e-5f));  // Well within 1e-5f * 10.0f = 1e-4f
    EXPECT_TRUE(isApproximatelyEqual(-1.0f, -1.0f + 1e-6f));

    // Test values well within default relative epsilon for double (1e-9)
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0 + 1e-10));     // Well within 1e-9 * 1.0
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0 - 1e-10));
    EXPECT_TRUE(isApproximatelyEqual(100.0, 100.0 + 5e-8));  // Well within 1e-9 * 100.0 = 1e-7

    // Test values outside default relative epsilon for float
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f + 2e-5f));   // Exceeds 1e-5f * 1.0f
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f - 2e-5f));
    EXPECT_FALSE(isApproximatelyEqual(10.0f, 10.0f + 2e-4f)); // Exceeds 1e-5f * 10.0f

    // Test values outside default relative epsilon for double
    EXPECT_FALSE(isApproximatelyEqual(1.0, 1.0 + 2e-9));      // Exceeds 1e-9 * 1.0
    EXPECT_FALSE(isApproximatelyEqual(1.0, 1.0 - 2e-9));
    EXPECT_FALSE(isApproximatelyEqual(100.0, 100.0 + 2e-7)); // Exceeds 1e-9 * 100.0

    // Test clearly different values
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 2.0f));
    EXPECT_FALSE(isApproximatelyEqual(-1.0f, 1.0f));
    EXPECT_FALSE(isApproximatelyEqual(0.0f, 1.0f));
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 0.0f));

    EXPECT_FALSE(isApproximatelyEqual(1.0, 2.0));
    EXPECT_FALSE(isApproximatelyEqual(-1.0, 1.0));
    EXPECT_FALSE(isApproximatelyEqual(0.0, 1.0));

    // Test integer differences
    EXPECT_FALSE(isApproximatelyEqual(1, 2));
    EXPECT_FALSE(isApproximatelyEqual(-1, 1));
    EXPECT_FALSE(isApproximatelyEqual(0, 1));

    // Test with custom epsilon values
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f + 1e-7f, 1e-6f));   // Custom epsilon
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f + 1e-5f, 1e-6f));  // Exceeds custom epsilon

    // Test edge cases near zero - demonstrate relative epsilon limitations
    EXPECT_TRUE(isApproximatelyEqual(0.0f, 0.0f));
    EXPECT_TRUE(isApproximatelyEqual(-0.0f, 0.0f));

    // Demonstrate that relative epsilon correctly fails for near-zero comparisons
    float tiny_val = 1e-10f;
    EXPECT_TRUE(isApproximatelyEqual(tiny_val, tiny_val));
    EXPECT_FALSE(isApproximatelyEqual(0.0f, tiny_val, 1e-8f)); // Correctly fails - this is expected behavior!

    // For absolute comparisons near zero, use direct comparison:
    EXPECT_TRUE(std::abs(0.0f - tiny_val) <= 1e-8f); // Direct absolute comparison works

    // Test with small values where relative epsilon works well
    float small_val = 1e-3f;  // 0.001f
    EXPECT_TRUE(isApproximatelyEqual(small_val, small_val));
    EXPECT_TRUE(isApproximatelyEqual(small_val, small_val + 5e-9f));  // Well within 1e-5f * 1e-3f = 1e-8f
    EXPECT_FALSE(isApproximatelyEqual(small_val, small_val + 2e-8f)); // Exceeds 1e-8f

    // Test with large values where relative epsilon scales appropriately
    float large_val = 1e6f;  // 1,000,000
    EXPECT_TRUE(isApproximatelyEqual(large_val, large_val));
    EXPECT_TRUE(isApproximatelyEqual(large_val, large_val + 5.0f));   // Well within 1e-5f * 1e6f = 10.0f
    EXPECT_FALSE(isApproximatelyEqual(large_val, large_val + 20.0f)); // Exceeds 10.0f threshold

    // Test audio-relevant ranges
    // Frequency range: 20Hz - 20kHz
    EXPECT_TRUE(isApproximatelyEqual(440.0f, 440.0f + 0.002f));   // Well within 1e-5f * 440 ≈ 0.0044f
    EXPECT_FALSE(isApproximatelyEqual(440.0f, 440.0f + 0.01f));   // Exceeds threshold

    // Gain range: 0.0 - 1.0
    EXPECT_TRUE(isApproximatelyEqual(0.5f, 0.5f + 1e-6f));       // Well within 1e-5f * 0.5 = 5e-6f
    EXPECT_FALSE(isApproximatelyEqual(0.5f, 0.5f + 1e-5f));      // Exceeds 5e-6f

    // Test sign differences
    EXPECT_FALSE(isApproximatelyEqual(1.0f, -1.0f));
    EXPECT_FALSE(isApproximatelyEqual(-0.5f, 0.5f));

    // Test boundary behavior around typical audio values
    // MIDI note velocity (0-127)
    EXPECT_TRUE(isApproximatelyEqual(64.0f, 64.0f + 0.0005f));   // Well within tolerance
    EXPECT_FALSE(isApproximatelyEqual(64.0f, 64.0f + 0.001f));   // Outside tolerance

    // Sample values (-1.0 to 1.0 range)
    EXPECT_TRUE(isApproximatelyEqual(0.8f, 0.8f + 5e-6f));      // Well within tolerance
    EXPECT_FALSE(isApproximatelyEqual(0.8f, 0.8f + 1e-5f));     // At/outside tolerance
}
