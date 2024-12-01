#include <gtest/gtest.h>

#include "neuron.h"

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

#ifdef NEO_USE_STD_ATOMIC
#include <atomic>

TEST(parameter_suite, attach_source_test)
{
    std::atomic<float> externalSource(2.0f);
    Parameter<float> param(1.0f);

    param.AttachSource(&externalSource);

    EXPECT_FLOAT_EQ(static_cast<float>(param), 2.0f);

    externalSource.store(3.5f);
    EXPECT_FLOAT_EQ(static_cast<float>(param), 3.5f);
}

TEST(parameter_suite, attach_and_assignment_test)
{
    Parameter<float> param(5.0f);

    param = 3.0f;

    EXPECT_FLOAT_EQ(static_cast<float>(param), 3.0f);
}

TEST(parameter_suite, attach_source_and_arithmetic_test)
{
    std::atomic<float> externalSource(10.0f);
    Parameter<float> param(5.0f);

    param.AttachSource(&externalSource);

    EXPECT_FLOAT_EQ(param + 5.0f, 15.0f);
    EXPECT_FLOAT_EQ(param - 2.0f, 8.0f);
    EXPECT_FLOAT_EQ(param * 2.0f, 20.0f);
    EXPECT_FLOAT_EQ(param / 2.0f, 5.0f);
}

TEST(parameter_suite, destructor_test)
{
    std::atomic<float>* externalSource = new std::atomic<float>(10.0f);
    Parameter<float>* param = new Parameter<float>(externalSource);

    delete param;

    EXPECT_FLOAT_EQ(externalSource->load(), 10.0f);

    delete externalSource;
}
#endif
