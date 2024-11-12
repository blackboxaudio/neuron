#pragma once

#include <algorithm>
#include <cmath>

namespace neuron {

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
        return clamp<T>(min + (n * n) * (max - min), min, max);
    case Mapping::LOG: {
        const float a = 1.f / log10f(max / min);
        return clamp<T>(min * powf(10, n / a), min, max);
    }
    case Mapping::LINEAR:
    default:
        return clamp<T>(min + n * (max - min), min, max);
    }
}

/**
 * A fast approximation of the hyperbolic tangent function, or `tanh`.
 *
 * NOTE: This function works best on a limited range from -5 to 5.
 *
 * @param n The function input.
 * @return
 */
template <typename T>
inline T tanh(T n)
{
    auto n2 = n * n;
    auto numerator = n * (135135 + n2 * (17325 + n2 * (378 + n2)));
    auto denominator = 135135 + n2 * (62370 + n2 * (3150 + 28 * n2));
    return numerator / denominator;
}

/**
 * A fast approximation of the exponential function, or `exp`.
 *
 * NOTE: This function works best on a limited range from -6 to 4.
 *
 * @param n The function input.
 * @return
 */
template <typename T>
inline T exp(T n)
{
    auto numerator = 1680 + n * (840 + n * (180 + n * (20 + n)));
    auto denominator = 1680 + n * (-840 + n * (180 + n * (-20 + n)));
    return numerator / denominator;
}

}
