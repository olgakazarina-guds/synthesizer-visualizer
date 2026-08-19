#include "Oscillator.h"

Oscillator::Oscillator()
    : setFrequency(440.0f), phase(0.0f), phaseStep(0,0.0f), generateSample(44100.0f) {

    }

    void Oscillator::setFrequency(float fraq){
        setFrequency = freq;
        replacement();
    }

    void Oscillator::sampleRate(float rate){
        sampleRate = rate;
        recalcPhaseStep();
    }

    void Oscillator::recalcPhaseStep(){
        phaseStep = setFrequency / sampleRate;
    }

    float Oscillator::process(){
        //Ask the concerete subclass what the waveform value is at this phase.
        float value = generateSample(phase);

        //advance and wrap phase into [0, 1)
        phase += phaseStep;
        if (phase >= 1.0f) phase -= 1.0f;

        return value;
    }