#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Oscillator {
public:
    Oscillator(float sr = 44100.0f) : frequency(440.0f), sampleRate(sr), phase(0.0f) {}
    virtual ~Oscillator() = default;

    virtual void setFrequency(float freq) { frequency = freq; }
    virtual void setSampleRate(float rate) { sampleRate = rate; }
    virtual float generateSample() = 0;

protected:
    float frequency;
    float sampleRate;
    float phase;
};
