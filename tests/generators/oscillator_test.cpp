#include <gtest/gtest.h>

#include "oscillator.h"

using namespace cortex;

TEST(oscillator_suite, generate_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.06264372f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.1250467f);
}

TEST(oscillator_suite, reset_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.06264372f);

    osc.Reset();
    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.06264372f);

    osc.Reset((float)WAVETABLE_SIZE / 2.0f);
    EXPECT_NEAR(osc.Generate(), 0.0f, 1e-5f);
    EXPECT_NEAR(osc.Generate(), -0.06264372f, 1e-5f);
}

TEST(oscillator_suite, oscillator_sync)
{
    Context context {
        44100,
        2,
        32
    };
    Oscillator leader(context, 55.0f);
    Oscillator follower(context, 82.41f);

    leader.AttachFollower(&follower);

    EXPECT_NEAR(leader.Generate(), follower.Generate(), 1e-5f);
    EXPECT_NEAR(leader.Generate(), 0.00783538f, 1e-5f);
    EXPECT_NEAR(follower.Generate(), 0.01174026f, 1e-5f);

    int numSamples = context.sampleRate;
    while (numSamples--) {
        leader.Generate();
        follower.Generate();
    }

    EXPECT_NE(leader.Generate(), follower.Generate());

    leader.Reset();

    EXPECT_NEAR(leader.Generate(), follower.Generate(), 1e-5f);
    EXPECT_NEAR(leader.Generate(), 0.00783538f, 1e-5f);
    EXPECT_NEAR(follower.Generate(), 0.01174026f, 1e-5f);

    leader.DetachFollower();
    leader.Reset();

    EXPECT_NE(leader.Generate(), follower.Generate());
}

TEST(oscillator_suite, set_frequency_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.06264372f);

    osc.Reset();
    osc.SetFrequency(220.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.03133744f);
}

TEST(oscillator_suite, set_waveform_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.06264372f);

    osc.SetWaveform(Waveform::SQUARE);

    EXPECT_FLOAT_EQ(osc.Generate(), 1.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 1.0f);
}
