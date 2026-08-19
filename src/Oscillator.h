#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Base Abstract Oscillator Class (Inheritance: IS-A)
class Oscillator {
public:
    virtual ~Oscillator() = default;

    virtual void setFrequency(float freq) {
        frequency = freq;
    }

    virtual void setSampleRate(int rate) {
        sampleRate = rate;
    }

    // Pure virtual method: returns a sample between -1.0 and 1.0
    // Subclasses implement their own DSP formulas
    virtual float generateSample() = 0;

protected:
    float frequency = 440.0f;
    int sampleRate = 44100;
    float phase = 0.0f;
};

// 1. Concrete Sine Wave Oscillator
class SineOscillator : public Oscillator {
public:
    SineOscillator(int sr = 44100) { sampleRate = sr; }

    float generateSample() override {
        float sample = std::sin(phase * 2.0f * (float)M_PI);
        phase += frequency / (float)sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample;
    }
};

// 2. Concrete Square Wave Oscillator
class SquareOscillator : public Oscillator {
public:
    SquareOscillator(int sr = 44100) { sampleRate = sr; }

    float generateSample() override {
        float sample = (phase < 0.5f) ? 0.8f : -0.8f;
        phase += frequency / (float)sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample;
    }
};

// 3. Concrete Sawtooth Wave Oscillator
class SawOscillator : public Oscillator {
public:
    SawOscillator(int sr = 44100) { sampleRate = sr; }

    float generateSample() override {
        float sample = 2.0f * phase - 1.0f;
        phase += frequency / (float)sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample * 0.7f;
    }
};
