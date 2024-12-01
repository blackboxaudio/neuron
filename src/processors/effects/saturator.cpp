#include "processors/effects/saturator.h"
#include "utilities/arithmetic.h"

using namespace neuron;

Saturator::Saturator()
    : p_saturation(1.0f)
    , p_symmetry(1.0f)
{
}

Sample Saturator::ProcessImpl(Sample input)
{
    float output = tanh(input * p_saturation);
    if (input < 0.0f) {
        output = (input * (1.0f - p_symmetry)) + (output * p_symmetry);
    }

    return clamp(output, -1.0f, 1.0f);
}

#ifdef NEO_PLUGIN_SUPPORT
void Saturator::AttachParameterToSourceImpl(SaturatorParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case SaturatorParameter::SATURATION:
            p_saturation.AttachSource(source);
            break;
        case SaturatorParameter::SYMMETRY:
            p_symmetry.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif

void Saturator::SetSaturation(float saturation)
{
    p_saturation = saturation < 1.0f ? 1.0f : saturation;
}

void Saturator::SetSymmetry(float symmetry)
{
    p_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
