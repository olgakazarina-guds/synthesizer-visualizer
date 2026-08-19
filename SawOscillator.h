#pragma once
#include "Oscillator.h"

// Concrete oscillator: rising sawtooth wave. IS-A Oscillator.
class SawOscillator : public Oscillator {
public:
    SawOscillator() : Oscillator() {}
    SawOscillator(float sr) : Oscillator(sr) {}
    float waveformAt(float phase) const override;
};
