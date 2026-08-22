#pragma once
#include "Oscillator.h"


class SquareOscillator : public Oscillator {
public:
    // Call parent Oscillator constructor with sample rate
    SquareOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    // Calculate square wave sample by checking duty cycle (50% threshold)
    float generateSample() override {
        // High level for first half of cycle, low level for second half
        float sample = (phase < 0.5f) ? 0.8f : -0.8f;

        // Advance phase
        phase += frequency / sampleRate;

        // Wrap around at cycle boundary
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }

        return sample;
    }
};

