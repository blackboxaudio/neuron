#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"
#include "neuron/dsp/processors/processor.h"

namespace neuron {

    const float FILTER_CUTOFF_FREQ_MIN = 20.0f;
    const float FILTER_CUTOFF_FREQ_MAX = 20000.0f;

    enum FilterParameter {
        FILTER_CUTOFF_FREQUENCY,
    };

    /**
     * The Filter class applies a simple low-pass filter
     * to audio signals.
     */
    class Filter : public Processor<Filter>, public Neuron<Filter, FilterParameter> {
    public:
        /**
         * Creates a filter processor.
         */
        explicit Filter(float cutoffFrequency = FILTER_CUTOFF_FREQ_MAX);

        /**
         * Sets the filter's cutoff frequency.
         */
        void SetCutoffFrequency(float frequency);

    protected:
        friend class Processor<Filter>;
        void ProcessImpl(Buffer<Sample>& input, Buffer<Sample>& output);

        friend class Neuron<Filter, FilterParameter>;
        void SetContextImpl(Context context);
        template<class M>
        void AttachModulatorImpl(FilterParameter parameter, Modulator<M>* modulator);
        void DetachModulatorImpl(FilterParameter parameter);
        void SetModulationDepthImpl(FilterParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(FilterParameter parameter, std::atomic<float>* source);
#endif

    private:
        void CalculateAlpha();

        Parameter<float> p_cutoffFrequency;
        Parameter<float> p_cutoffFrequencyModulationDepth;
        ModulationSource m_cutoffFrequencyModulator;

        float m_alpha;
        Sample m_previousOutput;
    };

}
