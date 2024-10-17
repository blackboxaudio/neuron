#pragma once

#include <cmath>

#include "sample.h"

namespace cortex {
class Saturator {
public:
    Saturator();
    ~Saturator();

    Sample Process(const Sample& input);

    void SetSaturationLevel(float level);

private:
    float m_saturationLevel = 1.0f;
};
}