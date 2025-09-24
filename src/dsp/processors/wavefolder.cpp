#include "neuron/dsp/processors/wavefolder.h"
#include "neuron/utils/arithmetic.h"

using namespace neuron;

Wavefolder::Wavefolder()
    : p_inputGain(1.0f)
    , p_threshold(1.0f)
    , p_symmetry(1.0f)
{
}

void Wavefolder::ProcessImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    Sample negativeThreshold = -p_threshold * p_symmetry;

    for (int i = 0; i < input.size(); i++) {
        Sample value = input[i] * p_inputGain;

        while (value > p_threshold) {
            value = 2.0f * p_threshold - value;
        }

        while (value < negativeThreshold) {
            value = 2.0f * negativeThreshold - value;
        }

        output[i] = clamp(value, -1.0f, 1.0f);
    }
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
