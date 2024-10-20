#pragma once

#include <cmath>

#include "../audio/context.h"
#include "../audio/sample.h"
#include "../utilities/arithmetic.h"

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
    ~Oscillator();

    /**
     * Generates a sample of an audio signal with a
     * basic waveform.
     *
     * @return Sample
     */
    Sample Generate();

    /**
     * Resets the phase of the oscillator, starting it at the beginning
     * waveform position.
     *
     * @param
     */
    void Reset(float phase = 0.0f);

    /**
     * Sets the frequency of the oscillator.
     *
     * @param frequency The new oscillator output frequency.
     */
    void SetFrequency(float frequency);

    /**
     * Attaches a follower oscillator to be synced to this one.
     *
     * @param oscillator The oscillator that will be synced to this one.
     */
    void AttachFollower(Oscillator* oscillator);

    /**
     * Detaches the follower oscillator from this one.
     */
    void DetachFollower();

private:
    void PopulateWavetable();
    Sample Lerp();

    Context& m_context;
    Sample m_wavetable[WAVETABLE_SIZE];
    float m_phase = 0.0f;
    float m_phaseIncrement = 0.0f;

    Oscillator* m_follower = nullptr;
};
} // namespace cortex
