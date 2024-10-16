#pragma once

#include <algorithm>

namespace cortex {
    const float ZERO = 0.0f;
    const float IDENTITY = 1.0f;
    const float PI = 3.14159265358979323846264338327950288f;
    const float EULER = 2.71828182845904523536028747135266250;

    template <typename T>
    T clamp(T n, T lower, T upper);
}
