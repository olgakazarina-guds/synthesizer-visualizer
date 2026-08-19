#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Base Abstract Oscillator Class (Inheritance: IS-A)
class Oscillator {
public:
    Oscillator(float sr = 44100.0f) : frequency(440.0f), sampleRate(sr), phase(0.0f) {}
    virtual ~Oscillator() = default;

    virtual void setFrequency(float freq) {
        frequency = freq;
    }

    virtual void setSampleRate(float rate) {
        sampleRate = rate;
    }

    // Pure virtual method: returns a sample between -1.0 and 1.0
    // Subclasses implement their own DSP formulas
    virtual float generateSample() = 0;

protected:
    float frequency;
    float sampleRate;
    float phase;
};
