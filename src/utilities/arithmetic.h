#pragma once

#include <algorithm>
#include <cmath>

namespace cortex {
/**
 * A value for the absence of quantity; nothing.
 */
const float ZERO = 0.0f;

/**
 * A value that leaves any other value unchanged
 * when multiplied with it.
 */
const float IDENTITY = 1.0f;

/**
 * An irrational number that represents the ratio of the circumference of a
 * circle to its diameter.
 */
const float PI = 3.14159265358979323846264338327950288f;

/**
 * An irrational number that is the base of the natural logarithm.
 */
const float EULER = 2.71828182845904523536028747135266250;

/**
 * Depicts different mathematical curves, e.g. exponential,
 * linear, logarithmic.
 */
enum class Mapping {
    EXP,
    LOG,
    LINEAR,
};

/**
 * Constricts a number between a lower and upper bound.
 *
 * @param n The number to the be clamped.
 * @param min The lowest possible number n can be.
 * @param max The highest possible number n can be.
 * @return T
 */
template <typename T>
inline T clamp(const T n, const T min, const T max)
{
    return std::max(min, std::min(n, max));
}

/**
 * Scales a number between 0 and 1 to a number within a range from min to max.
 *
 * @param n The number to scale.
 * @param min The lower bound of the range to scale n.
 * @param max The upper bound of the range to scale n.
 * @param curve The curve in which to scale n (e.g. linear, exponential,
 * logarithmic)
 * @return T
 */
template <typename T>
inline T map(const T n, const T min, const T max,
    const Mapping curve = Mapping::LINEAR)
{
    switch (curve) {
    case Mapping::EXP:
        return clamp(min + (n * n) * (max - min), min, max);
    case Mapping::LOG: {
        const float a = 1.f / log10f(max / min);
        return clamp(min * powf(10, n / a), min, max);
    }
    case Mapping::LINEAR:
    default:
        return clamp(min + n * (max - min), min, max);
    }
}

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

/**
 * A fast approximation of the hyperbolic tangent function, or `tanh`.
 *
 * @param n The function input.
 * @return
 */
template <typename T>
inline T fast_tanh(T n)
{
    T n2 = n * n;
    return n * (n2 + 27) / (n2 * 9 + 27);
}
} // namespace cortex
