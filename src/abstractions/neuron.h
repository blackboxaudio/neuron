#pragma once

#ifdef NEO_USE_STD_ATOMIC
#include <atomic>
#endif

namespace neuron {

    /**
     * Describes a Neuron DSP component, capable of processing, or
     * generating signals.
     */
    template<class N, typename P>
    class Neuron {
    public:
        /**
         * Frees any memory allocated by the processor.
         */
        ~Neuron() = default;

#ifdef NEO_USE_STD_ATOMIC
        /**
         * Attach a source via an atomic pointer to a given parameter.
         */
        void AttachParameter(const P parameter, std::atomic<float>* source)
        {
            static_cast<N*>(this)->AttachParameterImpl(parameter, source);
        }
#endif
    };

}
