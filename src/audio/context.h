#pragma once

#include <cmath>

namespace cortex {

/**
 * Contains information about the context in which DSP operations
 * will be executed, useful for some components such as oscillators
 * that use the sample rate to calculate phase positions.
 */
struct Context {
    size_t sampleRate;
    size_t numChannels;
    size_t blockSize;
};

/**
 * The common default context, using a sample rate of 44.1kHz, stereo
 * channel configuration, and a buffer size of 16 samples.
 */
static Context DEFAULT_CONTEXT = { 44100, 2, 16 };
}
