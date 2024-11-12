#include <chrono>

#include "utilities/logger.h"

namespace neuron {

class Timer {
public:
    Timer()
    {
        m_startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto durationMs = (end - start) * 0.001f;
        NEO_INFO(std::to_string(durationMs) + "ms");
    }

private:
    std::chrono::high_resolution_clock::time_point m_startTimepoint;
};

}