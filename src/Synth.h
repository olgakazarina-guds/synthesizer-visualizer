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

    void setup(int sampleRate = 44100, int bufferSize = 256, int polyphony = 8);
    void audioOut(ofSoundBuffer & buffer);

    void noteOn(int noteKey, float frequency);
    void noteOff(int noteKey);

    void setWaveType(WaveType type);
    WaveType getWaveType() const;

    void setADSR(float a, float d, float s, float r);
    void setMasterVolume(float vol) { masterVolume = ofClamp(vol, 0.0f, 1.0f); }

    const std::vector<float>& getBuffer() const { return monoBuffer; }

private:
    int sampleRate;
    int bufferSize;
    float masterVolume;

    std::vector<Voice> voices;
    std::vector<std::unique_ptr<Oscillator>> oscillatorPool;
    WaveType currentWaveType;

    std::vector<float> monoBuffer;
};
