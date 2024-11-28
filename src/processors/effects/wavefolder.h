#pragma once

#include "audio/sample.h"
#include "utilities/arithmetic.h"
#include "utilities/parameter.h"
#include "utilities/processor.h"

namespace neuron {

    /**
     * The Wavefolder class applies a wavefolding
     * algorithm to audio signals.
     */
    class Wavefolder : public Processor {
    public:
        /**
         * Creates a default wavefolder processor.
         */
        Wavefolder();

        /**
         * Frees any memory allocated by the wavefolder.
         */
        ~Wavefolder() override = default;

        /**
         * Applies a wavefolding algorithm to an input sample.
         *
         * @param input The input sample to be processed.
         * @return Sample
         */
        Sample Process(Sample input) override;

        /**
         * Sets the input gain level, which boosts the signal before
         * being measured against the wavefolder threshold.
         *
         * @param gain The multiplier of the audio signal going into the
         * wavefolding algorithm.
         */
        void SetInputGain(float gain);

        /**
         * Sets the threshold of the wavefolder, above which samples will
         * be "folded" toawrds zero until they are within the threshold.
         */
        void SetThreshold(float threshold);

        /**
         * Sets the symmetry of the algorithm, determining how much
         * wavefolding to apply to the positive and negative parts
         * of the signal separately.
         *
         * @param symmetry A value between 0.0 and 1.0, ranging from asymmetrical
         * (one-sided) to symmetrical respectively.
         */
        void SetSymmetry(float symmetry);

        Parameter<float> p_inputGain;
        Parameter<float> p_threshold;
        Parameter<float> p_symmetry;
    };

}
