#include "logger.h"

#include <iostream>

using namespace cortex;

void Logger::Log(std::string strings...)
{
    std::string result = "";

    for (auto string : strings) {
        result += string;
    }

    std::cout << result << std::endl;
}
