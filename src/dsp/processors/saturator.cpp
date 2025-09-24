#include "neuron/dsp/processors/saturator.h"
#include "neuron/utils/arithmetic.h"

using namespace neuron;

Saturator::Saturator()
    : p_saturation(1.0f)
    , p_symmetry(1.0f)
{
}

void Saturator::ProcessImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    Sample oneMinusSymmetry = 1.0f - p_symmetry;
    for (int i = 0; i < input.size(); i++) {
        Sample value = tanh(input[i] * p_saturation);
        if (value < 0.0f) {
            value = input[i] * oneMinusSymmetry + value * p_symmetry;
        }
        output[i] = clamp(value, -1.0f, 1.0f);
    }
}

#ifdef NEO_PLUGIN_SUPPORT
void Saturator::AttachParameterToSourceImpl(SaturatorParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case SaturatorParameter::SATURATOR_SATURATION:
            p_saturation.AttachSource(source);
            break;
        case SaturatorParameter::SATURATOR_SYMMETRY:
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
