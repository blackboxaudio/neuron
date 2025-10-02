#pragma once

#include "neuron/utils/arithmetic.h"
#include "neuron/utils/waveform.h"

namespace neuron {

    enum class FrequencyRange {
        MOD, // 0.01Hz to 20Hz
        AUDIO, // 20Hz to Nyquist (usually 22kHz)
    };

    class Wavetable {
    public:
        static constexpr int WAVETABLE_SIZE = 2048;

        explicit Wavetable(Waveform waveform = Waveform::SINE, float frequency = 440.0f, FrequencyRange range = FrequencyRange::AUDIO)
            : m_waveform(waveform)
            , m_baseFrequency(frequency)
            , m_frequencyRange(range)
        {
            PopulateWavetable();
        }

        void SetWaveform(Waveform waveform)
        {
            m_waveform = waveform;
        }

        void SetFrequency(float frequency, float sampleRate)
        {
            m_baseFrequency = frequency;
            m_basePhaseIncrement = frequency * static_cast<float>(WAVETABLE_SIZE) / sampleRate;
        }

        void Reset(float phase = 0.0f)
        {
            m_phase = clamp(phase, 0.0f, 1.0f) * static_cast<float>(WAVETABLE_SIZE);
        }

        bool GetNextSample(Sample& output)
        {
            output = SineToWaveform(InterpolateSample(), m_waveform);

            bool wasCycleCompleted = false;
            m_phase += m_basePhaseIncrement;
            if (m_phase >= static_cast<float>(WAVETABLE_SIZE)) {
                m_phase -= static_cast<float>(WAVETABLE_SIZE);
                wasCycleCompleted = true;
            }

            return wasCycleCompleted;
        }

        bool GetNextSample(Sample& output, float frequencyMod, float modDepth, float sampleRate)
        {
            output = SineToWaveform(InterpolateSample(), m_waveform);

            float modFactor = 1.0f + (frequencyMod * modDepth);
            float modFrequency = ClampFrequency(m_baseFrequency * modFactor, sampleRate);
            float phaseIncrement = modFrequency * static_cast<float>(WAVETABLE_SIZE) / sampleRate;

            bool wasCycleCompleted = false;
            m_phase += phaseIncrement;
            if (m_phase >= static_cast<float>(WAVETABLE_SIZE)) {
                m_phase -= static_cast<float>(WAVETABLE_SIZE);
                wasCycleCompleted = true;
            }

            return wasCycleCompleted;
        }

        float GetPhase() const { return m_phase; }

    private:
        void PopulateWavetable()
        {
            for (int idx = 0; idx < WAVETABLE_SIZE; idx++) {
                float phase = 2.0f * PI * idx / static_cast<float>(WAVETABLE_SIZE);
                m_wavetable[idx] = sin(phase);
            }
        }

        Sample InterpolateSample() const
        {
            int truncatedIndex = static_cast<int>(m_phase);
            int nextIndex = (truncatedIndex + 1) & (WAVETABLE_SIZE - 1);
            float nextIndexWeight = m_phase - static_cast<float>(truncatedIndex);
            float truncatedIndexWeight = 1.0f - nextIndexWeight;

            return m_wavetable[truncatedIndex] * truncatedIndexWeight + m_wavetable[nextIndex] * nextIndexWeight;
        }

        float ClampFrequency(float frequency, float sampleRate)
        {
            float minFreq, maxFreq;

            switch (m_frequencyRange) {
                case FrequencyRange::MOD:
                    minFreq = 0.01f;
                    maxFreq = sampleRate * (1.0f / 256.0f);
                    break;
                case FrequencyRange::AUDIO:
                default:
                    minFreq = 20.0f;
                    maxFreq = std::min(sampleRate / 2.01f, 22000.0f);
                    break;
            }

            return neuron::clamp(frequency, minFreq, maxFreq);
        }

        Sample m_wavetable[WAVETABLE_SIZE];
        Waveform m_waveform;

        float m_phase = 0.0f;
        float m_basePhaseIncrement = 0.0f;
        float m_baseFrequency = 440.0f;

        FrequencyRange m_frequencyRange;
    };

}