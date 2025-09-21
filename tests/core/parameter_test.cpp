#include "neuron/core/parameter.h"

#include <gtest/gtest.h>

using namespace neuron;

TEST(parameter_suite, initialization_test)
{
    Parameter<float> param(1.0f);

    EXPECT_FLOAT_EQ(static_cast<float>(param), 1.0f);
}

TEST(parameter_suite, arithmetic_operations_test)
{
    Parameter<float> param(5.0f);

    EXPECT_FLOAT_EQ(param + 3.0f, 8.0f);

    EXPECT_FLOAT_EQ(param - 2.0f, 3.0f);

    EXPECT_FLOAT_EQ(param * 2.0f, 10.0f);

    EXPECT_FLOAT_EQ(param / 2.0f, 2.5f);
}

TEST(parameter_suite, division_by_zero_test)
{
    Parameter<float> param(5.0f);

    EXPECT_FLOAT_EQ(param / 0.0f, 0.0f);
}

TEST(parameter_suite, operator_assignment_test)
{
    Parameter<float> param(5.0f);

    param = 3.0f;

    EXPECT_FLOAT_EQ(static_cast<float>(param), 3.0f);
}
