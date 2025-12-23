#include "neuron/dsp/effectors/channel_router.h"

using namespace neuron;

ChannelRouter::ChannelRouter(Context context)
    : p_mode(static_cast<float>(CHANNEL_STEREO))
    , p_mono(0.0f)
    , p_invertLeft(0.0f)
    , p_invertRight(0.0f)
{
    SetContext(context);
}

void ChannelRouter::SetMode(ChannelMode mode)
{
    p_mode = static_cast<float>(mode);
}

void ChannelRouter::SetMono(bool mono)
{
    p_mono = mono ? 1.0f : 0.0f;
}

void ChannelRouter::SetInvertLeft(bool invert)
{
    p_invertLeft = invert ? 1.0f : 0.0f;
}

void ChannelRouter::SetInvertRight(bool invert)
{
    p_invertRight = invert ? 1.0f : 0.0f;
}

void ChannelRouter::Effect(Buffer<Sample>& left, Buffer<Sample>& right)
{
    int mode = static_cast<int>(static_cast<float>(p_mode));
    bool mono = static_cast<float>(p_mono) >= 0.5f;
    float invertL = static_cast<float>(p_invertLeft) >= 0.5f ? -1.0f : 1.0f;
    float invertR = static_cast<float>(p_invertRight) >= 0.5f ? -1.0f : 1.0f;

    for (int i = 0; i < left.size(); ++i) {
        Sample inL = left[i] * invertL;
        Sample inR = right[i] * invertR;
        Sample outL, outR;

        switch (mode) {
            default:
            case CHANNEL_STEREO:
                if (mono) {
                    Sample avg = (inL + inR) * 0.5f;
                    outL = avg;
                    outR = avg;
                } else {
                    outL = inL;
                    outR = inR;
                }
                break;
            case CHANNEL_LEFT:
                outL = inL;
                outR = inL;
                break;
            case CHANNEL_RIGHT:
                outL = inR;
                outR = inR;
                break;
            case CHANNEL_SWAP:
                if (mono) {
                    Sample avg = (inL + inR) * 0.5f;
                    outL = avg;
                    outR = avg;
                } else {
                    outL = inR;
                    outR = inL;
                }
                break;
        }

        left[i] = outL;
        right[i] = outR;
    }
}

void ChannelRouter::SetContextImpl(Context /* context */)
{
    // No context-dependent calculations needed
}

void ChannelRouter::DetachModulatorImpl(ChannelRouterParameter /* parameter */)
{
    // No modulators to detach
}

void ChannelRouter::SetModulationDepthImpl(ChannelRouterParameter /* parameter */, float /* depth */)
{
    // No modulation depth to set
}

#if NEO_PLUGIN_SUPPORT
void ChannelRouter::AttachParameterToSourceImpl(ChannelRouterParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case ChannelRouterParameter::CHANNEL_ROUTER_MODE:
            p_mode.AttachSource(source);
            break;
        case ChannelRouterParameter::CHANNEL_ROUTER_MONO:
            p_mono.AttachSource(source);
            break;
        case ChannelRouterParameter::CHANNEL_ROUTER_INVERT_LEFT:
            p_invertLeft.AttachSource(source);
            break;
        case ChannelRouterParameter::CHANNEL_ROUTER_INVERT_RIGHT:
            p_invertRight.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif
