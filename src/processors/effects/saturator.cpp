#include "processors/effects/saturator.h"

using namespace neuron;

Saturator::Saturator()
    : p_saturation(1.0f), p_symmetry(1.0f)
{

}


Sample Saturator::Process(Sample input)
{
    float output = tanh(input * p_saturation);
    if (input < 0.0f) {
        output = (input * (1.0f - p_symmetry)) + (output * p_symmetry);
    }

    return (Sample)clamp(output, -1.0f, 1.0f);
}

void Saturator::SetSaturation(float saturation)
{
    p_saturation = saturation < 1.0f ? 1.0f : saturation;
}

void Saturator::SetSymmetry(float symmetry)
{
    p_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
