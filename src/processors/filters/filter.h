#pragma once

#include "abstractions/neuron.h"
#include "abstractions/parameter.h"
#include "abstractions/processor.h"
#include "audio/context.h"
#include "audio/sample.h"

namespace neuron {

    const float FILTER_CUTOFF_FREQ_MIN = 20.0f;
    const float FILTER_CUTOFF_FREQ_MAX = 20000.0f;

    enum class FilterParameter {
        CUTOFF_FREQUENCY,
    };

    /**
     * The Filter class applies a simple low-pass filter
     * to audio signals.
     */
    class Filter : public Processor<Filter>, public Neuron<Filter, FilterParameter> {
    public:
        /**
         * Creates a filter processor.
         *
         * @param context The DSP context to be used by the filter.
         * @param cutoffFrequency The initial cutoff frequency of the filter.
         * @return Filter
         */
        explicit Filter(Context& context = DEFAULT_CONTEXT,
            float cutoffFrequency = FILTER_CUTOFF_FREQ_MAX);

        /**
         * Sets the filter's cutoff frequency.
         *
         * @param frequency The new cutoff frequency.
         */
        void SetCutoffFrequency(float frequency);

        Parameter<float> p_cutoffFrequency;

    protected:
        friend class Processor<Filter>;
        Sample ProcessImpl(Sample input);

#ifdef NEO_USE_STD_ATOMIC
        friend class Neuron<Filter, FilterParameter>;
        void AttachParameterToSourceImpl(FilterParameter parameter, std::atomic<float>* source);
#endif

    private:
        void CalculateAlpha();

        Context& m_context;

        float m_alpha;
        Sample m_previousOutput;
    };

}
