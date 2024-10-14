#pragma once

#include "cortex.h"

namespace cortex {
    const size_t WAVETABLE_SIZE = 256;

    class Oscillator {
        public:
            Oscillator(Context& context, float frequency);
            ~Oscillator() {}

            void Process(OutputBuffer output, size_t size);

            void SetFrequency(float frequency);

        private:
            void PopulateWavetable();
            Sample Lerp();

            Context& m_context;
            Sample m_wavetable[WAVETABLE_SIZE];
            float m_phase = 0.0f;
            float m_phaseIncrement = 0.0f;
    };
}
