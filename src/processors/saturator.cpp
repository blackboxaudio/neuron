#include "saturator.h"

using namespace cortex;

Sample Saturator::Process(const Sample& input)
{
    return std::tanh((float)input * m_saturationLevel);
}

void Saturator::SetSaturationLevel(float level)
{
    m_saturationLevel = level;
}
