#pragma once

#include <stdint.h>
#include <vector>

namespace cortex {
    typedef float Sample;

    template <typename T>
    class Buffer {
        public:
            Buffer(uint32_t capacity);
            ~Buffer();

            void apply(Sample (*fn)(Sample));

            void gain(float factor);

            void zero();

        private:
            std::vector<Sample> m_data;
    };
}
