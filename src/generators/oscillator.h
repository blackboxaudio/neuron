#pragma once

#include "cortex.h"

namespace cortex {
    const size_t WAVETABLE_SIZE = 256;

    class Oscillator {
        public:
            Oscillator(Context& context, float frequency);
            ~Oscillator() {}

            SampleType Generate();

            void SetFrequency(float frequency);

        private:
            void PopulateWavetable();
            SampleType Lerp();

            Context& m_context;
            SampleType m_wavetable[WAVETABLE_SIZE];
            float m_phase = 0.0f;
            float m_phaseIncrement = 0.0f;
    };
}
