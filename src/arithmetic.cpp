#include "arithmetic.h"

using namespace cortex;

template <typename T>
inline T clamp(const T n, const T lower, const T upper)
{
    return std::max(lower, std::min(n, upper));
}
