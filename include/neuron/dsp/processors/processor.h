#pragma once

#include "neuron/core/sample.h"

namespace neuron {

    /**
     * Describes a DSP component that does some processing on
     * an input signal to produce an output signal.
     */
    template<class P>
    class Processor {
    public:
        /**
         * Frees any memory allocated by the processor.
         */
        ~Processor() = default;

        /**
         * Processes a sample of some audio signal.
         *
         * @return Sample
         */
        Sample Process(Sample input)
        {
            return static_cast<P*>(this)->ProcessImpl(input);
        }
    };

}
