#pragma once

#include "neuron/core/context.h"
#include "neuron/dsp/modulators/modulator.h"

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

        void SetContext(const Context& context)
        {
            static_cast<N*>(this)->SetContextImpl(context);
        }

        template<class M>
        void AttachModulator(const P parameter, Modulator<M>* modulator)
        {
            static_cast<N*>(this)->AttachModulatorImpl(parameter, modulator);
        }

        void DetachModulator(const P parameter)
        {
            static_cast<N*>(this)->DetachModulatorImpl(parameter);
        }

        void SetModulationDepth(const P parameter, float depth)
        {
            static_cast<N*>(this)->SetModulationDepthImpl(parameter, depth);
        }

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
