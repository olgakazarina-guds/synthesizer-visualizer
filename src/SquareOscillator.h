#pragma once
#include "Oscillator.h"

class SquareOscillator : public Oscillator {
public:
    SquareOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    float generateSample() override {
        float sample = (phase < 0.5f) ? 0.8f : -0.8f;
        phase += frequency / sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample;
    }
};
