#include <gtest/gtest.h>

#include "neuron.h"

using namespace neuron;

TEST(adsr_suite, trigger_test)
{
    AdsrEnvelopeModulator adsr;

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);

    adsr.Trigger();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00022675f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00045351f, 1e-5f);
}

TEST(adsr_suite, reset_test)
{
    AdsrEnvelopeModulator adsr;

    adsr.Trigger();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00022675f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00045351f, 1e-5f);

    adsr.Reset();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);

    adsr.Trigger();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00022675f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00045351f, 1e-5f);
}

TEST(adsr_suite, set_adsr_set)
{
    AdsrEnvelopeModulator adsr;

    adsr.SetAttackTime(0.1f);
    adsr.Trigger();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.22675738f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.45351475f, 1e-5f);

    adsr.SetAttackTime(0.05f);
    adsr.SetDecayTime(0.05f);
    adsr.SetSustainLevel(0.1f);
    adsr.SetReleaseTime(0.05f);
    adsr.Trigger();

    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.45351475f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.90702950f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 1.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.59183669f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.18367344f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.1f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.1f, 1e-5f);

    adsr.Release();

    EXPECT_NEAR(adsr.Modulate(), 0.1f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.05464852f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.00929705f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
    EXPECT_NEAR(adsr.Modulate(), 0.0f, 1e-5f);
}
