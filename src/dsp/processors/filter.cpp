#include "neuron/dsp/processors/filter.h"
#include "neuron/utils/arithmetic.h"

using namespace neuron;

Filter::Filter(Context& context, float cutoffFrequency)
    : p_cutoffFrequency(cutoffFrequency)
    , m_context(context)
    , m_previousOutput(0.0f)
{
    SetCutoffFrequency(cutoffFrequency);
}

void Filter::ProcessImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    const Sample oneMinusAlpha = 1.0f - m_alpha;
    for (int i = 0; i < input.size(); i++) {
        Sample value = input[i] * m_alpha + oneMinusAlpha * m_previousOutput;
        m_previousOutput = value;
        output[i] = value;
    }
}

#ifdef NEO_PLUGIN_SUPPORT
void Filter::AttachParameterToSourceImpl(FilterParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case FilterParameter::FILTER_CUTOFF_FREQUENCY:
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
    float deltaTime = 1.0f / static_cast<float>(m_context.sampleRate);
    m_alpha = deltaTime / (cutoffResponse + deltaTime);
}
