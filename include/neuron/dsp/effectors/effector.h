#pragma once

#include "neuron/core/buffer.h"
#include "neuron/core/sample.h"

namespace neuron {

    /**
     * Describes a neuron that does some processing on
     * an input signal to produce an output signal.
     */
    template<class E>
    class Effector {
    public:
        /**
         * Frees any memory allocated by the effector.
         */
        ~Effector() = default;

        /**
         * Processes a buffer representing a single channel of audio samples.
         */
        void Effect(Buffer<Sample>& input, Buffer<Sample>& output)
        {
            static_cast<E*>(this)->EffectImpl(input, output);
        }
    };

}
