#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"
#include "neuron/dsp/processors/processor.h"

namespace neuron {

    enum SaturatorParameter {
        SATURATOR_SATURATION,
        SATURATOR_SYMMETRY,
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
        void ProcessImpl(Buffer<Sample>& input, Buffer<Sample>& output);

#ifdef NEO_PLUGIN_SUPPORT
        friend class Neuron<Saturator, SaturatorParameter>;
        void AttachParameterToSourceImpl(SaturatorParameter parameter, std::atomic<float>* source);
#endif
    };

}