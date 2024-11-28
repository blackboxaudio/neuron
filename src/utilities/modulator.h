#pragma once

namespace neuron {

    /**
     * Describes a DSP component that produces a stream of data
     * that changes the parameter of another DSP component over time.
     */
    class Modulator {
    public:
        /**
         * Frees any memory allocated by the modulator.
         */
        virtual ~Modulator() = default;

        /**
         * Creates a modulation value to be used elsewhere.
         *
         * @return float
         */
        virtual float Modulate() = 0;
    };

}
