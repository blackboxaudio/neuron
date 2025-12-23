#include "neuron/dsp/generators/oscillator.h"

using namespace neuron;

Oscillator::Oscillator(Context context, float frequency, Waveform waveform)
    : m_wavetable(waveform, frequency, FrequencyRange::AUDIO)
    , p_frequency(frequency)
    , p_frequencyModulationDepth(0.0f)
{
    SetContext(context);
}

Oscillator::~Oscillator()
{
    m_follower = nullptr;
}

void Oscillator::Reset(float phase)
{
    m_wavetable.Reset(phase);
    if (m_follower != nullptr) {
        m_follower->Reset(phase);
    }
}

void Oscillator::SetFrequency(float frequency)
{
    p_frequency = frequency;
    m_wavetable.SetFrequency(frequency, m_context.sampleRate);
}

void Oscillator::SetWaveform(Waveform waveform)
{
    m_wavetable.SetWaveform(waveform);
}

void Oscillator::AttachFollower(Oscillator* follower)
{
    if (follower != nullptr && follower != this) {
        m_follower = follower;
    }
}

void Oscillator::DetachFollower()
{
    m_follower = nullptr;
}

void Oscillator::GenerateImpl(Buffer<Sample>& output)
{
    float freqModValue = m_frequencyModulator.GetModulationValue();
    for (int i = 0; i < output.size(); i++) {
        bool wasCycleCompleted = m_wavetable.GetNextSample(
            output[i],
            freqModValue,
            p_frequencyModulationDepth,
            m_context.sampleRate);
        if (wasCycleCompleted && m_follower != nullptr) {
            m_follower->Reset(m_wavetable.GetPhase());
        }
    }
}

void Oscillator::SetContextImpl(Context /* context */)
{
    SetFrequency(p_frequency);
}

void Oscillator::DetachModulatorImpl(OscillatorParameter parameter)
{
    switch (parameter) {
        case OscillatorParameter::OSC_FREQUENCY:
            m_frequencyModulator.Detach();
            break;
        default:
            break;
    }
}

void Oscillator::SetModulationDepthImpl(OscillatorParameter parameter, float depth)
{
    switch (parameter) {
        case OscillatorParameter::OSC_FREQUENCY:
            p_frequencyModulationDepth = depth;
            break;
        default:
            break;
    }
}

#if NEO_PLUGIN_SUPPORT
void Oscillator::AttachParameterToSourceImpl(OscillatorParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case OscillatorParameter::OSC_FREQUENCY:
            p_frequency.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif
