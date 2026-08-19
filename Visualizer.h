#pragma once
#include "ofMain.h"
#include <vector>

// Draws the audio as a live waveform and FFT spectrum.
// Relationship: ASSOCIATION — reads the Synth's buffer without owning it.
class Visualizer {
public:
    Visualizer();

    void setup(int fftSize);
    void update(const std::vector<float>& samples);

    void drawWaveform(float x, float y, float w, float h) const;
    void drawSpectrum(float x, float y, float w, float h) const;

private:
    std::vector<float> waveform;   // copy of float samples
    std::vector<float> spectrum;   // magnitude per frequency bin
    int fftSize;

    void computeSpectrum();        // naive DFT for teaching clarity
};
