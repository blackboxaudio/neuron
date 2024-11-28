#include "processors/filters/filter.h"

using namespace neuron;

Filter::Filter(Context& context, float cutoffFrequency)
    : m_context(context), m_previousOutput(0.0f), p_cutoffFrequency(cutoffFrequency)
{
    SetCutoffFrequency(cutoffFrequency);
}

Sample Filter::Process(Sample input)
{
    float output = m_alpha * input + (1.0f - m_alpha) * m_previousOutput;
    m_previousOutput = output;
    return output;
}

void Filter::SetCutoffFrequency(float frequency)
{
    p_cutoffFrequency = clamp(frequency, FILTER_CUTOFF_FREQ_MIN, FILTER_CUTOFF_FREQ_MAX);
    CalculateAlpha();
}

void Filter::CalculateAlpha()
{
    float cutoffResponse = 1.0f / (2.0f * PI * p_cutoffFrequency);
    float deltaTime = 1.0f / (float)m_context.sampleRate;
    m_alpha = deltaTime / (cutoffResponse + deltaTime);
}
