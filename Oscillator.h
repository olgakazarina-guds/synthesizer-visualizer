#pragma once
#include "ofMain.h"

// Abstract base class for all oscillators.
// Relationship: INHERITANCE — Sine/Square/Saw are-a Oscillator.
class Oscillator {
public:
    Oscillator();
    Oscillator(float sr);
    virtual ~Oscillator() = default;   // virtual dtor: safe deletion via base pointer

    void setFrequency(float freq);
    void setSampleRate(float rate);

    // Advances phase by one sample and returns amplitude [-1, 1].
    // Calls the subclass's waveformAt() to get the wave shape.
    float generateSample();

    // Each subclass implements its own waveform shape.
    virtual float waveformAt(float phase) const = 0;

protected:
    float frequency;    // Hz
    float phase;        // current position in the waveform cycle [0, 1)
    float phaseStep;    // how much phase advances each sample
    float sampleRate;   // samples per second (e.g. 44100)
    void recalcPhaseStep();
};
