#pragma once
#include "ofMain.h"
#include "WaveType.h"
#include "Voice.h"
#include "Oscillator.h"
#include <vector>
#include <memory>


class Synth {
public:
    // Constructor and Destructor
    Synth();
    ~Synth();

    // Set up sample rate, audio buffer size, and number of simultaneous voices (polyphony)
    void setup(int sampleRate = 44100, int bufferSize = 256, int polyphony = 8);
    
    // openFrameworks audio stream callback: fills the hardware audio output buffer
    void audioOut(ofSoundBuffer & buffer);

    // Note trigger and release functions (takes MIDI note number and frequency in Hz)
    void noteOn(int noteKey, float frequency);
    void noteOff(int noteKey);

    // Switch active waveform for all voices (Sine, Square, Saw)
    void setWaveType(WaveType type);
    WaveType getWaveType() const;

    // Set global ADSR volume envelope times across all voices
    void setADSR(float a, float d, float s, float r);

    // Set master output volume (clamped 0.0 to 1.0)
    void setMasterVolume(float vol) { masterVolume = ofClamp(vol, 0.0f, 1.0f); }

    // Read access to internal mono audio buffer (used by Visualizer)
    const std::vector<float>& getBuffer() const { return monoBuffer; }

private:
    int sampleRate;      // Hardware audio sampling rate (typically 44100 Hz)
    int bufferSize;      // Audio block frame size (e.g., 256 or 512 frames)
    float masterVolume;  // Master gain multiplier

    std::vector<Voice> voices;                               // Bank of polyphonic voices (Composition)
    WaveType currentWaveType;                                // Currently active waveform selection

    std::vector<float> monoBuffer;                           // Internal buffer copy for visualization
};

