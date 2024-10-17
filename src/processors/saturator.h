#pragma once

#include <cmath>

#include "sample.h"

namespace cortex {
class Saturator {
public:
    Saturator();
    ~Saturator();

    Sample Process(const Sample& input);

    void SetSaturation(float level);

private:
    float m_saturation = 1.0f;
};
}