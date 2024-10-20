# `cortex`

[![Test](https://github.com/blackboxdsp/cortex/actions/workflows/ci.test.yml/badge.svg)](https://github.com/blackboxdsp/cortex/actions/workflows/ci.test.yml)
[![Build](https://github.com/blackboxdsp/cortex/actions/workflows/ci.build.yml/badge.svg)](https://github.com/blackboxdsp/cortex/actions/workflows/ci.build.yml)
[![cortex: v0.1.0](https://img.shields.io/badge/Version-v0.1.0-blue.svg)](https://github.com/blackboxdsp/cortex)
[![License](https://img.shields.io/badge/License-MIT-yellow)](https://github.com/blackboxdsp/cortex/blob/develop/LICENSE)

> Collection of C++ audio DSP components 🧠

## Overview

`cortex` is an open-source DSP (Digital Signal Processing) library that provides a wide-ranging suite of components for building audio software.

It can be used for a variety of applications, including:

- [JUCE](https://juce.com/) audio plugins
- Embedded hardware such as the [Electrosmith Daisy](https://electro-smith.com/collections/daisy) (see [Flora](https://github.com/blackboxdsp/flora))
- [VCV Rack](https://vcvrack.com/) modules

## Features

`cortex` currently lacks wide support for features because it is in early stages, however the following are in its immediate roadmap:

- Dynamics
  - Compressor
  - Limiter
- Effects
  - Chorus
  - Delay
  - Echo
  - Flanger
  - Phaser
  - Reverb
  - Saturation
  - Wavefolding
- Filters
  - LP/HP/BP Filter
  - Ladder Filter
- Modulators
  - ADSR Envelope Generator
  - AHD Envelope Generator
  - Envelope Follower
  - Low-Frequency Oscillator
- Synthesis
  - Amplitude Modulation
  - Frequency Modulation
  - Granular
  - Phase Distortion
- Utilities
  - Clock 
  - DC Block
  - Delay Line
  - Sampler
  - Wavetable

## Getting Started

Clone this repository:
```bash 
git clone https://github.com/blackboxdsp/cortex
cd cortex/
```

Build the library:
```bash 
make
```

## Using the Library

```c++
#include "cortex.h"

// Create a DSP context (sample rate,
// number of channels, buffer size).
static Context context {
    44100,
    1,
    128,
};

// Create an oscillator with an initial
// frequency of 440Hz.
static Oscillator osc(context, 440.0f);

// Write to the buffer with samples
// generated from the oscillator
for(size_t idx = 0; idx < 128; idx++) {
    buffer[idx] = (float)osc.Generate();
}
```
