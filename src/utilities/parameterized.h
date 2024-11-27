#pragma once

#ifdef NEO_ENABLE_PLUGIN_SUPPORT
#include <atomic>
#endif

namespace neuron {
#ifdef NEO_ENABLE_PLUGIN_SUPPORT
    class Parameterized {
    public:
        virtual ~Parameterized() = default;

        // Attach a parameter (e.g. via an atomic float pointer)
        virtual void AttachParameter(int parameter, std::atomic<float>* value) = 0;
    };
#else
    class Parameterized {};
#endif
}
