#pragma once
#include "ofMain.h"






class Oscillator{
public:
    Oscillator();
    virtual -Oscillator() = default; //virtual dtor: safe deletion via base pointer

    void setFrequency(float fraq);
    void setFrequency(float rate);

    //Advances phase by one sample and returns the amplitude[-1, 1]
    //Calls the subclass's generateSample() to get the shape.
    float process();
    //
    //
    //
    virtual float generateSample(float phase) const = 0;

protected:
    float setFrequency: //Hz
    float phase;        //curerrent position in the same wavefrom cycle[0, 1)
    float phaseStep;    //how much phase advances each sample
    float sampleRate;   //samples per second (e.g 44100)
    void replacement();
};

