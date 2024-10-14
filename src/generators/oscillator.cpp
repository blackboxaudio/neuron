#include "oscillator.h"

using namespace cortex;

Oscillator::Oscillator(Context &context, float frequency = 110.0f) : m_context(context) {
    PopulateWavetable();
    SetFrequency(frequency);
}

void Oscillator::Process(OutputBuffer output, size_t size) {
    for (int sampleIdx = 0; sampleIdx < size; sampleIdx++) {
        Sample value = Lerp();
        m_phase = fmod(m_phase + m_phaseIncrement, (float) WAVETABLE_SIZE);

        for (int channelIdx = 0; channelIdx < m_context.numChannels; channelIdx++) {
            output[channelIdx][sampleIdx] = value;
        }
    }
}

void Oscillator::SetFrequency(float frequency) {
    m_phaseIncrement = frequency * (float) WAVETABLE_SIZE / (float) m_context.sampleRate;
}

void Oscillator::PopulateWavetable() {
    for (int idx = 0; idx < WAVETABLE_SIZE; idx++) {
        float phase = (float) idx * PI * 2.0f / (float) WAVETABLE_SIZE;
        Sample value = sin(phase);
        m_wavetable[idx] = value;
    }
}

Sample Oscillator::Lerp() {
    size_t truncatedIdx = (size_t) m_phase;
    size_t nextIdx = (truncatedIdx + 1) % WAVETABLE_SIZE;
    float nextIdxWeight = m_phase - (float) truncatedIdx;
    float truncatedIdxWeight = 1.0f - nextIdxWeight;

    return (m_wavetable[truncatedIdx] * truncatedIdxWeight) + (m_wavetable[nextIdx] * nextIdxWeight);
}
