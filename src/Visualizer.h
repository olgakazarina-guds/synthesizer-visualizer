#pragma once
#include "ofMain.h"
#include <vector>

// ==============================================================================
// Visualizer.h
// Real-time Audio Visualizer (OOP Principle: Association with Synth).
//
// Role in project architecture (Mohammed / Right UML):
// - Inspects the floating-point audio buffer output by the Synth.
// - Renders two real-time visual displays:
//     1. Oscilloscope Waveform (Time Domain): plots amplitude over time using ofPolyline.
//     2. Spectrum Analyzer (Frequency Domain): computes DFT magnitudes and draws frequency bars.
// ==============================================================================

class Visualizer {
public:
    // Constructor
    Visualizer();

    // Initialize FFT / DFT analysis window size (e.g., 256 samples)
    void setup(int fftSize);

    // Update internal buffer with the latest samples from Synth::getBuffer()
    void update(const std::vector<float>& samples);

    // Draw the time-domain waveform oscilloscope
    void drawWaveform(float x, float y, float w, float h) const;

    // Draw the frequency-domain spectrum bars
    void drawSpectrum(float x, float y, float w, float h) const;

private:
    std::vector<float> waveform; // Copy of latest audio buffer samples
    std::vector<float> spectrum; // Frequency magnitudes computed by DFT
    int fftSize;                 // Number of sample points analyzed

    // Discrete Fourier Transform calculation
    void computeSpectrum();
};

