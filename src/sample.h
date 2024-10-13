#pragma once

namespace cortex {
    class Sample {
        public:
            Sample(float value);
            ~Sample();

            float equilibrium();
            float gain(float factor);

        private:
            float m_value;
    };
}