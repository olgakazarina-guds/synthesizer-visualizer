#pragma once
#include "Oscillator.h"
#include "Envelope.h"
#include "WaveType.h"
#include <memory>


class Voice {
public:
    // Constructor initializing the voice with default sample rate
    Voice(float sampleRate = 44100.0f);

    // Assign waveform type (Sine, Square, Saw) - instantiates dedicated oscillator for true polyphony
    void setWaveType(WaveType type);

    // Trigger playback of a note with given MIDI number and Hz frequency
    void playNote(int midiKey, float frequency);

    // Release the note, entering the envelope's release phase
    void stopNote();

    // Compute the current sample: oscillator sample multiplied by envelope amplitude
    float generateSample();

    // Check if the voice is currently producing sound
    bool isPlaying() const;

    // Get the MIDI key assigned to this voice
    int getNoteKey() const { return currentMidiKey; }

    // Get the frequency in Hz assigned to this voice
    float getFrequency() const { return currentFrequency; }

    // Get reference to this voice's envelope for configuring ADSR
    Envelope& getEnvelope() { return envelope; }

private:
    float sampleRate;
    std::unique_ptr<Oscillator> oscillator; // Dedicated waveform generator (Polymorphism & Polyphony)
    WaveType currentWaveType;
    Envelope envelope;                      // Owned ADSR volume envelope (Composition)
    
    int currentMidiKey;                     // MIDI note number (e.g., 60 for C4)
    float currentFrequency;                 // Frequency in Hz (e.g., 261.63 Hz)
    bool noteActive;                        // Whether the key is physically pressed down
};

