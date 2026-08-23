#include "Voice.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SawOscillator.h"


// Constructor: initializes sample rate, envelope, and default Sine oscillator
Voice::Voice(float sampleRate)
    : sampleRate(sampleRate), currentWaveType(WaveType::SINE), envelope(sampleRate),
      currentMidiKey(-1), currentFrequency(440.0f), noteActive(false) {
    setWaveType(WaveType::SINE);
}

// Allocate a dedicated oscillator instance for this voice based on waveform selection
void Voice::setWaveType(WaveType type) {
    currentWaveType = type;
    switch (type) {
        case WaveType::SINE:
            oscillator = std::make_unique<SineOscillator>(sampleRate);
            break;
        case WaveType::SQUARE:
            oscillator = std::make_unique<SquareOscillator>(sampleRate);
            break;
        case WaveType::SAW:
            oscillator = std::make_unique<SawOscillator>(sampleRate);
            break;
    }
    if (oscillator != nullptr) {
        oscillator->setFrequency(currentFrequency);
    }
}

// Start playing a note: set frequency on oscillator and trigger envelope attack
void Voice::playNote(int midiKey, float frequency) {
    currentMidiKey = midiKey;
    currentFrequency = frequency;
    noteActive = true;

    if (oscillator != nullptr) {
        oscillator->setFrequency(frequency);
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
    if (!envelope.isActive() || oscillator == nullptr) {
        return 0.0f;
    }

    // Multiply raw oscillator waveform by the current ADSR volume level
    float rawOscSample = oscillator->generateSample();
    float envelopeLevel = envelope.process();
    return rawOscSample * envelopeLevel;
}

// Returns true if the voice is still actively generating sound
bool Voice::isPlaying() const {
    return envelope.isActive();
}

