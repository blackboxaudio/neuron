#include "logger.h"

#include <ctime>
#include <iostream>

using namespace cortex;

void Logger::Log(std::string str)
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "Y-%m-%d %H:%M:%S", localTime);

    std::cout << "[" << buffer << "] "
              << "CORTEX: " << str << std::endl;
}
