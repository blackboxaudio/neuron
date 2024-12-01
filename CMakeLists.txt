cmake_minimum_required(VERSION 3.14)

option(NEO_USE_STD_ATOMIC "Enable atomic variables" OFF)
option(NEO_BUILD_TESTS "Build unit tests" OFF)

project(neuron VERSION 0.1.0 LANGUAGES CXX)

file(GLOB_RECURSE LIST_DIRECTORIES INCLUDE_DIRS "src/*")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(neuron STATIC ${SRC_FILES})

set_target_properties(neuron PROPERTIES PUBLIC
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED
)

if(NEO_USE_STD_ATOMIC)
    target_compile_definitions(neuron PUBLIC NEO_USE_STD_ATOMIC)
endif()

if(NEO_BUILD_TESTS)
    function(add_neuron_test name src)
        add_executable("${name}_test_exe" ${src})
        target_include_directories("${name}_test_exe" PRIVATE ${INCLUDE_DIRS})
        target_link_libraries("${name}_test_exe" neuron)
        target_link_libraries("${name}_test_exe" gtest gtest_main)
        add_test(NAME "${name}_test" COMMAND "${name}_test_exe")
    endfunction()

    add_subdirectory(vendor/googletest)

    if(NEO_USE_STD_ATOMIC)
        add_neuron_test(parameter_atomic tests/abstractions/parameter_test_atomic.cpp)
    else()
        add_neuron_test(parameter tests/abstractions/parameter_test.cpp)
        add_neuron_test(waveform tests/audio/waveform_test.cpp)
        add_neuron_test(oscillator tests/generators/oscillator_test.cpp)
        add_neuron_test(adsr tests/modulators/adsr_test.cpp)
        add_neuron_test(saturator tests/processors/effects/saturator_test.cpp)
        add_neuron_test(wavefolder tests/processors/effects/wavefolder_test.cpp)
        add_neuron_test(filter tests/processors/filters/filter_test.cpp)
        add_neuron_test(arithmetic tests/utilities/arithmetic_test.cpp)
        add_neuron_test(midi tests/utilities/midi_test.cpp)
    endif()

    enable_testing()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_include_directories(neuron PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src PRIVATE ${INCLUDE_DIRS})
