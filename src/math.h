#pragma once

#include <algorithm>

namespace cortex {
    /**
     * Returns a value clamped within a lower and upper bound.
     * @tparam T
     * @param n
     * @param lower
     * @param upper
     * @return
     */
    template <typename T>
    T clamp(const T n, const T lower, const T upper) {
        return std::max(lower, std::min(n, upper));
    }
}
