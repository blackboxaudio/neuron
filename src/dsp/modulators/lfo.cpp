#include "neuron/dsp/modulators/lfo.h"

using namespace neuron;

Lfo::Lfo(Context context, float frequency, Waveform waveform)
    : m_wavetable(waveform, frequency, FrequencyRange::MOD)
    , p_frequency(frequency)
    , p_frequencyModulationDepth(0.0f)
{
    SetContext(context);
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
    float freqModValue = m_frequencyModulator.GetModulationValue();
    m_wavetable.GetNextSample(m_modulationValue, freqModValue, p_frequencyModulationDepth, m_context.sampleRate);
    m_wavetable.AdvancePhaseByBlock(freqModValue, p_frequencyModulationDepth, m_context.sampleRate, m_context.blockSize - 1);
}

void Lfo::SetContextImpl(Context context)
{
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
