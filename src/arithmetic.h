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
T clamp(T n, T lower, T upper);

template <typename T>
T map(T n, T min, T max, Mapping curve = Mapping::LINEAR);

template <typename T>
T midi_to_frequency(T n);
}
