#include "saturator.h"

using namespace cortex;

Saturator::Saturator() { }

Saturator::~Saturator() { }

Sample Saturator::Process(const Sample& input)
{
    return std::tanh((float)input * m_saturationLevel);
}

void Saturator::SetSaturationLevel(float level)
{
    m_saturationLevel = level;
}
