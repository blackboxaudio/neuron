#include "processors/effects/wavefolder.h"
#include "utilities/arithmetic.h"

using namespace neuron;

Wavefolder::Wavefolder()
    : p_inputGain(1.0f)
    , p_threshold(1.0f)
    , p_symmetry(1.0f)
{
}

Sample Wavefolder::ProcessImpl(Sample input)
{
    float output = input * p_inputGain;
    while (output > p_threshold || output < -p_threshold) {
        if (output > p_threshold) {
            output = p_threshold - (output - p_threshold);
        } else if (output < -p_threshold) {
            output = -p_threshold - (output + p_threshold);
        }
    }

    if (input < 0.0f) {
        output = input * (1.0f - p_symmetry) + output * p_symmetry;
    }

    return clamp(output, -1.0f, 1.0f);
}

#ifdef NEO_PLUGIN_SUPPORT
void Wavefolder::AttachParameterToSourceImpl(const WavefolderParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case WavefolderParameter::WAVEFOLDER_INPUT_GAIN:
            p_inputGain.AttachSource(source);
            break;
        case WavefolderParameter::WAVEFOLDER_THRESHOLD:
            p_threshold.AttachSource(source);
            break;
        case WavefolderParameter::WAVEFOLDER_SYMMETRY:
            p_symmetry.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif

void Wavefolder::SetInputGain(float gain)
{
    p_inputGain = gain;
}

void Wavefolder::SetThreshold(float threshold)
{
    p_threshold = threshold;
}

void Wavefolder::SetSymmetry(float symmetry)
{
    p_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
