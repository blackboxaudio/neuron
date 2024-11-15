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
