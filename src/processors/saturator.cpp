#include "saturator.h"

using namespace cortex;

Saturator::Saturator() { }

Saturator::~Saturator() { }

Sample Saturator::Process(const Sample input)
{
    float output = fast_tanh((float)input * m_saturation);
    if (input >= 0.0f) {
        return (Sample)output;
    } else {
        return (Sample)(input * (1.0f - m_symmetry)) + (output * m_symmetry);
    }
}

void Saturator::SetSaturation(float saturation)
{
    m_saturation = saturation;
}

void Saturator::SetSymmetry(float symmetry)
{
    m_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
