#!/bin/bash

printf "Formatting code...\n"

find include/ -iname '*.h' -o | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format source code\n"
    exit 1
fi

find src/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format source code\n"
    exit 1
fi

find tests/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format test code\n"
    exit 1
fi

printf "Done.\n"
