cmake_minimum_required(VERSION 3.14)

option(BUILD_TESTS "Build unit tests" OFF)

project(cortex VERSION 0.1.0 LANGUAGES CXX)

list(APPEND SOURCE
        src/generators/oscillator.cpp
        src/modulators/adsr.cpp
        src/processors/effects/saturator.cpp
        src/processors/effects/wavefolder.cpp
        src/processors/filters/filter.cpp
)

list(APPEND INCLUDE_DIR
        "src"
        "src/audio"
        "src/generators"
        "src/modulators"
        "src/processors/effects"
        "src/processors/filters"
        "src/utilities"
)

add_library(cortex STATIC ${SOURCE})

set_target_properties(cortex PROPERTIES PUBLIC
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED
)

if(BUILD_TESTS)
    function(add_cortex_test name src)
        add_executable("${name}_test_exe" ${src})
        target_include_directories("${name}_test_exe" PRIVATE ${INCLUDE_DIR})
        target_link_libraries("${name}_test_exe" cortex)
        target_link_libraries("${name}_test_exe" gtest gtest_main)
        add_test(NAME "${name}_test" COMMAND "${name}_test_exe")
    endfunction()

    add_subdirectory(vendor/googletest)

    add_cortex_test(waveform tests/audio/waveform_test.cpp)
    add_cortex_test(oscillator tests/generators/oscillator_test.cpp)
    add_cortex_test(adsr tests/modulators/adsr_test.cpp)
    add_cortex_test(saturator tests/processors/effects/saturator_test.cpp)
    add_cortex_test(wavefolder tests/processors/effects/wavefolder_test.cpp)
    add_cortex_test(filter tests/processors/filters/filter_test.cpp)
    add_cortex_test(arithmetic tests/utilities/arithmetic_test.cpp)
    add_cortex_test(midi tests/utilities/midi_test.cpp)

    enable_testing()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_include_directories(cortex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src PRIVATE ${INCLUDE_DIR})
