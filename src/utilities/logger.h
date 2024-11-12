#pragma once

#include <string>

namespace neuron {

class Logger {
public:
    static void Log(std::string str);
};

}

#define NEO_TRACE(...) ::neuron::Logger::Log(__VA_ARGS__)
#define NEO_INFO(...) ::neuron::Logger::Log(__VA_ARGS__)
#define NEO_WARN(...) ::neuron::Logger::Log(__VA_ARGS__)
#define NEO_ERROR(...) ::neuron::Logger::Log(__VA_ARGS__)
