#pragma once

#include "neuron/core/base.h"
#include "neuron/core/parameter.h"
#include "neuron/dsp/modulators/modulator.h"
#include "neuron/utils/waveform.h"
#include "neuron/utils/wavetable.h"

namespace neuron {

    enum LfoParameter {
        LFO_FREQUENCY,
    };

    class Lfo : public Modulator<Lfo>, public Neuron<Lfo, LfoParameter> {
    public:
        explicit Lfo(float frequency = 1.0f, Waveform waveform = Waveform::SINE);

        ~Lfo() = default;

        void SetFrequency(float frequency);
        void SetWaveform(Waveform waveform);

    protected:
        friend class Modulator<Lfo>;
        void ModulateImpl();

        friend class Neuron<Lfo, LfoParameter>;
        void SetContextImpl(Context context);

        template<class M>
        void AttachModulatorImpl(LfoParameter parameter, Modulator<M>* modulator)
        {
            switch (parameter) {
                case LfoParameter::LFO_FREQUENCY:
                    m_frequencyModulator = ModulationSource(modulator);
                break;
                default:
                    break;
            }
        }

        void DetachModulatorImpl(LfoParameter parameter);
        void SetModulationDepthImpl(LfoParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(LfoParameter parameter, std::atomic<float>* source);
#endif

    private:
        Wavetable m_wavetable;

        Parameter<float> p_frequency;
        Parameter<float> p_frequencyModulationDepth;
        ModulationSource m_frequencyModulator;
    };

}
