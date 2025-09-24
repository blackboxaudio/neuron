/**
* Neuron is a lightweight audio DSP library intended for use
 * in any relevant application e.g. Electrosmith Daisy patches,
 * JUCE plugins, VCV Rack modules.
 *
 * Author: Matthew Maxwell, 2024
 */
#pragma once

#ifndef NEURON_LIB_H
#define NEURON_LIB_H

// CORE
#include "neuron/core/base.h"
#include "neuron/core/buffer.h"
#include "neuron/core/context.h"
#include "neuron/core/parameter.h"
#include "neuron/core/sample.h"

// DSP (Generators)
#include "neuron/dsp/generators/generator.h"

#include "neuron/dsp/generators/oscillator.h"

// DSP (Modulators)
#include "neuron/dsp/modulators/modulator.h"

#include "neuron/dsp/modulators/adsr.h"

// DSP (Processors)
#include "neuron/dsp/processors/processor.h"

#include "neuron/dsp/processors/filter.h"
#include "neuron/dsp/processors/saturator.h"
#include "neuron/dsp/processors/wavefolder.h"

// UTILS
#include "neuron/utils/arithmetic.h"
#include "neuron/utils/midi.h"
#include "neuron/utils/smoothed_value.h"
#include "neuron/utils/waveform.h"

#endif
