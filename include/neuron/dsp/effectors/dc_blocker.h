#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"
#include "neuron/dsp/effectors/effector.h"

namespace neuron {

    const float DC_BLOCKER_COEFFICIENT_DEFAULT = 0.99f;

    enum DcBlockerParameter {
        DC_BLOCKER_COEFFICIENT,
    };

    /**
     * The DcBlocker class applies a high-pass filter to remove
     * DC offset from audio signals.
     *
     * Uses a first-order high-pass filter: y[n] = x[n] - x[n-1] + r * y[n-1]
     * where r is the coefficient (typically 0.99 for ~5Hz cutoff at 44.1kHz).
     */
    class DcBlocker : public Effector<DcBlocker>, public Neuron<DcBlocker, DcBlockerParameter> {
    public:
        /**
         * Creates a DC blocker effector.
         *
         * @param context The DSP context (sample rate, block size, channels)
         * @param coefficient The filter coefficient (0.99 typical, higher = lower cutoff)
         */
        explicit DcBlocker(Context context, float coefficient = DC_BLOCKER_COEFFICIENT_DEFAULT);

        /**
         * Sets the filter coefficient.
         * Higher values (closer to 1.0) result in a lower cutoff frequency.
         *
         * @param coefficient The coefficient value (typically 0.95-0.999)
         */
        void SetCoefficient(float coefficient);

        /**
         * Resets the filter state. Call this when starting playback
         * or when the audio stream is discontinuous.
         */
        void Reset();

    protected:
        friend class Effector<DcBlocker>;
        void EffectImpl(Buffer<Sample>& input, Buffer<Sample>& output);

        friend class Neuron<DcBlocker, DcBlockerParameter>;
        void SetContextImpl(Context context);
        template<class M>
        void AttachModulatorImpl(DcBlockerParameter /* parameter */, Modulator<M>* /* modulator */)
        {
            // DC blocker coefficient is not typically modulated
        }
        void DetachModulatorImpl(DcBlockerParameter parameter);
        void SetModulationDepthImpl(DcBlockerParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(DcBlockerParameter parameter, std::atomic<float>* source);
#endif

    private:
        Parameter<float> p_coefficient;

        Sample m_xPrev;  // Previous input sample
        Sample m_yPrev;  // Previous output sample
    };

}
