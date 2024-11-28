#pragma once

#include "audio/sample.h"

namespace neuron {

    /**
     * Describes a DSP component that does some processing on
     * an input signal to produce an output signal.
     */
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
