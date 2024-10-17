cmake_minimum_required(VERSION 3.14)

project(CORTEX VERSION 0.1.0 LANGUAGES CXX)

add_library(cortex STATIC
        src/generators/oscillator.cpp
        src/processors/saturator.cpp
)

set_target_properties(cortex PROPERTIES PUBLIC
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_include_directories(cortex PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src
        PRIVATE
        "src"
        "src/generators"
        "src/processors"
        "src/utilities"
)
