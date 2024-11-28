#include "processors/effects/wavefolder.h"

using namespace neuron;

Wavefolder::Wavefolder()
    : p_inputGain(1.0f), p_threshold(1.0f), p_symmetry(1.0f)
{

}

Sample Wavefolder::Process(Sample input)
{
    float output = input * p_inputGain;
    while (output > p_threshold || output < -p_threshold) {
        if (output > p_threshold) {
            output = p_threshold - (output - p_threshold);
        } else if (output < -p_threshold) {
            output = -p_threshold - (output + p_threshold);
        }
    }

    if (input < 0.0f) {
        output = input * (1.0f - p_symmetry) + output * p_symmetry;
    }

    return clamp(output, -1.0f, 1.0f);
}

void Wavefolder::SetInputGain(float gain)
{
    p_inputGain = gain;
}

void Wavefolder::SetThreshold(float threshold)
{
    p_threshold = threshold;
}

void Wavefolder::SetSymmetry(float symmetry)
{
    p_symmetry = clamp(symmetry, 0.0f, 1.0f);
}
