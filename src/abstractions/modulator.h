#pragma once

namespace neuron {

    /**
     * Describes a DSP component that produces a stream of data
     * that changes the parameter of another DSP component over time.
     */
    template<class M>
    class Modulator {
    public:
        /**
         * Frees any memory allocated by the modulator.
         */
        ~Modulator() = default;

        /**
         * Creates a modulation value to be used elsewhere.
         *
         * @return float
         */
        float Modulate()
        {
            return static_cast<M*>(this)->ModulateImpl();
        }
    };

}
