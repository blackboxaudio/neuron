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
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.1250467);
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
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729);

    osc.Reset();

    EXPECT_FLOAT_EQ(osc.Generate(), 0.0f);
    EXPECT_FLOAT_EQ(osc.Generate(), 0.062643729);
}
