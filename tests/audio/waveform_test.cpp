#include <gtest/gtest.h>

#include "cortex.h"

using namespace cortex;

TEST(waveform_suite, sine_conversion_test)
{
    EXPECT_FLOAT_EQ(SineToWaveform(1.0f, Waveform::SINE), 1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(0.0f, Waveform::SINE), 0.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(-1.0f, Waveform::SINE), -1.0f);

    EXPECT_FLOAT_EQ(SineToWaveform(1.0f, Waveform::TRIANGLE), 1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(0.5f, Waveform::TRIANGLE), 0.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(0.0f, Waveform::TRIANGLE), -1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(-0.5f, Waveform::TRIANGLE), 0.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(-1.0f, Waveform::TRIANGLE), 1.0f);

    EXPECT_FLOAT_EQ(SineToWaveform(1.0f, Waveform::SAWTOOTH), 0.28318548f);
    EXPECT_FLOAT_EQ(SineToWaveform(0.5f, Waveform::SAWTOOTH), 0.14159274f);
    EXPECT_FLOAT_EQ(SineToWaveform(-0.5f, Waveform::SAWTOOTH), -0.14159274f);
    EXPECT_FLOAT_EQ(SineToWaveform(-1.0f, Waveform::SAWTOOTH), -0.28318548f);

    EXPECT_FLOAT_EQ(SineToWaveform(1.0f, Waveform::SQUARE), 1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(0.5f, Waveform::SQUARE), 1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(-0.5f, Waveform::SQUARE), -1.0f);
    EXPECT_FLOAT_EQ(SineToWaveform(-1.0f, Waveform::SQUARE), -1.0f);
}
