#pragma once

#include "abstractions/neuron.h"
#include "abstractions/parameter.h"
#include "abstractions/processor.h"
#include "audio/sample.h"

namespace neuron {

    enum class SaturatorParameter {
        SATURATION,
        SYMMETRY,
    };

    /**
     * The Saturator class applies a tape saturation
     * algorithm to audio signals.
     */
    class Saturator : public Processor<Saturator>, public Neuron<Saturator, SaturatorParameter> {
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
        ~Saturator() = default;

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

    protected:
        friend class Processor<Saturator>;
        Sample ProcessImpl(Sample input);

#ifdef NEO_USE_STD_ATOMIC
        friend class Neuron<Saturator, SaturatorParameter>;
        void AttachParameterImpl(SaturatorParameter parameter, std::atomic<float>* source);
#endif
    };

}