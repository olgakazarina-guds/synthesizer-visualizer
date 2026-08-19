// Voice.cpp - Voice Implementation
// Responsibility: Olga (Left UML Side)
#include "Voice.h"

Voice::Voice(float sampleRate)
    : oscillatorPtr(nullptr), envelope(sampleRate), currentMidiKey(-1), currentFrequency(440.0f), noteActive(false) {
}

void Voice::setOscillator(Oscillator* osc) {
    oscillatorPtr = osc;
}

void Voice::playNote(int midiKey, float frequency) {
    currentMidiKey = midiKey;
    currentFrequency = frequency;
    noteActive = true;

    if (oscillatorPtr != nullptr) {
        // Contract: void setFrequency(float hz)
        oscillatorPtr->setFrequency(frequency);
    }
    envelope.triggerAttack();
}

void Voice::stopNote() {
    noteActive = false;
    envelope.triggerRelease();
}

float Voice::generateSample() {
    if (!envelope.isActive() || oscillatorPtr == nullptr) {
        return 0.0f;
    }

    // Interface Contract: calls float generateSample() -> returns -1.0 to 1.0
    float rawSample = oscillatorPtr->generateSample(); 
    float envVolume = envelope.process();              // 0.0 to 1.0

    return rawSample * envVolume;                      // -1.0 to 1.0
}

bool Voice::isPlaying() const {
    return envelope.isActive();
}
