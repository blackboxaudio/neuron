#pragma once

#include "abstractions/neuron.h"
#include "abstractions/parameter.h"
#include "abstractions/processor.h"

namespace neuron {

    enum class WavefolderParameter {
        INPUT_GAIN,
        THRESHOLD,
        SYMMETRY,
    };

    /**
     * The Wavefolder class applies a wavefolding
     * algorithm to audio signals.
     */
    class Wavefolder : public Processor<Wavefolder>, public Neuron<Wavefolder, WavefolderParameter> {
    public:
        /**
         * Creates a default wavefolder processor.
         */
        Wavefolder();

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

    protected:
        friend class Processor<Wavefolder>;
        Sample ProcessImpl(Sample input);

#ifdef NEO_PLUGIN_SUPPORT
        friend class Neuron<Wavefolder, WavefolderParameter>;
        void AttachParameterToSourceImpl(const WavefolderParameter parameter, std::atomic<float>* source);
#endif

    private:
        Parameter<float> p_inputGain;
        Parameter<float> p_threshold;
        Parameter<float> p_symmetry;
    };

}
