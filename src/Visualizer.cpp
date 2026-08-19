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

// Draw the oscilloscope waveform line with panel frame and grid lines
void Visualizer::drawWaveform(float x, float y, float w, float h) const {
    ofPushStyle();
    
    // Panel background card
    ofFill();
    ofSetColor(12, 16, 22);
    ofDrawRectRounded(x, y, w, h, 8);

    // Subtle grid lines
    ofNoFill();
    ofSetColor(24, 32, 44);
    ofSetLineWidth(1);
    for (int i = 1; i <= 4; ++i) {
        float gx = x + (w * i) / 5.0f;
        ofDrawLine(gx, y, gx, y + h);
    }
    ofDrawLine(x, y + h * 0.25f, x + w, y + h * 0.25f);
    ofDrawLine(x, y + h * 0.75f, x + w, y + h * 0.75f);

    // Center zero-baseline
    ofSetColor(40, 56, 75);
    ofDrawLine(x, y + h / 2.0f, x + w, y + h / 2.0f);

    // Border
    ofSetColor(35, 45, 60);
    ofDrawRectRounded(x, y, w, h, 8);

    // Emerald green glowing waveform line
    if (!waveform.empty()) {
        ofPolyline line;
        for (size_t i = 0; i < waveform.size(); ++i) {
            float px = x + ofMap((float)i, 0.0f, (float)(waveform.size() - 1), 4.0f, w - 4.0f);
            float py = y + (h / 2.0f) + (waveform[i] * (h * 0.44f));
            line.addVertex(px, py);
        }

        // Soft outer glow line
        ofSetColor(52, 211, 153, 90);
        ofSetLineWidth(3.5f);
        line.draw();

        // Crisp inner core line
        ofSetColor(167, 243, 208, 255);
        ofSetLineWidth(1.8f);
        line.draw();
    }
    
    ofPopStyle();
}

// Draw the frequency spectrum bars with panel frame and colorful frequency bins
void Visualizer::drawSpectrum(float x, float y, float w, float h) const {
    ofPushStyle();
    
    // Panel background card
    ofFill();
    ofSetColor(12, 16, 22);
    ofDrawRectRounded(x, y, w, h, 8);

    // Subtle grid lines
    ofNoFill();
    ofSetColor(24, 32, 44);
    ofSetLineWidth(1);
    for (int i = 1; i <= 4; ++i) {
        float gx = x + (w * i) / 5.0f;
        ofDrawLine(gx, y, gx, y + h);
    }
    for (int i = 1; i <= 3; ++i) {
        float gy = y + (h * i) / 4.0f;
        ofDrawLine(x, gy, x + w, gy);
    }

    // Border
    ofSetColor(35, 45, 60);
    ofDrawRectRounded(x, y, w, h, 8);

    // Frequency Bars
    if (!spectrum.empty()) {
        ofFill();
        float barW = (w - 8.0f) / (float)spectrum.size();
        for (size_t i = 0; i < spectrum.size(); ++i) {
            float mag = ofClamp(spectrum[i] * 8.0f, 0.0f, 1.0f);
            float barH = mag * (h - 10.0f);

            if (barH > 1.0f) {
                // Harmonic color shift: Cyan (low bass) -> Emerald (mids) -> Amber/Violet (highs)
                float t = (float)i / (float)spectrum.size();
                float r = ofLerp(16.0f, 245.0f, t);
                float g = ofLerp(185.0f, 158.0f, mag);
                float b = ofLerp(229.0f, 66.0f, t);

                ofSetColor((int)r, (int)g, (int)b, 230);
                ofDrawRectRounded(x + 4.0f + i * barW, y + h - 5.0f - barH, barW > 2.0f ? barW - 1.0f : barW, barH, 2);
            }
        }
    }
    
    ofPopStyle();
}

