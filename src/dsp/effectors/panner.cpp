#include "neuron/dsp/effectors/panner.h"
#include "neuron/utils/arithmetic.h"

#include <cmath>

using namespace neuron;

Panner::Panner(Context context, float smoothingMs)
    : p_position(0.0f)
    , p_positionModulationDepth(0.0f)
    , m_smoothingMs(smoothingMs)
{
    SetContext(context);
}

void Panner::SetPosition(float position)
{
    p_position = clamp(position, PANNER_POSITION_MIN, PANNER_POSITION_MAX);
}

void Panner::SetSmoothingTime(float ms)
{
    m_smoothingMs = ms;
    m_smoothedAngle.Reset(m_context.sampleRate, m_smoothingMs);
}

void Panner::Effect(Buffer<Sample>& left, Buffer<Sample>& right)
{
    float targetPos = p_position;

    // Apply modulation if present
    if (m_positionModulator.IsValid()) {
        float modValue = m_positionModulator.GetModulationValue();
        float range = PANNER_POSITION_MAX - PANNER_POSITION_MIN;
        targetPos = clamp(targetPos + modValue * p_positionModulationDepth * range,
            PANNER_POSITION_MIN, PANNER_POSITION_MAX);
    }

    // Convert position (-100 to +100) to angle (0 to pi/2)
    // -100 = 0 (full left: cos=1, sin=0)
    // 0 = pi/4 (center: cos=sin=0.707)
    // +100 = pi/2 (full right: cos=0, sin=1)
    float targetAngle = ((targetPos + 100.0f) / 200.0f) * PI * 0.5f;
    m_smoothedAngle.SetTargetValue(targetAngle);

    for (int i = 0; i < left.size(); ++i) {
        float angle = m_smoothedAngle.GetNextValue();
        left[i] *= std::cos(angle);
        right[i] *= std::sin(angle);
    }
}

void Panner::SetContextImpl(Context context)
{
    m_smoothedAngle.Reset(context.sampleRate, m_smoothingMs);
}

void Panner::DetachModulatorImpl(PannerParameter parameter)
{
    switch (parameter) {
        case PannerParameter::PANNER_POSITION:
            m_positionModulator.Detach();
            break;
        default:
            break;
    }
}

void Panner::SetModulationDepthImpl(PannerParameter parameter, float depth)
{
    switch (parameter) {
        case PannerParameter::PANNER_POSITION:
            p_positionModulationDepth = depth;
            break;
        default:
            break;
    }
}

#if NEO_PLUGIN_SUPPORT
void Panner::AttachParameterToSourceImpl(PannerParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case PannerParameter::PANNER_POSITION:
            p_position.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif
