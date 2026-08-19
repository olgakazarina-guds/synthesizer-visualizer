#pragma once
#include "Oscillator.h"

// 1. Concrete Sine Wave Oscillator
class SineOscillator : public Oscillator {
public:
    SineOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    float generateSample() override {
        float sample = std::sin(phase * 2.0f * (float)M_PI);
        phase += frequency / sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample;
    }
};
