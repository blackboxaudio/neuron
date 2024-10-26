#pragma once

#include <cmath>

#include "utilities/arithmetic.h"

namespace cortex {

/**
 * Converts a MIDI note number to a (floating-point) frequency. Any value outside
 * of the proper note range [0, 127] will be clamped.
 *
 * @param n The MIDI note number to convert
 * @return
 */
template <typename T>
inline T midi_to_frequency(T n)
{
    return powf(2, (clamp(n, 0.0f, 127.0f) - 69.0f) / 12.0f) * 440.0f;
}

}
