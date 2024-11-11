#pragma once

#include "audio/context.h"
#include "audio/sample.h"
#include "utilities/arithmetic.h"

namespace cortex {
const float FILTER_CUTOFF_FREQ_MIN = 20.0f;
const float FILTER_CUTOFF_FREQ_MAX = 20000.0f;

/**
 * The Filter class applies a simple low-pass filter
 * to audio signals.
 */
class Filter {
public:
    /**
     * Creates a filter processor.
     *
     * @param context The DSP context to be used by the filter.
     * @param cutoffFrequency The initial cutoff frequency of the filter.
     * @return Filter
     */
    Filter(Context& context = DEFAULT_CONTEXT,
        float cutoffFrequency = FILTER_CUTOFF_FREQ_MAX);

    /**
     * Frees any memory allocated by the oscillator.
     */
    ~Filter() { }

    /**
     * Applies a low-pass filter to an input sample.
     *
     * @param input The input sample to be processed.
     * @return Sample
     */
    Sample Process(const Sample input);

    /**
     * Sets the filter's cutoff frequency.
     *
     * @param frequency The new cutoff frequency.
     */
    void SetCutoffFrequency(float frequency);

private:
    void CalculateAlpha();

    Context& m_context;

    float m_cutoffFrequency;
    float m_alpha;
    Sample m_previousOutput;
};

}
