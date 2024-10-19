#!/bin/bash

# CAUTION: Assumes this script is run from the root repository directory
TARGET_DIR=$PWD/target/test

BUILD_TESTS=false

for i in "$@"; do
    case $i in
        -b|--build)
            BUILD_TESTS=true
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

ctest
