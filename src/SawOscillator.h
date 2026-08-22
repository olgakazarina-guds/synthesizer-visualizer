#pragma once
#include "Oscillator.h"


class SawOscillator : public Oscillator {
public:
    // Call parent Oscillator constructor with sample rate
    SawOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    // Calculate linear ramp sawtooth sample
    float generateSample() override {
        // Linearly ramp from -1.0 up to +1.0 as phase goes from 0.0 to 1.0
        float sample = 2.0f * phase - 1.0f;

        // Advance phase for next audio frame
        phase += frequency / sampleRate;

        // Wrap around at cycle boundary
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }

        // Multiply by 0.7 gain factor for headroom
        return sample * 0.7f;
    }
};

