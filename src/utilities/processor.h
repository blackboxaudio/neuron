#pragma once

#include "audio/sample.h"

namespace neuron {

    class Processor {
    public:
        /**
         * Frees any memory allocated by the processor.
         */
        virtual ~Processor() = default;

        /**
         * Processes a sample of some audio signal.
         *
         * @return Sample
         */
        virtual Sample Process(Sample input) = 0;
    };

}
