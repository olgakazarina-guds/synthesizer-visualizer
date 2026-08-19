// Voice.h - Single Playing Note Manager
// Responsibility: Olga (Left UML Side)
#pragma once
#include "Oscillator.h"
#include "Envelope.h"

class Voice {
public:
    Voice(float sampleRate = 44100.0f);

    // Interface Contract: Holds Mohammed's Oscillator base-class pointer
    void setOscillator(Oscillator* osc);

    // Note trigger and release
    void playNote(int midiKey, float frequency);
    void stopNote();

    // Generates combined audio sample (Oscillator sample * Envelope volume)
    // Range: [-1.0, 1.0]
    float generateSample();

    bool isPlaying() const;
    int getNoteKey() const { return currentMidiKey; }

    // Direct access to composed Envelope for parameter adjustments
    Envelope& getEnvelope() { return envelope; }

private:
    Oscillator* oscillatorPtr; // Interface Contract: Holds base-class pointer
    Envelope envelope;         // Composition: Voice HAS-A Envelope (owns it)
    
    int currentMidiKey;
    float currentFrequency;
    bool noteActive;
};
