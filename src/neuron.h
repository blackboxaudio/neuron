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

// ABSTRACTIONS
#include "abstractions/generator.h"
#include "abstractions/modulator.h"
#include "abstractions/neuron.h"
#include "abstractions/parameter.h"
#include "abstractions/processor.h"

// AUDIO
#include "audio/context.h"
#include "audio/sample.h"
#include "audio/waveform.h"

// GENERATORS
#include "generators/oscillator.h"

// MODULATORS
#include "modulators/adsr.h"

// PROCESSORS
#include "processors/effects/saturator.h"
#include "processors/effects/wavefolder.h"
#include "processors/filters/filter.h"

// UTILITIES
#include "utilities/arithmetic.h"
#include "utilities/midi.h"

#endif
