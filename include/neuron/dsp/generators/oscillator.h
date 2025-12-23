#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/dsp/generators/generator.h"
#include "neuron/utils/waveform.h"
#include "neuron/utils/wavetable.h"

namespace neuron {

    enum OscillatorParameter {
        OSC_FREQUENCY,
    };

    /**
     * The Oscillator class creates an audio signal
     * with a basic waveform.
     */
    class Oscillator : public Generator<Oscillator>, public Neuron<Oscillator, OscillatorParameter> {
    public:
        /**
         * Creates an oscillator generator that produces the given waveform at the given frequency.
         */
        explicit Oscillator(Context context, float frequency = 440.0f, Waveform waveform = Waveform::SINE);

        /**
         * Frees any memory allocated by the oscillator.
         */
        ~Oscillator();

        /**
         * Resets the phase of the oscillator, starting it at the beginning
         * waveform position.
         */
        void Reset(float phase = 0.0f);

        /**
         * Sets the frequency of the oscillator.
         */
        void SetFrequency(float frequency);

        /**
         * Sets the waveform of the oscillator.
         */
        void SetWaveform(Waveform waveform);

        /**
         * Attaches a follower oscillator to be synced to this one.
         */
        void AttachFollower(Oscillator* oscillator);

        /**
         * Detaches the follower oscillator from this one.
         */
        void DetachFollower();

    protected:
        friend class Generator<Oscillator>;
        void GenerateImpl(Buffer<Sample>& output);

        friend class Neuron<Oscillator, OscillatorParameter>;
        void SetContextImpl(Context context);

        template<class M>
        void AttachModulatorImpl(OscillatorParameter parameter, Modulator<M>* modulator)
        {
            switch (parameter) {
                case OscillatorParameter::OSC_FREQUENCY:
                    m_frequencyModulator = ModulationSource(modulator);
                    break;
                default:
                    break;
            }
        }

        void DetachModulatorImpl(OscillatorParameter parameter);
        void SetModulationDepthImpl(OscillatorParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(OscillatorParameter parameter, std::atomic<float>* source);
#endif

    private:
        Wavetable m_wavetable;

        Parameter<float> p_frequency;
        Parameter<float> p_frequencyModulationDepth;
        ModulationSource m_frequencyModulator;

        Oscillator* m_follower = nullptr;
    };

}
