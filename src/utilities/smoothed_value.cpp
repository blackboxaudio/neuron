#include "smoothed_value.h"
#include "arithmetic.h"

using namespace neuron;

SmoothedValue::SmoothedValue()
    : m_currentValue(0.0f)
    , m_targetValue(0.0f)
{
    UpdateIncrement();
}

SmoothedValue::SmoothedValue(float initialValue)
{
    m_currentValue = initialValue;
    m_targetValue = initialValue;
    UpdateIncrement();
}

void SmoothedValue::Reset(double sampleRate, double rampLengthMillis)
{
    if (sampleRate > 0.0 && rampLengthMillis >= 0.0) {
        m_sampleRate = sampleRate;
        m_rampLengthMillis = rampLengthMillis;
        UpdateIncrement();
    }
}

void SmoothedValue::SetTargetValue(float value)
{
    m_targetValue = value;
    UpdateIncrement();
}

float SmoothedValue::GetNextValue()
{
    if (neuron::isApproximatelyEqual(m_currentValue, m_targetValue)) {
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

void SmoothedValue::Skip(int numSamples)
{
    if (isApproximatelyEqual(m_currentValue, m_targetValue)) {
        m_currentValue = m_targetValue;
        return;
    }

    float newValue = m_currentValue + m_increment * numSamples;
    if ((m_increment > 0.0f && newValue > m_targetValue) || (m_increment < 0.0f && newValue < m_targetValue)) {
        m_currentValue = newValue;
    } else {
        m_currentValue = newValue;
    }
}

void SmoothedValue::UpdateIncrement()
{
    if (m_rampLengthMillis <= 0.0) {
        m_currentValue = m_targetValue;
        m_increment = 0.0f;
        return;
    }

    float numSamplesBetweenValues = static_cast<float>(m_rampLengthMillis) * static_cast<float>(m_sampleRate) / 1000.0f;
    m_increment = (m_targetValue - m_currentValue) / numSamplesBetweenValues;
}
