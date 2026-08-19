#include "Oscillator.h"

Oscillator::Oscillator()
    : frequency(440.0f), phase(0.0f), phaseStep(0.0f), sampleRate(44100.0f) {
}

Oscillator::Oscillator(float sr)
    : frequency(440.0f), phase(0.0f), phaseStep(0.0f), sampleRate(sr) {
}

void Oscillator::setFrequency(float freq){
    frequency = freq;
    recalcPhaseStep();
}

void Oscillator::setSampleRate(float rate){
    sampleRate = rate;
    recalcPhaseStep();
}

void Oscillator::recalcPhaseStep(){
    phaseStep = frequency / sampleRate;
}

float Oscillator::generateSample(){
    // Ask the concrete subclass what the waveform value is at this phase.
    float value = waveformAt(phase);

    // advance and wrap phase into [0, 1)
    phase += phaseStep;
    if (phase >= 1.0f) phase -= 1.0f;

    return value;
}
