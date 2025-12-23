#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"
#include "neuron/utils/smoothed_value.h"

namespace neuron {

    const float PANNER_POSITION_MIN = -100.0f;
    const float PANNER_POSITION_MAX = 100.0f;
    const float PANNER_SMOOTHING_MS_DEFAULT = 20.0f;

    enum PannerParameter {
        PANNER_POSITION,  // -100 (full left) to +100 (full right)
    };

    /**
     * The Panner class applies equal-power stereo panning with smoothing.
     *
     * Uses cos/sin law for equal-power panning:
     * - left *= cos(angle)
     * - right *= sin(angle)
     *
     * Where angle ranges from 0 (full left) to pi/2 (full right).
     *
     * This is a stereo effector that processes left and right buffers together.
     */
    class Panner : public Neuron<Panner, PannerParameter> {
    public:
        /**
         * Creates a panner effector.
         *
         * @param context The DSP context
         * @param smoothingMs Smoothing time in milliseconds (default 20ms)
         */
        explicit Panner(Context context, float smoothingMs = PANNER_SMOOTHING_MS_DEFAULT);

        /**
         * Sets the pan position.
         *
         * @param position Pan position from -100 (full left) to +100 (full right)
         */
        void SetPosition(float position);

        /**
         * Sets the smoothing time.
         *
         * @param ms Smoothing time in milliseconds
         */
        void SetSmoothingTime(float ms);

        /**
         * Applies panning to stereo buffers in-place.
         *
         * @param left Left channel buffer (modified in place)
         * @param right Right channel buffer (modified in place)
         */
        void Effect(Buffer<Sample>& left, Buffer<Sample>& right);

    protected:
        friend class Neuron<Panner, PannerParameter>;
        void SetContextImpl(Context context);
        template<class M>
        void AttachModulatorImpl(PannerParameter parameter, Modulator<M>* modulator)
        {
            switch (parameter) {
                case PannerParameter::PANNER_POSITION:
                    m_positionModulator = ModulationSource(modulator);
                    break;
                default:
                    break;
            }
        }
        void DetachModulatorImpl(PannerParameter parameter);
        void SetModulationDepthImpl(PannerParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(PannerParameter parameter, std::atomic<float>* source);
#endif

    private:
        Parameter<float> p_position;
        Parameter<float> p_positionModulationDepth;
        ModulationSource m_positionModulator;

        LinearSmoothedValue m_smoothedAngle;
        float m_smoothingMs;
    };

}
