#pragma once

#include <cmath>

#include "../utilities/arithmetic.h"
#include "context.h"
#include "sample.h"

namespace cortex {
const size_t WAVETABLE_SIZE = 256;

/**
 * The Oscillator class creates an audio signal
 * with a basic waveform.
 */
class Oscillator {
public:
    /**
     * Creates an oscillator generator.
     *
     * @param context The DSP context to be used by the oscillator.
     * @param frequency The initial frequency of the oscillator.
     * @return Oscillator
     */
    Oscillator(Context& context = DEFAULT_CONTEXT, float frequency = 440.0f);

    /**
     * Frees any memory allocated by the oscillator.
     */
    ~Oscillator() { }

    /**
     * Generates a sample of an audio signal with a
     * basic waveform.
     *
     * @return Sample
     */
    Sample Generate();

    void SetFrequency(float frequency);

private:
    void PopulateWavetable();
    Sample Lerp();

    Context& m_context;
    Sample m_wavetable[WAVETABLE_SIZE];
    float m_phase = 0.0f;
    float m_phaseIncrement = 0.0f;
};
}
