#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==============================================================================
// Oscillator.h
// Abstract Base Class for all audio oscillators (OOP Principle: Inheritance & Polymorphism).
//
// Role in project architecture:
// - Holds the common properties needed to produce a tone: frequency, sample rate, and phase.
// - Defines pure virtual function `generateSample() = 0` so derived classes
//   (SineOscillator, SquareOscillator, SawOscillator) can write their own wave math.
// ==============================================================================

class Oscillator {
public:
    // Constructor: initializes default frequency (440 Hz = musical A4), sample rate, and phase at 0
    Oscillator(float sr = 44100.0f) : frequency(440.0f), sampleRate(sr), phase(0.0f) {}
    
    // Virtual destructor: ensures derived class objects are properly cleaned up in memory
    virtual ~Oscillator() = default;

    // Set pitch/frequency in Hertz (Hz)
    virtual void setFrequency(float freq) { frequency = freq; }

    // Set sample rate (typically 44100 Hz)
    virtual void setSampleRate(float rate) { sampleRate = rate; }

    // Pure virtual function: each specific oscillator must calculate and return its sample (-1.0 to 1.0)
    virtual float generateSample() = 0;

protected:
    float frequency;   // Current pitch in Hz (e.g., 440.0f for note A4)
    float sampleRate;  // Audio engine sample rate (e.g., 44100 Hz)
    float phase;       // Current position in the waveform cycle, normalized from 0.0 to 1.0
};

