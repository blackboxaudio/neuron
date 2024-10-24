#pragma once

#include "../audio/context.h"

namespace cortex {
/**
 * The configuration of an ADSR envelope curve including durations
 * for the attack, decay, and release stages as well as a sustain level.
 */
struct AdsrEnvelope {
    float attack;
    float decay;
    float sustain;
    float release;
};

/**
 * The stages of an ADSR envelope, including an "idle" stage when not in use.
 */
enum class AdsrStage {
    IDLE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

const AdsrEnvelope DEFAULT_ADSR_ENVELOPE = {
    100.0f, 200.0f, 1.0f, 1000.0f
};

/**
 * The ADsrEnvelopeModulator class is a modulation source
 * that is based off of an ADSR envelope generator.
 */
class AdsrEnvelopeModulator {
public:
    /**
     * Creates an ADSR envelope modulator.
     *
     * @param context The DSP context to be used by the envelope.
     * @param envelope The envelope configuration to initialize the class with.
     * @return AdsrEnvelopeModulator
     */
    AdsrEnvelopeModulator(Context& context = DEFAULT_CONTEXT, AdsrEnvelope envelope = DEFAULT_ADSR_ENVELOPE);

    /**
     * Frees any memory allocated by the modulator.
     */
    ~AdsrEnvelopeModulator();

    /**
     * Calculates a modulation value to apply to some arbitrary variable.
     */
    float Modulate();

    /**
     * Starts the envelope from its attack phase.
     */
    void Trigger();

    /**
     * Starts the envelope from its release phase.
     */
    void Release();

    /**
     * Re-initializes the modulator, ready to be re-triggered.
     */
    void Reset();

    /**
     * Sets the attack time of the envelope.
     *
     * @param attackTimeMs The new attack time for the envelope.
     */
    void SetAttackTime(float attackTimeMs);

    /**
     * Sets the decay time of the envelope.
     *
     * @param decayTimeMs The new decay time for the envelope.
     */
    void SetDecayTime(float decayTimeMs);

    /**
     * Sets the sustain level of the envelope.
     *
     * @param sustainLevel The new sustain level for the envelope.
     */
    void SetSustainLevel(float sustainLevel);

    /**
     * Sets the release time of the envelope.
     *
     * @param releaseTimeMs The new release time for the envelope.
     */
    void SetReleaseTime(float releaseTimeMs);

private:
    // Checks and updates the modulator's state if necessary
    void Update(float stageDuration, AdsrStage nextStage, bool incrementSampleCount);

    Context& m_context;

    AdsrEnvelope m_envelope;
    AdsrStage m_stage = AdsrStage::IDLE;
    size_t m_samplesSinceLastStage = 0;
};
}
