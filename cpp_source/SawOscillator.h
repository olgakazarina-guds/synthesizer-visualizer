#pragma once
#include "Oscillator.h"

// 3. Concrete Sawtooth Wave Oscillator
class SawOscillator : public Oscillator {
public:
    SawOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    float generateSample() override {
        float sample = 2.0f * phase - 1.0f;
        phase += frequency / sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample * 0.7f;
    }
};
