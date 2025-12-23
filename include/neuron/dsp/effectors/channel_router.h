#pragma once

#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"

namespace neuron {

    /**
     * Channel routing modes.
     */
    enum ChannelMode {
        CHANNEL_STEREO = 0,  // Pass through stereo (or sum to mono if enabled)
        CHANNEL_LEFT = 1,    // Output left channel to both outputs
        CHANNEL_RIGHT = 2,   // Output right channel to both outputs
        CHANNEL_SWAP = 3,    // Swap left and right channels
    };

    enum ChannelRouterParameter {
        CHANNEL_ROUTER_MODE,
        CHANNEL_ROUTER_MONO,
        CHANNEL_ROUTER_INVERT_LEFT,
        CHANNEL_ROUTER_INVERT_RIGHT,
    };

    /**
     * The ChannelRouter class handles stereo channel routing including:
     * - Channel mode selection (stereo, left, right, swap)
     * - Mono summing
     * - Per-channel phase inversion
     *
     * This is a stereo effector that processes left and right buffers together.
     */
    class ChannelRouter : public Neuron<ChannelRouter, ChannelRouterParameter> {
    public:
        /**
         * Creates a channel router effector.
         *
         * @param context The DSP context
         */
        explicit ChannelRouter(Context context);

        /**
         * Sets the channel routing mode.
         */
        void SetMode(ChannelMode mode);

        /**
         * Enables or disables mono summing.
         */
        void SetMono(bool mono);

        /**
         * Enables or disables left channel phase inversion.
         */
        void SetInvertLeft(bool invert);

        /**
         * Enables or disables right channel phase inversion.
         */
        void SetInvertRight(bool invert);

        /**
         * Processes stereo buffers in-place.
         *
         * @param left Left channel buffer (modified in place)
         * @param right Right channel buffer (modified in place)
         */
        void Effect(Buffer<Sample>& left, Buffer<Sample>& right);

    protected:
        friend class Neuron<ChannelRouter, ChannelRouterParameter>;
        void SetContextImpl(Context context);
        template<class M>
        void AttachModulatorImpl(ChannelRouterParameter /* parameter */, Modulator<M>* /* modulator */)
        {
            // Channel router parameters are not typically modulated
        }
        void DetachModulatorImpl(ChannelRouterParameter parameter);
        void SetModulationDepthImpl(ChannelRouterParameter parameter, float depth);
#if NEO_PLUGIN_SUPPORT
        void AttachParameterToSourceImpl(ChannelRouterParameter parameter, std::atomic<float>* source);
#endif

    private:
        Parameter<float> p_mode;
        Parameter<float> p_mono;
        Parameter<float> p_invertLeft;
        Parameter<float> p_invertRight;
    };

}
