#include <gtest/gtest.h>

#include <atomic>

#include "neuron.h"

// Tests for Parameter class
TEST(parameter_suite, initialization_test) {
    Parameter<float> param(1.0f);

    // Test if the parameter initializes correctly
    EXPECT_FLOAT_EQ(static_cast<float>(param), 1.0f);
}

TEST(parameter_suite, attach_source_test) {
    std::atomic<float> externalSource(2.0f);
    Parameter<float> param(1.0f);

    // Attach the external source
    param.AttachSource(&externalSource);

    // Test if the parameter now reads from the external source
    EXPECT_FLOAT_EQ(static_cast<float>(param), 2.0f);

    // Modify the external source and ensure the parameter reflects the change
    externalSource.store(3.5f);
    EXPECT_FLOAT_EQ(static_cast<float>(param), 3.5f);
}

TEST(parameter_suite, arithmetic_operations_test) {
    Parameter<float> param(5.0f);

    // Test addition
    EXPECT_FLOAT_EQ(param + 3.0f, 8.0f);

    // Test subtraction
    EXPECT_FLOAT_EQ(param - 2.0f, 3.0f);

    // Test multiplication
    EXPECT_FLOAT_EQ(param * 2.0f, 10.0f);

    // Test division
    EXPECT_FLOAT_EQ(param / 2.0f, 2.5f);
}

TEST(parameter_suite, division_by_zero_test) {
    Parameter<float> param(5.0f);

    // Test division by zero
    EXPECT_FLOAT_EQ(param / 0.0f, 0.0f); // As per the class behavior
}

TEST(parameter_suite, attach_source_and_arithmetic_test) {
    std::atomic<float> externalSource(10.0f);
    Parameter<float> param(5.0f);

    // Attach external source
    param.AttachSource(&externalSource);

    // Test arithmetic operations after attaching a source
    EXPECT_FLOAT_EQ(param + 5.0f, 15.0f);
    EXPECT_FLOAT_EQ(param - 2.0f, 8.0f);
    EXPECT_FLOAT_EQ(param * 2.0f, 20.0f);
    EXPECT_FLOAT_EQ(param / 2.0f, 5.0f);
}

TEST(parameter_suite, operator_assignment_test) {
    Parameter<float> param(5.0f);

    // Assign a value to the parameter
    param = 3.0f;

    // Test that the assignment operator does not modify the atomic source
    EXPECT_FLOAT_EQ(static_cast<float>(param), 5.0f);
}

TEST(parameter_suite, destructor_test) {
    std::atomic<float>* externalSource = new std::atomic<float>(10.0f);
    Parameter<float>* param = new Parameter<float>(externalSource);

    // Test that the destructor sets the parameter pointer to null
    delete param;

    // Ensure the external source is unaffected
    EXPECT_FLOAT_EQ(externalSource->load(), 10.0f);

    // Cleanup
    delete externalSource;
}