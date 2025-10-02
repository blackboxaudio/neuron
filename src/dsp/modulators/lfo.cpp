#include "neuron/dsp/modulators/lfo.h"

using namespace neuron;

Lfo::Lfo(float frequency, Waveform waveform)
    : m_wavetable(waveform, frequency, FrequencyRange::MOD)
    , p_frequency(frequency)
    , p_frequencyModulationDepth(0.0f)
{
    SetFrequency(frequency);
}

void Lfo::SetFrequency(float frequency)
{
    p_frequency = frequency;
    m_wavetable.SetFrequency(p_frequency, m_context.sampleRate);
}

void Lfo::SetWaveform(Waveform waveform)
{
    m_wavetable.SetWaveform(waveform);
}

void Lfo::ModulateImpl()
{
    Buffer<float> freqModValues = m_frequencyModulator.GetModulationValues();
    for (int i = 0; i < m_context.blockSize; i++) {
        m_wavetable.GetNextSample(
            m_modulationValues[i],
            freqModValues[i],
            p_frequencyModulationDepth,
            m_context.sampleRate);
    }
}

void Lfo::SetContextImpl(Context context)
{
    SetBufferSize(context.blockSize);
    SetFrequency(p_frequency);
}

void Lfo::DetachModulatorImpl(LfoParameter parameter)
{
    switch (parameter) {
        case LfoParameter::LFO_FREQUENCY:
            m_frequencyModulator.Detach();
            break;
        default:
            break;
    }
}

void Lfo::SetModulationDepthImpl(LfoParameter parameter, float depth)
{
    switch (parameter) {
        case LfoParameter::LFO_FREQUENCY:
            p_frequencyModulationDepth = depth;
            break;
        default:
            break;
    }
}

#if NEO_PLUGIN_SUPPORT
void Lfo::AttachParameterToSourceImpl(LfoParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case LfoParameter::LFO_FREQUENCY:
            p_frequency.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif
