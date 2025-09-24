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
         * Processes a buffer representing a single channel of audio samples.
         */
        void Process(Buffer<Sample>& input, Buffer<Sample>& output)
        {
            static_cast<P*>(this)->ProcessImpl(input, output);
        }
    };

}
