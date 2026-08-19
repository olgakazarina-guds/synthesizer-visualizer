#include "Visualizer.h"

Visualizer::Visualizer() : fftSize(256){}

void Visualizer::setup(int size){

    fftSize = size;
    spectrum.assign(fftSize / 2, 0.0f);

}

void Visualizer::update(const std::vector<float>& samples){
    waveform = samples;
    computeSpectrum();
}
void Visualizer::computeSpectrum(){
    if ((int)waveform.size() < fftSize) return;

    // naive Discrete Fourier Transform over the first ffsize samples.
    //(Reachable for teaching; a real synth would use ofxFft / kissFFT.)
    for (int k = 0; k < fftSize / 2; ++k){
        float re = 0.0f, im = 0.0f;
        for (int n = 0; n < fftSize; n++){
            float angle = TWO_PI * k * n / fftSize;
            re += waveform[n] * cos(angle);
            im -= waveform[n] * sin(angle);

        }
        spectrum[k] = sqrt(re * re + im * im) / fftSize;
    }
    
}

void Visualizer::drawWaveform(float x, float y, float w, float h) const {
    ofPushStyle();
    ofSetColor(80, 220, 160);
    ofNoFill();

    ofPolyline line;
    if (!waveform.empty()){
        for (size_t i = 0; i < waveform.size(); ++i){
            float px = x + ofMap(i, 0, waveform.size() -1, 0, w);
            float py = y + h / 2 + waveform[i] * (h / 2);
            line.addVertex(px, py); 
        }
    }
    line.draw();
    ofSetColor(120);
    ofDrawLine(x, y + h / 2, x + w, y + h / 2);
    ofPopStyle();
}

void Visualizer::drawSpectrum(float x, float y, float w, float h) const {
    ofPushStyle();
    ofFill();

    if(!spectrum.empty()){
        float barW = w / spectrum.size();
        for (size_t i = 0; i < spectrum.size(); ++i){
            float mag = ofClamp(spectrum[i] * 6.0f, 0.0f, 1.0f);
            float barH = mag * h;
            ofSetColor(60 + mag * 195, 120, 220 - mag * 120);
            ofDrawRectangle(x + i * barW, y + h - barH, barW - 1, barH);
        }
    }
    ofPopStyle();
}