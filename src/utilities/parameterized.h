#pragma once

#include <atomic>

namespace neuron {
    class Parameterized {
    public:
        virtual ~Parameterized() = default;

        // Attach a parameter (e.g. via an atomic float pointer)
        virtual void AttachParameter(int parameterId, std::atomic<float>* parameter) = 0;
    };
}
