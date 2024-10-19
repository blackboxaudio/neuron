cmake_minimum_required(VERSION 3.14)

option(BUILD_TESTS "Build unit tests" OFF)

project(cortex VERSION 0.1.0 LANGUAGES CXX)

list(APPEND SOURCE
        src/generators/oscillator.cpp
        src/processors/filter.cpp
        src/processors/saturator.cpp
        src/processors/wavefolder.cpp
)

list(APPEND INCLUDE_DIR
        "src"
        "src/audio"
        "src/generators"
        "src/processors"
        "src/utilities"
)

add_library(cortex STATIC ${SOURCE})

set_target_properties(cortex PROPERTIES PUBLIC
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED
)

if(BUILD_TESTS)
    list(APPEND TEST
            tests/utilities/arithmetic_test.cpp
            tests/utilities/midi_test.cpp
    )
    add_subdirectory(vendor/googletest)
    add_executable(cortex_tests ${TEST})
    target_include_directories(cortex_tests PRIVATE ${INCLUDE_DIR})
    target_link_libraries(cortex_tests gtest gtest_main)
    enable_testing()
    add_test(NAME arithmetic_test COMMAND cortex_tests)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_include_directories(cortex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src PRIVATE ${INCLUDE_DIR})
