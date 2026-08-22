#pragma once
#include "Oscillator.h"


class SineOscillator : public Oscillator {
public:
    // Call parent Oscillator constructor with the engine sample rate
    SineOscillator(float sr = 44100.0f) : Oscillator(sr) {}

    // Calculate one audio sample for the current moment in time
    float generateSample() override {
        // Compute sine value from phase (phase is between 0.0 and 1.0)
        float sample = std::sin(phase * 2.0f * (float)M_PI);

        // Step phase forward based on desired pitch and audio rate
        phase += frequency / sampleRate;

        // Wrap phase back to [0.0, 1.0) when reaching the end of one cycle
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }

        return sample;
    }
};

