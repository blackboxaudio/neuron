#pragma once

namespace cortex {
    /**
 * Converts a MIDI note number to a (floating-point) frequency.
 *
 * @param n The MIDI note number to convert
 * @return
 */
    template <typename T>
    inline T midi_to_frequency(T n)
    {
        return powf(2, (n - 69.0f) / 12.0f) * 440.0f;
    }
}
