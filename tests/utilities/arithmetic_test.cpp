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

    // Test integer types
    EXPECT_TRUE(isApproximatelyEqual(0, 0));
    EXPECT_TRUE(isApproximatelyEqual(5, 5));
    EXPECT_TRUE(isApproximatelyEqual(-3, -3));

    // Test values within epsilon for float
    float epsilon_f = std::numeric_limits<float>::epsilon();
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f + epsilon_f));
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f - epsilon_f));
    EXPECT_TRUE(isApproximatelyEqual(0.0f, epsilon_f));
    EXPECT_TRUE(isApproximatelyEqual(0.0f, -epsilon_f));

    // Test values within epsilon for double
    double epsilon_d = std::numeric_limits<double>::epsilon();
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0 + epsilon_d));
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0 - epsilon_d));
    EXPECT_TRUE(isApproximatelyEqual(0.0, epsilon_d));
    EXPECT_TRUE(isApproximatelyEqual(0.0, -epsilon_d));

    // Test values exactly at epsilon boundary
    EXPECT_TRUE(isApproximatelyEqual(1.0f, 1.0f + epsilon_f));
    EXPECT_TRUE(isApproximatelyEqual(1.0, 1.0 + epsilon_d));

    // Test values outside epsilon for float - use larger multiples
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f + epsilon_f * 10.0f));
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f - epsilon_f * 10.0f));
    EXPECT_FALSE(isApproximatelyEqual(0.0f, epsilon_f * 10.0f));

    // Test values outside epsilon for double - use larger multiples
    EXPECT_FALSE(isApproximatelyEqual(1.0, 1.0 + epsilon_d * 10.0));
    EXPECT_FALSE(isApproximatelyEqual(1.0, 1.0 - epsilon_d * 10.0));
    EXPECT_FALSE(isApproximatelyEqual(0.0, epsilon_d * 10.0));

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

    // Test negative values
    EXPECT_TRUE(isApproximatelyEqual(-1.0f, -1.0f + epsilon_f));
    EXPECT_TRUE(isApproximatelyEqual(-1.0f, -1.0f - epsilon_f));
    EXPECT_FALSE(isApproximatelyEqual(-1.0f, -1.0f + epsilon_f * 10.0f));

    // Test with small positive values
    float small_val = 1e-6f;
    EXPECT_TRUE(isApproximatelyEqual(small_val, small_val));
    EXPECT_TRUE(isApproximatelyEqual(small_val, small_val + epsilon_f));
    EXPECT_FALSE(isApproximatelyEqual(small_val, small_val + epsilon_f * 100.0f));

    // Test with large values
    float large_val = 1e6f;
    EXPECT_TRUE(isApproximatelyEqual(large_val, large_val));
    EXPECT_TRUE(isApproximatelyEqual(large_val, large_val + epsilon_f));
    EXPECT_FALSE(isApproximatelyEqual(large_val, large_val + 1.0f));

    // Test special cases with zero
    EXPECT_TRUE(isApproximatelyEqual(0.0f, 0.0f));
    EXPECT_TRUE(isApproximatelyEqual(-0.0f, 0.0f));
    EXPECT_TRUE(isApproximatelyEqual(0.0f, epsilon_f));

    // Test values that should definitely be outside epsilon
    EXPECT_FALSE(isApproximatelyEqual(1.0f, 1.0f + 1e-6f)); // Much larger than epsilon
    EXPECT_FALSE(isApproximatelyEqual(1.0, 1.0 + 1e-15)); // Much larger than double epsilon

    // Test with nextafter to get the actual next representable value
    float next_float = std::nextafter(1.0f, 2.0f);
    EXPECT_TRUE(isApproximatelyEqual(1.0f, next_float)); // Should be exactly epsilon apart

    float next_next_float = std::nextafter(next_float, 2.0f);
    EXPECT_FALSE(isApproximatelyEqual(1.0f, next_next_float)); // Should be > epsilon apart
}
