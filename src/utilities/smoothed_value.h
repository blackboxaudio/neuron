#pragma once

#include "arithmetic.h"

namespace neuron {
    class SmoothedValue {
    public:
        SmoothedValue() : m_currentValue(0.0f), m_targetValue(0.0f)
        {
            UpdateIncrement();
        }

        SmoothedValue(float initialValue) : m_currentValue(initialValue), m_targetValue(initialValue)
        {
            UpdateIncrement();
        }

        ~SmoothedValue() = default;

        void Reset(double sampleRate, double rampLengthMillis)
        {
            if (sampleRate > 0.0 && rampLengthMillis >= 0.0) {
                m_sampleRate = sampleRate;
                m_rampLengthMillis = rampLengthMillis;
                UpdateIncrement();
            }
        }

        void SetTargetValue(float value) noexcept
        {
            m_targetValue = value;
            UpdateIncrement();
        }

        float GetNextValue() noexcept
        {
            if (isApproximatelyEqual(m_currentValue, m_targetValue)) {
                m_currentValue = m_targetValue;
                return m_currentValue;
            } else {
                m_currentValue += m_increment;
                if ((m_increment > 0.0f && m_currentValue > m_targetValue) || (m_increment < 0.0f && m_currentValue < m_targetValue)) {
                    m_currentValue = m_targetValue;
                }
                return m_currentValue;
            }
        }

        void Skip(int numSamples)
        {
            if (isApproximatelyEqual(m_currentValue, m_targetValue)) {
                m_currentValue = m_targetValue;
                return;
            }

            float newValue = m_currentValue + m_increment * numSamples;
            if ((m_increment > 0.0f && newValue > m_targetValue) || (m_increment < 0.0f && newValue < m_targetValue)) {
                m_currentValue = m_targetValue;
            } else {
                m_currentValue = newValue;
            }
        }

    private:
        static constexpr float INV_1000 = 1.0f / 1000.0f;

        void UpdateIncrement() noexcept
        {
            if (m_rampLengthMillis <= 0.0) {
                m_currentValue = m_targetValue;
                m_increment = 0.0f;
                return;
            }

            float numSamplesBetweenValues = static_cast<float>(m_rampLengthMillis * m_sampleRate) * INV_1000;
            m_increment = (m_targetValue - m_currentValue) / numSamplesBetweenValues;
        }

        double m_sampleRate = 44100.0;
        double m_rampLengthMillis = 50;

        float m_currentValue;
        float m_targetValue;
        float m_increment = 0.0f;
    };

}
