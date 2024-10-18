#include "wavefolder.h"

using namespace cortex;

Wavefolder::Wavefolder(float threshold, float inputGain)
    : m_threshold(threshold)
    , m_inputGain(inputGain)
{
}

Sample Wavefolder::Process(const Sample input)
{
    Sample output = input * m_inputGain;
    while (output > m_threshold || output < -m_threshold) {
        if (output > m_threshold) {
            output = m_threshold - (output - m_threshold);
        } else if (output < -m_threshold) {
            output = -m_threshold - (output + m_threshold);
        }
    }
    return output;
}

void Wavefolder::SetThreshold(float threshold)
{
    m_threshold = threshold;
}

void Wavefolder::SetInputGain(float gain)
{
    m_inputGain = gain;
}
