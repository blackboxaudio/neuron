#pragma once

#include <cmath>

#include "../audio/context.h"

namespace cortex {
struct AdsrEnvelope {
    float attack;
    float decay;
    float sustain;
    float release;
};

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

class AdsrEnvelopeModulator {
public:
    AdsrEnvelopeModulator(Context& context = DEFAULT_CONTEXT, AdsrEnvelope envelope = DEFAULT_ADSR_ENVELOPE);
    ~AdsrEnvelopeModulator();

    /**
     * Calculates a modulation value to apply to some arbitrary variable.
     */
    float Modulate();

    void Trigger();
    void Release();
    void Reset();

    void SetAttackTime(float attackTimeMs);
    void SetDecayTime(float decayTimeMs);
    void SetSustainLevel(float sustainLevel);
    void SetReleaseTime(float releaseTimeMs);

private:
    void Update(float stageDuration, AdsrStage nextStage);
    void CheckEnvelopePosition(float stageDuration, AdsrStage nextStage);

    Context& m_context;

    AdsrEnvelope m_envelope;
    AdsrStage m_stage = AdsrStage::IDLE;
    size_t m_samplesSinceLastStage = 0;

    // The amount to which the modulation is applied.
    float m_depth;
};
}
