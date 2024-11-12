#pragma once

#include "audio/sample.h"
#include "utilities/arithmetic.h"

namespace neuron {

/**
 * The Wavefolder class applies a wavefolding
 * algorithm to audio signals.
 */
class Wavefolder {
public:
    /**
     * Creates a default wavefolder processor.
     */
    Wavefolder() {};

    /**
     * Frees any memory allocated by the wavefolder.
     */
    ~Wavefolder() {};

    /**
     * Applies a wavefolding algorithm to an input sample.
     *
     * @param input The input sample to be processed.
     * @return Sample
     */
    Sample Process(const Sample input);

    /**
     * Sets the input gain level, which boosts the signal before
     * being measured against the wavefolder threshold.
     *
     * @param gain The multiplier of the audio signal going into the
     * wavefolding algorithm.
     */
    void SetInputGain(float gain);

    /**
     * Sets the threshold of the wavefolder, above which samples will
     * be "folded" toawrds zero until they are within the threshold.
     */
    void SetThreshold(float threshold);

    /**
     * Sets the symmetry of the algorithm, determining how much
     * wavefolding to apply to the positive and negative parts
     * of the signal separately.
     *
     * @param symmetry A value between 0.0 and 1.0, ranging from asymmetrical
     * (one-sided) to symmetrical respectively.
     */
    void SetSymmetry(float symmetry);

private:
    float m_inputGain = 1.0f;
    float m_threshold = 1.0f;
    float m_symmetry = 1.0f;
};

}