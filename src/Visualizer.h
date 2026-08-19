#pragma once
#include "ofMain.h"
#include <vector>

class Visualizer {
public:
    Visualizer();

    void setup(int fftSize);
    void update(const std::vector<float>& samples);

    void drawWaveform(float x, float y, float w, float h) const;
    void drawSpectrum(float x, float y, float w, float h) const;

private:
    std::vector<float> waveform;
    std::vector<float> spectrum;
    int fftSize;

    void computeSpectrum();
};
