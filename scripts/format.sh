#!/bin/bash

cd src/
if [ $? -ne 0 ]; then
    printf "Failed to find \"src\" directory\n"
    exit 1
fi

printf "Formatting code...\n"
find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=WebKit
if [ $? -ne 0 ]; then
    printf "Failed to format code\n"
    exit 1
fi

printf "Done.\n"
