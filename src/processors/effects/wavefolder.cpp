#include "processors/effects/wavefolder.h"

using namespace neuron;

Wavefolder::~Wavefolder()
{
#ifdef NEO_ENABLE_PLUGIN_SUPPORT
    a_inputGain = nullptr;
#endif
}

Sample Wavefolder::Process(const Sample input)
{
#ifdef NEO_ENABLE_PLUGIN_SUPPORT
    m_inputGain = a_inputGain->load();
#endif

    float output = (float)input * m_inputGain;
    while (output > m_threshold || output < -m_threshold) {
        if (output > m_threshold) {
            output = m_threshold - (output - m_threshold);
        } else if (output < -m_threshold) {
            output = -m_threshold - (output + m_threshold);
        }
    }

    if (input < 0.0f) {
        output = (Sample)(input * (1.0f - m_symmetry)) + (output * m_symmetry);
    }

    return (Sample)clamp(output, -1.0f, 1.0f);
}

void Wavefolder::SetInputGain(float gain)
{
#ifdef NEO_ENABLE_PLUGIN_SUPPORT
    a_inputGain->store(gain);
#endif
    m_inputGain = gain;
}

void Wavefolder::SetThreshold(float threshold)
{
    m_threshold = threshold;
}

void Wavefolder::SetSymmetry(float symmetry)
{
    m_symmetry = clamp(symmetry, 0.0f, 1.0f);
}

#ifdef NEO_ENABLE_PLUGIN_SUPPORT
void Wavefolder::AttachParameter(int parameter, std::atomic<float>* value)
{
    switch ((WavefolderParameter)parameter) {
        case InputGain:
            a_inputGain = value;
            break;
        default:
            // Do nothing.
            break;
    }
}
#endif
