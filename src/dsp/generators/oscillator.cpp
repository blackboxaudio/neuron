#include "neuron/dsp/generators/oscillator.h"

#include <cstddef>

using namespace neuron;

Oscillator::Oscillator(Context& context, float frequency, Waveform waveform)
    : m_context(context)
    , m_waveform(waveform)
    , p_frequency(frequency)
    , p_frequencyModulationDepth(0.0f)
{
    PopulateWavetable();
    SetFrequency(frequency);
}

Oscillator::~Oscillator()
{
    m_follower = nullptr;
}

void Oscillator::GenerateImpl(Buffer<Sample>& output)
{
    auto freqModValues = m_frequencyModulator.GetModulationValues();
    for (int i = 0; i < output.size(); i++) {
        Sample value = Lerp();
        IncrementPhase();
        output[i] = SineToWaveform(value, m_waveform);
    }
}

void Oscillator::SetContextImpl(const Context& context)
{
    m_context = context;
    SetFrequency(p_frequency);
}

template<class M>
void Oscillator::AttachModulatorImpl(OscillatorParameter parameter, Modulator<M>* modulator)
{
    switch (parameter) {
        case OscillatorParameter::OSC_FREQUENCY:
            m_frequencyModulator = ModulationSource(modulator);
            break;
        default:
            break;
    }
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


#ifdef NEO_PLUGIN_SUPPORT
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

void Oscillator::Reset(float phase)
{
    float clampedPhase = clamp(phase, 0.0f, static_cast<float>(WAVETABLE_SIZE));
    m_phase = clampedPhase;
    if (m_follower != nullptr) {
        m_follower->Reset(clampedPhase);
    }
}

void Oscillator::SetFrequency(float frequency)
{
    p_frequency = frequency;
    m_phaseIncrement = p_frequency * static_cast<float>(WAVETABLE_SIZE) / static_cast<float>(m_context.sampleRate);
}

void Oscillator::SetWaveform(Waveform waveform)
{
    m_waveform = waveform;
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

void Oscillator::PopulateWavetable()
{
    for (size_t idx = 0; idx < WAVETABLE_SIZE; idx++) {
        float phase = static_cast<float>(idx) * PI * 2.0f / static_cast<float>(WAVETABLE_SIZE);
        m_wavetable[idx] = sin(phase);
    }
}

void Oscillator::IncrementPhase()
{
    m_phase += m_phaseIncrement;
    if (m_phase >= static_cast<float>(WAVETABLE_SIZE)) {
        m_phase -= static_cast<float>(WAVETABLE_SIZE);
        if (m_follower != nullptr) {
            m_follower->Reset(m_phase);
        }
    }
}

Sample Oscillator::Lerp()
{
    size_t truncatedIdx = m_phase;
    size_t nextIdx = (truncatedIdx + 1) % WAVETABLE_SIZE;
    float nextIdxWeight = m_phase - static_cast<float>(truncatedIdx);
    float truncatedIdxWeight = 1.0f - nextIdxWeight;

    return (m_wavetable[truncatedIdx] * truncatedIdxWeight) + (m_wavetable[nextIdx] * nextIdxWeight);
}
