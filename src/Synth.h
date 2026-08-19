// Synth.h - Main Audio Engine Container
// Responsibility: Olga (Left UML Side)
#pragma once
#include "ofMain.h"
#include "WaveType.h"
#include "Voice.h"
#include "Oscillator.h"
#include <vector>
#include <memory>

class Synth {
public:
    Synth();
    ~Synth();

    // Audio setup and stream callback
    void setup(int sampleRate = 44100, int bufferSize = 256, int polyphony = 8);
    void audioOut(ofSoundBuffer & buffer);

    // Note triggers
    void noteOn(int noteKey, float frequency);
    void noteOff(int noteKey);

    // Waveform selector using Mohammed's WaveType enum
    void setWaveType(WaveType type);
    WaveType getWaveType() const;

    void setADSR(float a, float d, float s, float r);
    void setMasterVolume(float vol) { masterVolume = ofClamp(vol, 0.0f, 1.0f); }

    // Interface Contract: How Visualizer reads audio
    // Returns internal audio buffer with samples in range [-1.0, 1.0]
    const std::vector<float>& getBuffer() const { return monoBuffer; }

private:
    int sampleRate;
    int bufferSize;
    float masterVolume;

    // Composition: Synth HAS-A Voices (owns note instances)
    std::vector<Voice> voices;

    // Composition: Synth HAS-A concrete Oscillator instances
    std::vector<std::unique_ptr<Oscillator>> oscillatorPool;
    WaveType currentWaveType;

    // Audio buffer read by Visualizer via getBuffer()
    std::vector<float> monoBuffer;
};
