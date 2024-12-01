#include "modulators/adsr.h"

using namespace neuron;

AdsrEnvelopeModulator::AdsrEnvelopeModulator(Context& context, AdsrEnvelope envelope)
    : m_context(context)
    , p_attack(envelope.attack)
    , p_decay(envelope.decay)
    , p_sustain(envelope.sustain)
    , p_release(envelope.release)
{
}

float AdsrEnvelopeModulator::ModulateImpl()
{
    float position = (float)m_samplesSinceLastStage * (1000.0f / m_context.sampleRate);

    /**
     * NOTE: The modulation value is calculated based on the current stage of the modulator.
     * Based on the stage of the envelope, the calculation of the modulation value is made from a
     * particular linear equation. The x position as input for this linear equation is calculated
     * from the number of samples generated since the stage was last updated multiplied by the number
     * of milliseconds per sample. If the x position is greater than the stage's duration, then the
     * modulator is updated to the next stage.
     */
    float value;
    switch (m_stage) {
        case AdsrStage::ATTACK:
            value = position / p_attack;
            Update(p_attack, AdsrStage::DECAY, true);
            break;
        case AdsrStage::DECAY:
            value = (((p_sustain - 1.0f) / p_decay) * position) + 1.0f;
            Update(p_decay, AdsrStage::SUSTAIN, true);
            break;
        case AdsrStage::SUSTAIN:
            value = p_sustain;
            break;
        case AdsrStage::RELEASE:
            value = ((-p_sustain / p_release) * position) + p_sustain;
            Update(p_release, AdsrStage::IDLE, true);
            break;
        case AdsrStage::IDLE:
        default:
            value = 0.0f;
    }

    return value;
}

#ifdef NEO_PLUGIN_SUPPORT
void AdsrEnvelopeModulator::AttachParameterToSourceImpl(AdsrParameter parameter, std::atomic<float>* source)
{
    switch (parameter) {
        case AdsrParameter::ATTACK:
            p_attack.AttachSource(source);
            break;
        case AdsrParameter::DECAY:
            p_decay.AttachSource(source);
            break;
        case AdsrParameter::SUSTAIN:
            p_sustain.AttachSource(source);
            break;
        case AdsrParameter::RELEASE:
            p_release.AttachSource(source);
            break;
        default:
            break;
    }
}
#endif

void AdsrEnvelopeModulator::Trigger()
{
    m_stage = AdsrStage::ATTACK;
    m_samplesSinceLastStage = 0;
}

void AdsrEnvelopeModulator::Release()
{
    m_stage = AdsrStage::RELEASE;
    m_samplesSinceLastStage = 0;
}

void AdsrEnvelopeModulator::Reset()
{
    m_stage = AdsrStage::IDLE;
    m_samplesSinceLastStage = 0;
}

void AdsrEnvelopeModulator::SetAttackTime(float attackTimeMs)
{
    p_attack = attackTimeMs;
    Update(p_attack, AdsrStage::DECAY, false);
}

void AdsrEnvelopeModulator::SetDecayTime(float decayTimeMs)
{
    p_decay = decayTimeMs;
    Update(p_decay, AdsrStage::SUSTAIN, false);
}

void AdsrEnvelopeModulator::SetSustainLevel(float sustainLevel)
{
    p_sustain = sustainLevel;
}

void AdsrEnvelopeModulator::SetReleaseTime(float releaseTimeMs)
{
    p_release = releaseTimeMs;
    Update(p_release, AdsrStage::IDLE, false);
}

void AdsrEnvelopeModulator::Update(float stageDuration, AdsrStage nextStage, bool incrementSampleCount)
{
    if (incrementSampleCount) {
        m_samplesSinceLastStage++;
    }

    float msPerSample = 1000.0f / (float)m_context.sampleRate;
    if ((float)m_samplesSinceLastStage * msPerSample >= stageDuration) {
        m_samplesSinceLastStage = 0;
        m_stage = nextStage;
    }
}
