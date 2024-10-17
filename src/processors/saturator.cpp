#include "saturator.h"

using namespace cortex;

Saturator::Saturator() { }

Saturator::~Saturator() { }

Sample Saturator::Process(const Sample& input)
{
    return (Sample)std::tanh((float)input * m_saturation);
}

void Saturator::SetSaturation(float level)
{
    m_saturation = level;
}
