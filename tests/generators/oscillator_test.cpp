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
