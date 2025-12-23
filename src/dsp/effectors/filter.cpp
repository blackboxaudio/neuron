#include "neuron/dsp/effectors/filter.h"
#include "neuron/utils/arithmetic.h"

using namespace neuron;

Filter::Filter(Context context, float cutoffFrequency)
    : p_cutoffFrequency(cutoffFrequency)
    , p_cutoffFrequencyModulationDepth(0.0f)
    , m_previousOutput(0.0f)
{
    SetContext(context);
}

void Filter::SetCutoffFrequency(float frequency)
{
    p_cutoffFrequency = clamp(frequency, FILTER_CUTOFF_FREQ_MIN, FILTER_CUTOFF_FREQ_MAX);
    CalculateAlpha();
}

void Filter::EffectImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    float alpha = m_alpha;

    if (m_cutoffFrequencyModulator.IsValid()) {
        float modValue = m_cutoffFrequencyModulator.GetModulationValue();
        float range = FILTER_CUTOFF_FREQ_MAX - FILTER_CUTOFF_FREQ_MIN;
        float modulatedCutoff = p_cutoffFrequency + modValue * p_cutoffFrequencyModulationDepth * range;
        modulatedCutoff = clamp(modulatedCutoff, FILTER_CUTOFF_FREQ_MIN, FILTER_CUTOFF_FREQ_MAX);

        float cutoffResponse = 1.0f / (2.0f * PI * modulatedCutoff);
        float deltaTime = 1.0f / static_cast<float>(m_context.sampleRate);
        alpha = deltaTime / (cutoffResponse + deltaTime);
    }

    const Sample oneMinusAlpha = 1.0f - alpha;
    for (int i = 0; i < input.size(); i++) {
        Sample value = input[i] * alpha + oneMinusAlpha * m_previousOutput;
        m_previousOutput = value;
        output[i] = value;
    }
}

void Filter::SetContextImpl(Context /* context */)
{
    CalculateAlpha();
}

void Filter::DetachModulatorImpl(FilterParameter parameter)
{
    switch (parameter) {
        case FilterParameter::FILTER_CUTOFF_FREQUENCY:
            m_cutoffFrequencyModulator.Detach();
            break;
        default:
            break;
    }
}

void Filter::SetModulationDepthImpl(FilterParameter parameter, float depth)
{
    switch (parameter) {
        case FilterParameter::FILTER_CUTOFF_FREQUENCY:
            p_cutoffFrequencyModulationDepth = depth;
            break;
        default:
            break;
    }
}

#if NEO_PLUGIN_SUPPORT
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

void Filter::CalculateAlpha()
{
    float cutoffResponse = 1.0f / (2.0f * PI * p_cutoffFrequency);
    float deltaTime = 1.0f / static_cast<float>(m_context.sampleRate);
    m_alpha = deltaTime / (cutoffResponse + deltaTime);
}
