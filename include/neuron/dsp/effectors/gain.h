#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"
#include "neuron/dsp/effectors/effector.h"
#include "neuron/utils/smoothed_value.h"

namespace neuron {

    const float GAIN_DB_MIN = -60.0f;
    const float GAIN_DB_MAX = 30.0f;
    const float GAIN_SMOOTHING_MS_DEFAULT = 20.0f;

    enum GainParameter {
        GAIN_LEVEL,  // Gain level in dB
    };

    /**
     * The Gain class applies gain with smoothing to prevent clicks.
     *
     * Uses decibel input with linear smoothing for smooth gain changes.
     */
    class Gain : public Effector<Gain>, public Neuron<Gain, GainParameter> {
    public:
        /**
         * Creates a gain effector.
         *
         * @param context The DSP context
         * @param smoothingMs Smoothing time in milliseconds (default 20ms)
         */
        explicit Gain(Context context, float smoothingMs = GAIN_SMOOTHING_MS_DEFAULT);

        /**
         * Sets the gain level in decibels.
         *
         * @param db Gain level in dB
         */
        void SetGainDb(float db);

        /**
         * Sets the smoothing time.
         *
         * @param ms Smoothing time in milliseconds
         */
        void SetSmoothingTime(float ms);

    protected:
        friend class Effector<Gain>;
        void EffectImpl(Buffer<Sample>& input, Buffer<Sample>& output);

        friend class Neuron<Gain, GainParameter>;
        void SetContextImpl(Context context);
        template<class M>
        void AttachModulatorImpl(GainParameter parameter, Modulator<M>* modulator)
        {
            switch (parameter) {
                case GainParameter::GAIN_LEVEL:
                    m_gainModulator = ModulationSource(modulator);
                    break;
                default:
                    break;
            }
        }
        void DetachModulatorImpl(GainParameter parameter);
        void SetModulationDepthImpl(GainParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(GainParameter parameter, std::atomic<float>* source);
#endif

    private:
        static float DbToLinear(float db);

        Parameter<float> p_gainDb;
        Parameter<float> p_gainModulationDepth;
        ModulationSource m_gainModulator;

        LinearSmoothedValue m_smoothedGain;
        float m_smoothingMs;
    };

}
