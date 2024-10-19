cmake_minimum_required(VERSION 3.14)

option(BUILD_TESTS "Build unit tests" OFF)

project(cortex VERSION 0.1.0 LANGUAGES CXX)

add_library(cortex STATIC
        src/generators/oscillator.cpp
        src/processors/filter.cpp
        src/processors/saturator.cpp
        src/processors/wavefolder.cpp
)

set_target_properties(cortex PROPERTIES PUBLIC
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED
)

if(BUILD_TESTS)
    add_subdirectory(vendor/googletest)
    add_executable(cortex_tests
            tests/utilities/arithmetic_test.cpp
            tests/utilities/midi_test.cpp
    )
    target_include_directories(cortex_tests PRIVATE
            "src"
            "src/audio"
            "src/generators"
            "src/processors"
            "src/utilities"
    )
    target_link_libraries(cortex_tests gtest gtest_main)
    enable_testing()
    add_test(NAME arithmetic_test COMMAND cortex_tests)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_include_directories(cortex PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src
        PRIVATE
        "src"
        "src/audio"
        "src/generators"
        "src/processors"
        "src/utilities"
)
