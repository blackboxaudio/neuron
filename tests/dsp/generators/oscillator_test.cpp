#include "neuron/dsp/generators/oscillator.h"

#include <gtest/gtest.h>
#include <array>

using namespace neuron;

TEST(oscillator_suite, generate_test)
{
    Context context { 44100, 2, 16 };
    Oscillator osc(context, 440.0f);

    std::array<Sample, 3> data {};
    Buffer<Sample> buffer(data.data(), data.size());
    osc.Generate(buffer);

    EXPECT_NEAR(buffer[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer[1], 0.0626f, 1e-3f);
    EXPECT_NEAR(buffer[2], 0.125f, 1e-3f);
}

TEST(oscillator_suite, reset_test)
{
    Context context { 44100, 2, 16 };
    Oscillator osc(context, 440.0f);

    std::array<Sample, 2> data1 {};
    Buffer<Sample> buffer1(data1.data(), data1.size());
    osc.Generate(buffer1);

    EXPECT_NEAR(buffer1[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer1[1], 0.0626f, 1e-3f);

    osc.Reset();

    std::array<Sample, 2> data2 {};
    Buffer<Sample> buffer2(data2.data(), data2.size());
    osc.Generate(buffer2);

    EXPECT_NEAR(buffer2[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer2[1], 0.0626f, 1e-3f);

    osc.Reset(0.5f);

    std::array<Sample, 2> data3 {};
    Buffer<Sample> buffer3(data3.data(), data3.size());
    osc.Generate(buffer3);

    EXPECT_NEAR(buffer3[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer3[1], -0.0626f, 1e-3f);
}

TEST(oscillator_suite, oscillator_sync)
{
    Context context { 44100, 2, 32 };
    Oscillator leader(context, 55.0f);
    Oscillator follower(context, 82.41f);

    leader.AttachFollower(&follower);

    std::array<Sample, 1> leaderData1 {}, followerData1 {};
    Buffer<Sample> leaderBuf1(leaderData1.data(), leaderData1.size());
    Buffer<Sample> followerBuf1(followerData1.data(), followerData1.size());

    leader.Generate(leaderBuf1);
    follower.Generate(followerBuf1);
    EXPECT_NEAR(leaderBuf1[0], followerBuf1[0], 1e-5f);

    std::array<Sample, 1> leaderData2 {}, followerData2 {};
    Buffer<Sample> leaderBuf2(leaderData2.data(), leaderData2.size());
    Buffer<Sample> followerBuf2(followerData2.data(), followerData2.size());

    leader.Generate(leaderBuf2);
    follower.Generate(followerBuf2);
    EXPECT_NEAR(leaderBuf2[0], 0.00783538f, 1e-3f);
    EXPECT_NEAR(followerBuf2[0], 0.01174026f, 1e-3f);

    int numSamples = context.sampleRate;
    std::array<Sample, 1> tempData {};
    Buffer<Sample> tempBuf(tempData.data(), tempData.size());
    while (numSamples--) {
        leader.Generate(tempBuf);
        follower.Generate(tempBuf);
    }

    std::array<Sample, 1> leaderData3 {}, followerData3 {};
    Buffer<Sample> leaderBuf3(leaderData3.data(), leaderData3.size());
    Buffer<Sample> followerBuf3(followerData3.data(), followerData3.size());

    leader.Generate(leaderBuf3);
    follower.Generate(followerBuf3);
    EXPECT_NE(leaderBuf3[0], followerBuf3[0]);

    leader.Reset();

    std::array<Sample, 1> leaderData4 {}, followerData4 {};
    Buffer<Sample> leaderBuf4(leaderData4.data(), leaderData4.size());
    Buffer<Sample> followerBuf4(followerData4.data(), followerData4.size());

    leader.Generate(leaderBuf4);
    follower.Generate(followerBuf4);
    EXPECT_NEAR(leaderBuf4[0], followerBuf4[0], 1e-5f);

    std::array<Sample, 1> leaderData5 {}, followerData5 {};
    Buffer<Sample> leaderBuf5(leaderData5.data(), leaderData5.size());
    Buffer<Sample> followerBuf5(followerData5.data(), followerData5.size());

    leader.Generate(leaderBuf5);
    follower.Generate(followerBuf5);
    EXPECT_NEAR(leaderBuf5[0], 0.00783538f, 1e-3f);
    EXPECT_NEAR(followerBuf5[0], 0.01174026f, 1e-3f);

    leader.DetachFollower();
    leader.Reset();

    std::array<Sample, 1> leaderData6 {}, followerData6 {};
    Buffer<Sample> leaderBuf6(leaderData6.data(), leaderData6.size());
    Buffer<Sample> followerBuf6(followerData6.data(), followerData6.size());

    leader.Generate(leaderBuf6);
    follower.Generate(followerBuf6);
    EXPECT_NE(leaderBuf6[0], followerBuf6[0]);
}

TEST(oscillator_suite, set_frequency_test)
{
    Context context { 44100, 2, 16 };
    Oscillator osc(context, 440.0f);

    std::array<Sample, 2> data1 {};
    Buffer<Sample> buffer1(data1.data(), data1.size());
    osc.Generate(buffer1);

    EXPECT_NEAR(buffer1[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer1[1], 0.0626f, 1e-3f);

    osc.Reset();
    osc.SetFrequency(220.0f);

    std::array<Sample, 2> data2 {};
    Buffer<Sample> buffer2(data2.data(), data2.size());
    osc.Generate(buffer2);

    EXPECT_NEAR(buffer2[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer2[1], 0.0313f, 1e-3f);
}

TEST(oscillator_suite, set_waveform_test)
{
    Context context { 44100, 2, 16 };
    Oscillator osc(context, 440.0f);

    std::array<Sample, 2> data1 {};
    Buffer<Sample> buffer1(data1.data(), data1.size());
    osc.Generate(buffer1);

    EXPECT_NEAR(buffer1[0], 0.0f, 1e-5f);
    EXPECT_NEAR(buffer1[1], 0.0626f, 1e-3f);

    osc.SetWaveform(Waveform::SQUARE);

    std::array<Sample, 2> data2 {};
    Buffer<Sample> buffer2(data2.data(), data2.size());
    osc.Generate(buffer2);

    EXPECT_FLOAT_EQ(buffer2[0], 1.0f);
    EXPECT_FLOAT_EQ(buffer2[1], 1.0f);
}
