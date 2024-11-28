#pragma once

#include "audio/sample.h"
#include "utilities/arithmetic.h"
#include "utilities/parameter.h"
#include "utilities/processor.h"

namespace neuron {

    /**
     * The Saturator class applies a tape saturation
     * algorithm to audio signals.
     */
    class Saturator : public Processor {
    public:
        /**
         * Creates a default saturator processor.
         *
         * @return Saturator
         */
        Saturator();

        /**
         * Frees any memory allocated by the saturator.
         */
        ~Saturator() override;

        /**
         * Applies a saturation algorithm to an input sample.
         *
         * @param input The input sample to be processed.
         * @return Sample
         */
        Sample Process(Sample input) override;

        /**
         * Sets the saturation level, which boosts the signal before
         * distortion is applied. This multiplier will always be greater than
         * one.
         *
         * @param saturation The multiplier of the audio signal going into the
         * distortion algorithm.
         */
        void SetSaturation(float saturation);

        /**
         * Sets the symmetry of the algorithm, determining how much
         * distortion to apply to the positive and negative parts
         * of the signal separately.
         *
         * @param symmetry A value between 0.0 and 1.0, ranging from asymmetrical
         * (one-sided) to symmetrical respectively.
         */
        void SetSymmetry(float symmetry);

        Parameter<float> p_saturation;
        Parameter<float> p_symmetry;
    };

}