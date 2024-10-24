#include "adsr.h"

using namespace cortex;

AdsrEnvelopeModulator::AdsrEnvelopeModulator(Context& context, AdsrEnvelope envelope)
    : m_context(context)
    , m_envelope(envelope)
{
}

AdsrEnvelopeModulator::~AdsrEnvelopeModulator()
{
}

float AdsrEnvelopeModulator::Modulate()
{
    float position = (float)m_samplesSinceLastStage * (1000.0f / m_context.sampleRate);

    float value;
    switch (m_stage) {
    case AdsrStage::ATTACK:
        value = position / m_envelope.attack;
        Update(m_envelope.attack, AdsrStage::DECAY, true);
        break;
    case AdsrStage::DECAY:
        value = (((m_envelope.sustain - 1.0f) / m_envelope.decay) * position) + 1.0f;
        Update(m_envelope.decay, AdsrStage::SUSTAIN, true);
        break;
    case AdsrStage::SUSTAIN:
        value = m_envelope.sustain;
        break;
    case AdsrStage::RELEASE:
        value = ((-m_envelope.sustain / m_envelope.release) * position) + m_envelope.sustain;
        Update(m_envelope.release, AdsrStage::IDLE, true);
        break;
    case AdsrStage::IDLE:
    default:
        value = 0.0f;
    }

    return value;
}

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
    m_envelope.attack = attackTimeMs;
    Update(m_envelope.attack, AdsrStage::DECAY, false);
}

void AdsrEnvelopeModulator::SetDecayTime(float decayTimeMs)
{
    m_envelope.decay = decayTimeMs;
    Update(m_envelope.decay, AdsrStage::SUSTAIN, false);
}

void AdsrEnvelopeModulator::SetSustainLevel(float sustainLevel)
{
    m_envelope.sustain = sustainLevel;
}

void AdsrEnvelopeModulator::SetReleaseTime(float releaseTimeMs)
{
    m_envelope.release = releaseTimeMs;
    Update(m_envelope.release, AdsrStage::IDLE, false);
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
