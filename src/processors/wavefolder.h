#pragma once

#include "sample.h"

namespace cortex {
class Wavefolder {
public:
    Wavefolder(float threshold = 1.0f, float inputGain = 1.0f);
    ~Wavefolder() {};

    Sample Process(const Sample input);

    void SetThreshold(float threshold);
    void SetInputGain(float gain);

private:
    float m_threshold;
    float m_inputGain;
};
}