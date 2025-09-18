#pragma once

namespace neuron {
    struct SmoothedValueConfig {
        double sampleRate;
        double rampLengthMillis;
    };

    class SmoothedValue {
        public:
            SmoothedValue();
            SmoothedValue(float initialValue);
            ~SmoothedValue() = default;

            void Reset(double sampleRate, double rampLengthMillis);

            void SetTargetValue(float value);
            float GetNextValue();

            void Skip(int numSamples);

        private:
            void UpdateIncrement();

            double m_sampleRate = 44100.0;
            double m_rampLengthMillis = 50;

            float m_currentValue;
            float m_targetValue;

            float m_increment = 0.0f;
    };

}
