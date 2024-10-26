#pragma once

#include <memory>

namespace cortex {

class Logger {
public:
    static void Log(std::string str);
};

}

#define CX_INFO(...) ::cortex::Logger::Log(__VA_ARGS__)
#define CX_WARN(...) ::cortex::Logger::Log(__VA_ARGS__)
#define CX_ERROR(...) ::cortex::Logger::Log(__VA_ARGS__)