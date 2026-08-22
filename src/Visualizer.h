#pragma once
#include "ofMain.h"
#include <vector>

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

