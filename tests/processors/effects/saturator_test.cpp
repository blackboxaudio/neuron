#include <gtest/gtest.h>

#include "saturator.h"

using namespace cortex;

TEST(saturator_suite, process_test)
{
    Saturator saturator;
    EXPECT_NEAR(saturator.Process(0.5f), 0.46211716f, 1e-5);
    EXPECT_NEAR(saturator.Process(-0.5f), -0.46211716f, 1e-5);
}

TEST(saturator_suite, set_saturation_test)
{
    Saturator saturator;
    saturator.SetSaturation(1.0f);
    EXPECT_NEAR(saturator.Process(0.5f), 0.46211716f, 1e-5);
    saturator.SetSaturation(12.0f);
    EXPECT_EQ(saturator.Process(0.5f), 1.0f);
}

TEST(saturator_suite, set_symmetry_test)
{
    Saturator saturator;

    saturator.SetSymmetry(0.0f); // Only positive signal components are affected
    float positiveResult = saturator.Process(0.9f);
    EXPECT_NEAR(positiveResult, 0.71629780f, 1e-5);
    float negativeResult = saturator.Process(-0.9f);
    EXPECT_EQ(negativeResult, -0.9f);

    saturator.SetSymmetry(1.0f); // Positive and negative signal components are affected
    positiveResult = saturator.Process(0.9f);
    EXPECT_NEAR(positiveResult, 0.71629780f, 1e-5);
    negativeResult = saturator.Process(-0.9f);
    EXPECT_NEAR(negativeResult, -0.71629780f, 1e-5);
}
