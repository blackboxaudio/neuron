#include "oscillator.h"

using namespace cortex;

Oscillator::Oscillator(Context& context, float frequency, Waveform waveform)
    : m_context(context)
    , m_waveform(waveform)
{
    PopulateWavetable();
    SetFrequency(frequency);
}

Oscillator::~Oscillator()
{
    m_follower = nullptr;
}

Sample Oscillator::Generate()
{
    Sample value = Lerp();

    IncrementPhase();

    return SineToWaveform(value, m_waveform);
}

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
    m_phaseIncrement = frequency * (float)WAVETABLE_SIZE / (float)m_context.sampleRate;
}

void Oscillator::SetWaveform(Waveform waveform)
{
    m_waveform = waveform;
}

void Oscillator::AttachFollower(Oscillator* follower)
{
    if (follower != nullptr) {
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
        Sample value = sin(phase);
        m_wavetable[idx] = value;
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
    size_t truncatedIdx = (size_t)m_phase;
    size_t nextIdx = (truncatedIdx + 1) % WAVETABLE_SIZE;
    float nextIdxWeight = m_phase - (float)truncatedIdx;
    float truncatedIdxWeight = 1.0f - nextIdxWeight;

    return (m_wavetable[truncatedIdx] * truncatedIdxWeight) + (m_wavetable[nextIdx] * nextIdxWeight);
}
