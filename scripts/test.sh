#!/bin/bash

# CAUTION: Assumes this script is run from the root repository directory
TARGET_DIR=$PWD/target/test

BUILD_TESTS=false
OUTPUT_FAILURE=false

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
    esac
done

if [ $BUILD_TESTS == "true" ]; then
    rm -rf "$TARGET_DIR"
    mkdir -p "$TARGET_DIR"
    cd "$TARGET_DIR" || exit 1
    cmake -DBUILD_TESTS=ON ../../
    make
else
    cd "$TARGET_DIR" || exit 1
fi

CTEST_FLAGS=$([ $OUTPUT_FAILURE == "true" ] && echo "--rerun-failed --output-on-failure")
ctest $CTEST_FLAGS
