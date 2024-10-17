#pragma once

#include <algorithm>
#include <cmath>

namespace cortex {
const float ZERO = 0.0f;
const float IDENTITY = 1.0f;
const float PI = 3.14159265358979323846264338327950288f;
const float EULER = 2.71828182845904523536028747135266250;

enum class Mapping {
    EXP,
    LOG,
    LINEAR,
};

template <typename T>
inline T clamp(const T n, const T min, const T max)
{
    return std::max(min, std::min(n, max));
}

template <typename T>
inline T map(const T n, const T min, const T max, const Mapping curve = Mapping::LINEAR)
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

template <typename T>
inline T midi_to_frequency(T n)
{
    return powf(2, (n - 69.0f) / 12.0f) * 440.0f;
}
}