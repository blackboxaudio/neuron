#include <gtest/gtest.h>
#include "arithmetic.h"

using namespace cortex;

TEST(arithmetic_test, BasicTest) {
    EXPECT_EQ(1.0f, clamp(2.0f, -1.0f, 1.0f));
}
