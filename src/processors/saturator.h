#pragma once

#include <cmath>

#include "../utilities/arithmetic.h"
#include "sample.h"

namespace cortex {
/**
 * The Saturator class applies a tape saturation
 * algorithm to audio signals.
 */
class Saturator {
public:
    /**
     * Creates a default saturator processor.
     *
     * @return Saturator
     */
    Saturator();

    /**
     * Frees any memory allocated by the saturator.
     */
    ~Saturator();

    /**
     * Applies a saturation algorithm to an input sample.
     *
     * @param input The input sample to be processed.
     * @return Sample
     */
    Sample Process(const Sample input);

    /**
     * Sets the saturation level, which boosts the signal before
     * distortion is applied.
     *
     * @param saturation The multiplier of the audio signal going into the distortion algorithm.
     */
    void SetSaturation(float saturation);

    /**
     * Sets the symmetry of the algorithm, determining how much
     * distortion to apply to the positive and negative parts
     * of the signal separately.
     *
     * @param symmetry A value between 0.0 and 1.0, ranging from asymmetrical (one-sided) to symmetrical respectively.
     */
    void SetSymmetry(float symmetry);

private:
    float m_saturation = 1.0f;
    float m_symmetry = 0.0f;
};
}