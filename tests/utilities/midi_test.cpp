#include <gtest/gtest.h>

#include "midi.h"

using namespace cortex;

TEST(midi_suite, midi_to_frequency_test)
{
    EXPECT_EQ(8.17579842f, midi_to_frequency<float>(0)); // C-1
    EXPECT_EQ(27.5f, midi_to_frequency<float>(21)); // A0
    EXPECT_EQ(261.625549f, midi_to_frequency<float>(60)); // C4
    EXPECT_EQ(440.0f, midi_to_frequency<float>(69)); // A4
    EXPECT_EQ(4186.00879f, midi_to_frequency<float>(108)); // C8
    EXPECT_EQ(12543.855f, midi_to_frequency<float>(127.0f)); // G9
}
