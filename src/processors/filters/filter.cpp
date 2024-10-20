#include "filter.h"

using namespace cortex;

Filter::Filter(cortex::Context& context, float cutoffFrequency)
    : m_context(context)
    , m_cutoffFrequency(cutoffFrequency)
    , m_previousOutput(0.0f)
{
    SetCutoffFrequency(cutoffFrequency);
}

Sample Filter::Process(const cortex::Sample input)
{
    float output = m_alpha * (float)input + (1.0f - m_alpha) * (float)m_previousOutput;
    m_previousOutput = (Sample)output;
    return (Sample)output;
}

void Filter::SetCutoffFrequency(float frequency)
{
    m_cutoffFrequency = clamp(frequency, FILTER_CUTOFF_FREQ_MIN, FILTER_CUTOFF_FREQ_MAX);
    CalculateAlpha();
}

void Filter::CalculateAlpha()
{
    float cutoffResponse = 1.0f / (2.0f * PI * m_cutoffFrequency);
    float deltaTime = 1.0f / (float)m_context.sampleRate;
    m_alpha = deltaTime / (cutoffResponse + deltaTime);
}
