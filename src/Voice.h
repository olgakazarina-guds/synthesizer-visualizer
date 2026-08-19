#pragma once
#include "Oscillator.h"
#include "Envelope.h"

class Voice {
public:
    Voice(float sampleRate = 44100.0f);

    void setOscillator(Oscillator* osc);
    void playNote(int midiKey, float frequency);
    void stopNote();
    float generateSample();

    bool isPlaying() const;
    int getNoteKey() const { return currentMidiKey; }
    Envelope& getEnvelope() { return envelope; }

private:
    Oscillator* oscillatorPtr;
    Envelope envelope;
    
    int currentMidiKey;
    float currentFrequency;
    bool noteActive;
};
