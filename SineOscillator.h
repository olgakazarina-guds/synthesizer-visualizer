#pragma once
#include "Oscillator.h"

// Concrete oscillator: smooth sine wave. IS-A Oscillator.
class SineOscillator : public Oscillator {
public:
    SineOscillator() : Oscillator() {}
    SineOscillator(float sr) : Oscillator(sr) {}
    float waveformAt(float phase) const override;
};
