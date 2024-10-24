#include <gtest/gtest.h>

#include "cortex.h"

using namespace cortex;

TEST(filter_suite, basic_test)
{
    Filter filter;
    Oscillator oscillator;

    oscillator.SetFrequency(12000.0f);
    filter.SetCutoffFrequency(100.0f);

    int numSamples = 32;
    while (numSamples--) {
        float result = filter.Process(oscillator.Generate());
        EXPECT_NEAR(result, 0.0f, 1e-1);
    }
}
