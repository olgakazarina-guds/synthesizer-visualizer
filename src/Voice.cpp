#include "Voice.h"

// ==============================================================================
// Voice.cpp
// Implementation of polyphonic voice logic.
// ==============================================================================

// Constructor: initializes pointers and default state
Voice::Voice(float sampleRate)
    : oscillatorPtr(nullptr), envelope(sampleRate), currentMidiKey(-1), currentFrequency(440.0f), noteActive(false) {
}

// Point to the selected waveform oscillator
void Voice::setOscillator(Oscillator* osc) {
    oscillatorPtr = osc;
}

// Start playing a note: set frequency on oscillator and trigger envelope attack
void Voice::playNote(int midiKey, float frequency) {
    currentMidiKey = midiKey;
    currentFrequency = frequency;
    noteActive = true;

    if (oscillatorPtr != nullptr) {
        oscillatorPtr->setFrequency(frequency);
    }
    // Begin Attack phase of ADSR envelope
    envelope.triggerAttack();
}

// Stop playing a note: tell envelope to begin its release phase
void Voice::stopNote() {
    noteActive = false;
    envelope.triggerRelease();
}

// Generate the next audio sample by combining oscillator output with envelope volume
float Voice::generateSample() {
    // If the envelope is completely off or no oscillator is connected, output silence
    if (!envelope.isActive() || oscillatorPtr == nullptr) {
        return 0.0f;
    }

    // Multiply raw oscillator waveform by the current ADSR volume level
    float rawOscSample = oscillatorPtr->generateSample();
    float envelopeLevel = envelope.process();
    return rawOscSample * envelopeLevel;
}

// Returns true if the voice is still actively generating sound
bool Voice::isPlaying() const {
    return envelope.isActive();
}

