#!/bin/bash

CONFIG=${1:-debug}
if [ $CONFIG != "debug" ] && [ $CONFIG != "release" ]; then
    echo "Invalid build configuration"
    exit 1
fi

# CAUTION: Assumes this script is run from the root repository directory
TARGET_DIR=$PWD/target/$CONFIG

mkdir -p "$TARGET_DIR"

cd $TARGET_DIR

cmake ../../

make
