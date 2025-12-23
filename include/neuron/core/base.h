#pragma once

#include "neuron/core/context.h"
#include "neuron/dsp/modulators/modulator.h"

#if NEO_PLUGIN_SUPPORT
#include <atomic>
#endif

namespace neuron {

    /**
     * Describes a "neuron", i.e. a DSP component capable of generating signals,
     * processing signals, or modulating parameters of other neurons.
     */
    template<class N, typename P>
    class Neuron {
    public:
        /**
         * Frees any memory allocated by the processor.
         */
        ~Neuron() = default;

        /**
         * Sets the DSP context for this neuron, holding information
         * such as sample rate, block size, and number of channels.
         */
        void SetContext(Context context)
        {
            m_context = context;
            static_cast<N*>(this)->SetContextImpl(context);
        }

        /**
         * Attaches a modulator object to a given parameter of this neuron.
         *
         * CAUTION: Modulators MUST be evaluated in the audio callback before
         * any other neurons. If modulator A is modulating modulator B, then A
         * must be evaluated before B is. A and B must both be evaluated before any
         * of the neurons they modulate are evaluated.
         */
        template<class M>
        void AttachModulator(P parameter, Modulator<M>* modulator)
        {
            static_cast<N*>(this)->AttachModulatorImpl(parameter, modulator);
        }

        /**
         * Detaches a modulator object from a given parameter of this neuron.
         */
        void DetachModulator(P parameter)
        {
            static_cast<N*>(this)->DetachModulatorImpl(parameter);
        }

        /**
         * Sets the modulation depth of a given parameter of this neuron, which is a value
         * between -1.0f and 1.0f.
         *
         * NOTE: If no modulator has been attached then this will have no effect.
         */
        void SetModulationDepth(P parameter, float depth)
        {
            static_cast<N*>(this)->SetModulationDepthImpl(parameter, depth);
        }

#if NEO_PLUGIN_SUPPORT
        /**
         * Attach a source via an atomic pointer to a given parameter.
         */
        void AttachParameterToSource(P parameter, std::atomic<float>* source)
        {
            static_cast<N*>(this)->AttachParameterToSourceImpl(parameter, source);
        }
#endif

    protected:
        Context m_context = DEFAULT_CONTEXT;
    };
}
