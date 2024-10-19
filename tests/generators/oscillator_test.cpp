#include <gtest/gtest.h>

#include "oscillator.h"

using namespace cortex;

TEST(oscillator_tests, generate_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.1250467f);
}

TEST(oscillator_tests, reset_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729f);

    osc.Reset();

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729f);
}

TEST(oscillator_tests, set_frequency_test)
{
    Context context {
        44100,
        2,
        16
    };
    Oscillator osc(context, 440.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729f);

    osc.Reset();
    osc.SetFrequency(220.0f);

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.031337444f);
}
