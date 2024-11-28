#pragma once

namespace neuron {

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
