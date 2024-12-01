#pragma once

#ifdef NEO_PLUGIN_SUPPORT
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

#ifdef NEO_PLUGIN_SUPPORT
        /**
         * Attach a source via an atomic pointer to a given parameter.
         */
        void AttachParameterToSource(const P parameter, std::atomic<float>* source)
        {
            static_cast<N*>(this)->AttachParameterToSourceImpl(parameter, source);
        }
#endif
    };

}
