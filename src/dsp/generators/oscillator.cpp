#include "neuron/dsp/generators/oscillator.h"

#include <cstddef>

using namespace neuron;

Oscillator::Oscillator(Context& context, float frequency, Waveform waveform)
    : m_context(context)
    , m_waveform(waveform)
    , p_frequency(frequency)
{
    PopulateWavetable();
    SetFrequency(frequency);
}

Oscillator::~Oscillator()
{
    m_follower = nullptr;
}

Sample Oscillator::GenerateImpl()
{
    Sample value = Lerp();

    IncrementPhase();

    return SineToWaveform(value, m_waveform);
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
    float clampedPhase = clamp(phase, 0.0f, (float)WAVETABLE_SIZE);
    m_phase = clampedPhase;
    if (m_follower != nullptr) {
        m_follower->Reset(clampedPhase);
    }
}

void Oscillator::SetFrequency(float frequency)
{
    p_frequency = frequency;
    m_phaseIncrement = p_frequency * (float)WAVETABLE_SIZE / (float)m_context.sampleRate;
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
        float phase = (float)idx * PI * 2.0f / (float)WAVETABLE_SIZE;
        m_wavetable[idx] = sin(phase);
    }
}

void Oscillator::IncrementPhase()
{
    m_phase += m_phaseIncrement;
    if (m_phase >= (float)WAVETABLE_SIZE) {
        m_phase -= (float)WAVETABLE_SIZE;
        if (m_follower != nullptr) {
            m_follower->Reset(m_phase);
        }
    }
}

Sample Oscillator::Lerp()
{
    size_t truncatedIdx = m_phase;
    size_t nextIdx = (truncatedIdx + 1) % WAVETABLE_SIZE;
    float nextIdxWeight = m_phase - (float)truncatedIdx;
    float truncatedIdxWeight = 1.0f - nextIdxWeight;

    return (m_wavetable[truncatedIdx] * truncatedIdxWeight) + (m_wavetable[nextIdx] * nextIdxWeight);
}
