#pragma once

#include "audio/context.h"
#include "audio/sample.h"
#include "utilities/arithmetic.h"
#include "utilities/parameter.h"
#include "utilities/processor.h"

namespace neuron {

    const float FILTER_CUTOFF_FREQ_MIN = 20.0f;
    const float FILTER_CUTOFF_FREQ_MAX = 20000.0f;

    /**
     * The Filter class applies a simple low-pass filter
     * to audio signals.
     */
    class Filter : public Processor<Filter> {
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

    private:
        void CalculateAlpha();

        Context& m_context;

        float m_alpha;
        Sample m_previousOutput;
    };

}
