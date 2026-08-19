#pragma once
#include "Oscillator.h"
#include "Envelope.h"

// ==============================================================================
// Voice.h
// Represents a single polyphonic voice (OOP Principle: Composition).
//
// Role in project architecture (Olga / Left UML):
// - Manages the playback of one specific musical note.
// - Composes an Envelope (for ADSR amplitude shaping).
// - Holds a pointer to the active Oscillator (for waveform generation).
// - Combines waveform * envelope volume to output the final audio sample.
// ==============================================================================

class Voice {
public:
    // Constructor initializing the voice with default sample rate
    Voice(float sampleRate = 44100.0f);

    // Assign which oscillator type (Sine, Square, Saw) this voice should use
    void setOscillator(Oscillator* osc);

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

    // Get reference to this voice's envelope for configuring ADSR
    Envelope& getEnvelope() { return envelope; }

private:
    Oscillator* oscillatorPtr; // Pointer to shared waveform generator (Polymorphism)
    Envelope envelope;         // Owned ADSR volume envelope (Composition)
    
    int currentMidiKey;        // MIDI note number (e.g., 60 for C4)
    float currentFrequency;    // Frequency in Hz (e.g., 261.63 Hz)
    bool noteActive;           // Whether the key is physically pressed down
};

