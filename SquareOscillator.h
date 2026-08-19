#pragma once
#include "Oscillator.h"

// Concrete oscillator: hard square wave. IS-A Oscillator.
class SquareOscillator : public Oscillator {
public:
    SquareOscillator() : Oscillator() {}
    SquareOscillator(float sr) : Oscillator(sr) {}
    float waveformAt(float phase) const override;
};
