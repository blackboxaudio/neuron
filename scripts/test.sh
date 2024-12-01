#!/bin/bash

# CAUTION: Assumes this script is run from the root repository directory
TARGET_DIR=$PWD/target/test

BUILD_TESTS=false
OUTPUT_FAILURE=false
PLUGIN_SUPPORT=false

for i in "$@"; do
    case $i in
        -b|--build)
            BUILD_TESTS=true
            shift
            ;;
        -o|--output-failure)
            OUTPUT_FAILURE=true
            shift
            ;;
        -p|--plugin-support)
            PLUGIN_SUPPORT=true
            shift
            ;;
    esac
done

CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Release -DNEO_BUILD_TESTS=ON"
if [ "$PLUGIN_SUPPORT" == "true" ]; then
    CMAKE_FLAGS+=" -DNEO_USE_STD_ATOMIC=ON"
fi

if [ $BUILD_TESTS == "true" ]; then
    rm -rf "$TARGET_DIR"
    mkdir -p "$TARGET_DIR"
    cd "$TARGET_DIR" || exit 1
    cmake $CMAKE_FLAGS ../../
    make
else
    cd "$TARGET_DIR" || exit 1
fi

CTEST_CMD="ctest"
if [ "$OUTPUT_FAILURE" == "true" ]; then
    CTEST_CMD="$CTEST_CMD --rerun-failed --output-on-failure"
fi

eval "$CTEST_CMD"
