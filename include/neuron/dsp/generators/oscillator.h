#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/dsp/generators/generator.h"
#include "neuron/utils/waveform.h"

namespace neuron {

    const size_t WAVETABLE_SIZE = 256;

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
         * Creates an oscillator generator.
         *
         * @param context The DSP context to be used by the oscillator.
         * @param frequency The initial frequency of the oscillator.
         * @return Oscillator
         */
        explicit Oscillator(Context& context = DEFAULT_CONTEXT, float frequency = 440.0f, Waveform waveform = Waveform::SINE);

        /**
         * Frees any memory allocated by the oscillator.
         */
        ~Oscillator();

        /**
         * Resets the phase of the oscillator, starting it at the beginning
         * waveform position.
         *
         * @param
         */
        void Reset(float phase = 0.0f);

        /**
         * Sets the frequency of the oscillator.
         *
         * @param frequency The new oscillator output frequency.
         */
        void SetFrequency(float frequency);

        /**
         * Sets the waveform of the oscillator.
         *
         * @param waveform The new oscillator output waveform.
         */
        void SetWaveform(Waveform waveform);

        /**
         * Attaches a follower oscillator to be synced to this one.
         *
         * @param oscillator The oscillator that will be synced to this one.
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
        void SetContextImpl(const Context& context);
        template<class M>
        void AttachModulatorImpl(OscillatorParameter parameter, Modulator<M>* modulator);
        void DetachModulatorImpl(OscillatorParameter parameter);
        void SetModulationDepthImpl(OscillatorParameter parameter, float depth);
#ifdef NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(OscillatorParameter parameter, std::atomic<float>* source);
#endif

    private:
        void PopulateWavetable();
        void IncrementPhase();
        Sample Lerp();

        Context& m_context;

        Sample m_wavetable[WAVETABLE_SIZE];
        Waveform m_waveform;

        Parameter<float> p_frequency;
        Parameter<float> p_frequencyModulationDepth;
        ModulationSource m_frequencyModulator;

        float m_phase = 0.0f;
        float m_phaseIncrement = 0.0f;

        Oscillator* m_follower = nullptr;
    };

}
