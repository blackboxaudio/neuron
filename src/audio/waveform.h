#pragma once

#include "../utilities/arithmetic.h"
#include "sample.h"

namespace cortex {
/**
 * The basic waveform variants, i.e. sine, triangle,
 * sawtooth, and square.
 */
enum class Waveform {
    SINE,
    TRIANGLE,
    SAWTOOTH,
    SQUARE,
};

/**
 * Converts the value of a sine wave to a basic waveform.
 *
 * @param sample The sample value to convert.
 * @param waveform The waveform to convert the sample to.
 * @return Sample
 */
inline Sample SineToWaveform(Sample sample, Waveform waveform)
{
    switch (waveform) {
    default:
    case Waveform::SINE:
        return sample;
    case Waveform::TRIANGLE:
        return sample >= 0.0f ? (2.0f * sample - 1.0f) : (2.0f * -sample - 1.0f);
    case Waveform::SAWTOOTH:
        return (2.0f * PI * sample) - floorf((2.0f * PI * sample) + 0.5f);
    case Waveform::SQUARE:
        return sample >= 0.0f ? 1.0f : -1.0f;
    }
}
}