#!/bin/bash

convertsecs() {
    ((m = (${1} % 3600) / 60))
    ((s = ${1} % 60))
    printf "%02dm %02ds\n" $m $s
}

START_TIME=$(date +%s)

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

CMAKE_FLAGS=$([ $CONFIG == "test" ] && echo "-DBUILD_TESTS=ON")
cmake $CMAKE_FLAGS ../../
if [ $? -ne 0 ]; then
    printf "Failed to generate build files\n"
    exit 1
fi

make
if [ $? -ne 0 ]; then
    printf "Failed to compile code\n"
    exit 1
fi

END_TIME=$(date +%s)
EXEC_TIME=$(convertsecs $(expr $END_TIME - $START_TIME))

printf "\nDone ($EXEC_TIME)\n"
