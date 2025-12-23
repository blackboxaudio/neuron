#include "neuron/dsp/effectors/gain.h"
#include "neuron/utils/arithmetic.h"

#include <cmath>

using namespace neuron;

Gain::Gain(Context context, float smoothingMs)
    : p_gainDb(0.0f)
    , p_gainModulationDepth(0.0f)
    , m_smoothedGain(1.0f)  // 0 dB = 1.0 linear
    , m_smoothingMs(smoothingMs)
{
    SetContext(context);
}

void Gain::SetGainDb(float db)
{
    p_gainDb = clamp(db, GAIN_DB_MIN, GAIN_DB_MAX);
}

void Gain::SetSmoothingTime(float ms)
{
    m_smoothingMs = ms;
    m_smoothedGain.Reset(m_context.sampleRate, m_smoothingMs);
}

void Gain::EffectImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    float targetDb = p_gainDb;

    // Apply modulation if present
    if (m_gainModulator.IsValid()) {
        float modValue = m_gainModulator.GetModulationValue();
        float range = GAIN_DB_MAX - GAIN_DB_MIN;
        targetDb = clamp(targetDb + modValue * p_gainModulationDepth * range,
            GAIN_DB_MIN, GAIN_DB_MAX);
    }

    m_smoothedGain.SetTargetValue(DbToLinear(targetDb));

    for (int i = 0; i < input.size(); ++i) {
        output[i] = input[i] * m_smoothedGain.GetNextValue();
    }
}

void Gain::SetContextImpl(Context context)
{
    m_smoothedGain.Reset(context.sampleRate, m_smoothingMs);
}

void Gain::DetachModulatorImpl(GainParameter parameter)
{
    switch (parameter) {
        case GainParameter::GAIN_LEVEL:
            m_gainModulator.Detach();
            break;
        default:
            break;
    }
}

void Gain::SetModulationDepthImpl(GainParameter parameter, float depth)
{
    switch (parameter) {
        case GainParameter::GAIN_LEVEL:
            p_gainModulationDepth = depth;
            break;
        default:
            break;
    }
}

#if NEO_PLUGIN_SUPPORT
void Gain::AttachParameterToSourceImpl(GainParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case GainParameter::GAIN_LEVEL:
            p_gainDb.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif

float Gain::DbToLinear(float db)
{
    // 10^(db/20) - standard dB to linear conversion
    return std::pow(10.0f, db * 0.05f);
}
