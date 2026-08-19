#include "Visualizer.h"

// ==============================================================================
// Visualizer.cpp
// Implementation of real-time waveform and DFT spectrum analyzer drawing.
// ==============================================================================

// Constructor: sets default FFT size to 256
Visualizer::Visualizer() : fftSize(256) {}

// Allocate space for half the FFT size (Nyquist frequency bins)
void Visualizer::setup(int size) {
    fftSize = size;
    spectrum.assign(fftSize / 2, 0.0f);
}

// Ingest latest audio samples and compute the frequency spectrum
void Visualizer::update(const std::vector<float>& samples) {
    waveform = samples;
    computeSpectrum();
}

// Compute Discrete Fourier Transform (DFT) to convert Time Domain -> Frequency Domain
void Visualizer::computeSpectrum() {
    if ((int)waveform.size() < fftSize) return;

    // For each frequency bin k (0 up to Nyquist limit: fftSize / 2)
    for (int k = 0; k < fftSize / 2; ++k) {
        float re = 0.0f; // Real component
        float im = 0.0f; // Imaginary component

        // Correlate with cosine and sine basis functions
        for (int n = 0; n < fftSize; n++) {
            float angle = TWO_PI * k * n / fftSize;
            re += waveform[n] * cos(angle);
            im -= waveform[n] * sin(angle);
        }

        // Calculate magnitude sqrt(re^2 + im^2) and normalize by fftSize
        spectrum[k] = sqrt(re * re + im * im) / fftSize;
    }
}

// Draw the oscilloscope waveform line
void Visualizer::drawWaveform(float x, float y, float w, float h) const {
    ofPushStyle();
    
    // Emerald green waveform line
    ofSetColor(80, 220, 160);
    ofNoFill();

    ofPolyline line;
    if (!waveform.empty()) {
        for (size_t i = 0; i < waveform.size(); ++i) {
            // Map sample index across width, and sample amplitude across height
            float px = x + ofMap(i, 0, waveform.size() - 1, 0, w);
            float py = y + h / 2 + waveform[i] * (h / 2);
            line.addVertex(px, py);
        }
    }
    line.draw();

    // Subtle center zero-baseline in gray
    ofSetColor(120);
    ofDrawLine(x, y + h / 2, x + w, y + h / 2);
    
    ofPopStyle();
}

// Draw the frequency spectrum bars
void Visualizer::drawSpectrum(float x, float y, float w, float h) const {
    ofPushStyle();
    ofFill();

    if (!spectrum.empty()) {
        float barW = w / spectrum.size();
        for (size_t i = 0; i < spectrum.size(); ++i) {
            // Scale magnitude for clear visual response
            float mag = ofClamp(spectrum[i] * 6.0f, 0.0f, 1.0f);
            float barH = mag * h;

            // Color gradient shifting from purple/blue to vibrant orange/red as intensity increases
            ofSetColor(60 + mag * 195, 120, 220 - mag * 120);
            ofDrawRectangle(x + i * barW, y + h - barH, barW - 1, barH);
        }
    }
    
    ofPopStyle();
}

