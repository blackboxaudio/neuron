#pragma once

#include <cmath>

namespace cortex {
    struct Context {
        size_t sampleRate;
        size_t numChannels;
        size_t blockSize;
    };

    static Context DEFAULT_CONTEXT = {
        44100,
        2,
        16
    };
}
