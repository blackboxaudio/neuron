#include "wavefolder.h"

using namespace cortex;

Sample Wavefolder::Process(const Sample input)
{
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
