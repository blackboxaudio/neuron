#include "processors/effects/saturator.h"

using namespace neuron;

Sample Saturator::Process(const Sample input)
{
    float output = tanh((float)input * m_saturation);
    if (input < 0.0f) {
        output = (Sample)(input * (1.0f - m_symmetry)) + (output * m_symmetry);
    }

    return (Sample)clamp(output, -1.0f, 1.0f);
}

void Saturator::SetSaturation(float saturation)
{
    m_saturation = saturation < 1.0f ? 1.0f : saturation;
}

void Saturator::SetSymmetry(float symmetry)
{
    m_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
