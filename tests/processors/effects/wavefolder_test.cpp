#include <gtest/gtest.h>

#include "wavefolder.h"

using namespace cortex;

TEST(wavefolder_suite, process_test)
{
    Wavefolder wavefolder;
    EXPECT_LT(wavefolder.Process(1.5f), 1.0f);
    EXPECT_GT(wavefolder.Process(-1.5f), -1.0f);
}

TEST(wavefolder_suite, set_input_gain_test)
{
    Wavefolder wavefolder;
    float preGainAdjustment = wavefolder.Process(1.0f);
    wavefolder.SetInputGain(2.0f);
    EXPECT_EQ(preGainAdjustment, wavefolder.Process(0.5f));
}

TEST(wavefolder_suite, set_symmetry_test)
{
    Wavefolder wavefolder;

    wavefolder.SetSymmetry(0.0f); // Only positive signal components are affected
    float positiveResult = wavefolder.Process(0.7f);
    EXPECT_NEAR(positiveResult, 0.69999998f, 1e-3);
    float negativeResult = wavefolder.Process(-0.7f);
    EXPECT_EQ(negativeResult, -0.7f);

    wavefolder.SetSymmetry(1.0f); // Positive and negative signal components are affected
    positiveResult = wavefolder.Process(0.7f);
    EXPECT_NEAR(positiveResult, 0.69999998f, 1e-5);
    negativeResult = wavefolder.Process(-0.7f);
    EXPECT_NEAR(negativeResult, -0.69999998f, 1e-5);
}

TEST(wavefolder_suite, set_threshold_test)
{
    Wavefolder wavefolder;
    wavefolder.SetThreshold(0.5f);
    EXPECT_LT(wavefolder.Process(2.4f), 0.5f);
}
