#include "oscillator.h"

using namespace cortex;

Oscillator::Oscillator(Context& context, float frequency)
    : m_context(context)
{
    PopulateWavetable();
    SetFrequency(frequency);
}

Sample Oscillator::Generate()
{
    Sample value = Lerp();
    m_phase = fmod(m_phase + m_phaseIncrement, (float)WAVETABLE_SIZE);
    return value;
}

void Oscillator::SetFrequency(float frequency)
{
    m_phaseIncrement = frequency * (float)WAVETABLE_SIZE / (float)m_context.sampleRate;
}

void Oscillator::PopulateWavetable()
{
    for (size_t idx = 0; idx < WAVETABLE_SIZE; idx++) {
        float phase = (float)idx * PI * 2.0f / (float)WAVETABLE_SIZE;
        Sample value = sin(phase);
        m_wavetable[idx] = value;
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
