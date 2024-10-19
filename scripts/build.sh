#!/bin/bash

CONFIG=${1:-debug}
if [ $CONFIG != "debug" ] && [ $CONFIG != "release" ] && [ $CONFIG != "test" ]; then
    echo "Invalid build configuration"
    exit 1
fi

# CAUTION: Assumes this script is run from the root repository directory
TARGET_DIR=$PWD/target/$CONFIG

rm -rf "$TARGET_DIR"
mkdir -p "$TARGET_DIR"
cd $TARGET_DIR

if [ $CONFIG == "test" ]; then
    cmake -DBUILD_TESTS=ON ../../
else
    cmake ../../
fi

make
