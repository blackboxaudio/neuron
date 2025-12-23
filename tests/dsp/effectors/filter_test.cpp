#include "neuron/dsp/generators/oscillator.h"
#include "neuron/dsp/effectors/filter.h"

#include <gtest/gtest.h>
#include <array>

using namespace neuron;

TEST(filter_suite, basic_test)
{
    Context context { 44100, 2, 32 };
    Filter filter(context, 100.0f);
    Oscillator oscillator(context, 12000.0f);

    std::array<Sample, 32> oscData {}, filterData {};
    Buffer<Sample> oscBuffer(oscData.data(), oscData.size());
    Buffer<Sample> filterBuffer(filterData.data(), filterData.size());

    oscillator.Generate(oscBuffer);
    filter.Effect(oscBuffer, filterBuffer);

    for (int i = 0; i < 32; i++) {
        EXPECT_NEAR(filterBuffer[i], 0.0f, 1e-1);
    }
}
