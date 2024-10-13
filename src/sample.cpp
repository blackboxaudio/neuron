#include "math.h"
#include "sample.h"

using namespace cortex;

Sample::Sample(float value) {
    m_value = clamp(value, -1.0f, 1.0f);
}

Sample::~Sample() { }

float Sample::equilibrium() {
    m_value = 0.0f;
    return m_value;
}

float Sample::gain(float factor) {
    m_value *= clamp(factor, -1.0f, 1.0f);
    return m_value;
}
