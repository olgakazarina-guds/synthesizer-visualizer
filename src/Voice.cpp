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
    return oscillatorPtr->generateSample() * envelope.process();
}

bool Voice::isPlaying() const {
    return envelope.isActive();
}
