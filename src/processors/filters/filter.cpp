#include "processors/filters/filter.h"
#include "utilities/arithmetic.h"

using namespace neuron;

Filter::Filter(Context& context, float cutoffFrequency)
    : p_cutoffFrequency(cutoffFrequency)
    , m_context(context)
    , m_previousOutput(0.0f)
{
    SetCutoffFrequency(cutoffFrequency);
}

Sample Filter::ProcessImpl(Sample input)
{
    float output = m_alpha * input + (1.0f - m_alpha) * m_previousOutput;
    m_previousOutput = output;
    return output;
}

#ifdef NEO_PLUGIN_SUPPORT
void Filter::AttachParameterToSourceImpl(FilterParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case FilterParameter::CUTOFF_FREQUENCY:
            p_cutoffFrequency.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif

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
