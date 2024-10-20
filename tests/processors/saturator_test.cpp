#include <gtest/gtest.h>

#include "saturator.h"

using namespace cortex;

TEST(saturator_suite, process_test)
{
    Saturator saturator;
    EXPECT_NEAR(saturator.Process(0.5f), 0.46581196784973145f, 1e-5);
    EXPECT_NEAR(saturator.Process(-0.5f), -0.46581196784973145f, 1e-5);
}

TEST(saturator_suite, symmetry_test)
{
    Saturator saturator;

    saturator.SetSymmetry(0.0f); // Only positive signal components are affected
    float positiveResult = saturator.Process(0.9f);
    EXPECT_NEAR(positiveResult, 0.72992122173309326f, 1e-5);
    float negativeResult = saturator.Process(-0.9f);
    EXPECT_NEAR(negativeResult, -0.9f, 1e-5);

    saturator.SetSymmetry(1.0f); // Positive and negative signal components are affected
    positiveResult = saturator.Process(0.9f);
    EXPECT_NEAR(positiveResult, 0.72992122173309326f, 1e-5);
    negativeResult = saturator.Process(-0.9f);
    EXPECT_NEAR(negativeResult, -0.72992122173309326f, 1e-5);
}

TEST(saturator_suite, set_saturation_test)
{
    Saturator saturator;
    EXPECT_NEAR(saturator.Process(0.5f), 0.46581196784973145f, 1e-5);
    saturator.SetSaturation(12.0f);
    EXPECT_NEAR(saturator.Process(0.5f), 1.076923131942749f, 1e-5);
}
