#pragma once

#include "audio/sample.h"

namespace neuron {

    class Generator {
    public:
        /**
         * Frees any memory allocated by the generator.
         */
        virtual ~Generator() = default;

        /**
         * Generates a sample of some audio signal, depending
         * on the type of generator.
         *
         * @return Sample
         */
        virtual Sample Generate() = 0;
    };

}
