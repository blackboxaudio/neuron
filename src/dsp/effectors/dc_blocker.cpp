#include "neuron/dsp/effectors/dc_blocker.h"
#include "neuron/utils/arithmetic.h"

using namespace neuron;

DcBlocker::DcBlocker(Context context, float coefficient)
    : p_coefficient(coefficient)
    , m_xPrev(0.0f)
    , m_yPrev(0.0f)
{
    SetContext(context);
}

void DcBlocker::SetCoefficient(float coefficient)
{
    p_coefficient = clamp(coefficient, 0.9f, 0.9999f);
}

void DcBlocker::Reset()
{
    m_xPrev = 0.0f;
    m_yPrev = 0.0f;
}

void DcBlocker::EffectImpl(Buffer<Sample>& input, Buffer<Sample>& output)
{
    float r = p_coefficient;

    for (int i = 0; i < input.size(); ++i) {
        Sample x = input[i];
        Sample y = x - m_xPrev + r * m_yPrev;
        m_xPrev = x;
        m_yPrev = y;
        output[i] = y;
    }
}

void DcBlocker::SetContextImpl(Context /* context */)
{
    // No context-dependent calculations needed
}

void DcBlocker::DetachModulatorImpl(DcBlockerParameter /* parameter */)
{
    // No modulators to detach
}

void DcBlocker::SetModulationDepthImpl(DcBlockerParameter /* parameter */, float /* depth */)
{
    // No modulation depth to set
}

#if NEO_PLUGIN_SUPPORT
void DcBlocker::AttachParameterToSourceImpl(DcBlockerParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case DcBlockerParameter::DC_BLOCKER_COEFFICIENT:
            p_coefficient.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif
