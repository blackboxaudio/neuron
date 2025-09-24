#pragma once

#include "neuron/core/sample.h"

namespace neuron {

    /**
     * Describes a DSP component that generates a signal without
     * processing an input signal.
     */
    template<class G>
    class Generator {
    public:
        /**
         * Frees any memory allocated by the generator.
         */
        ~Generator() = default;

        /**
         * Generates a buffer of some audio signal, depending
         * on the type of generator, G.
         */
        void Generate(Buffer<Sample>& output)
        {
            return static_cast<G*>(this)->GenerateImpl();
        }
    };

}
