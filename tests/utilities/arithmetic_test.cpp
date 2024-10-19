#include <gtest/gtest.h>

#include "arithmetic.h"

using namespace cortex;

TEST(arithmetic_test, clamp_test)
{
    EXPECT_EQ(0.0f, clamp<float>(0.0f, -1.0f, 1.0f));
    EXPECT_EQ(1.0f, clamp<float>(2.0f, -1.0f, 1.0f));
    EXPECT_EQ(-1.0f, clamp<float>(-2.0f, -1.0f, 1.0f));
}
