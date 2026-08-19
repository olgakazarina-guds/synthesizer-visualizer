// Synth.cpp - Synth Engine Implementation
// Responsibility: Olga (Left UML Side)
#include "Synth.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SawOscillator.h"

Synth::Synth()
    : sampleRate(44100), bufferSize(256), masterVolume(0.8f), currentWaveType(WaveType::SINE) {
}

Synth::~Synth() {}

void Synth::setup(int sr, int bufSize, int polyphony) {
    sampleRate = sr;
    bufferSize = bufSize;
    monoBuffer.resize(bufferSize, 0.0f);

    // Instantiate Concrete Oscillators (SINE = 0, SQUARE = 1, SAW = 2)
    oscillatorPool.clear();
    oscillatorPool.push_back(std::make_unique<SineOscillator>(sampleRate));
    oscillatorPool.push_back(std::make_unique<SquareOscillator>(sampleRate));
    oscillatorPool.push_back(std::make_unique<SawOscillator>(sampleRate));

    // Initialize Voice instances
    voices.clear();
    for (int i = 0; i < polyphony; i++) {
        Voice v(sampleRate);
        v.setOscillator(oscillatorPool[static_cast<int>(currentWaveType)].get());
        voices.push_back(v);
    }
}

void Synth::setWaveType(WaveType type) {
    int typeIndex = static_cast<int>(type);
    if (typeIndex < 0 || typeIndex >= (int)oscillatorPool.size()) return;
    currentWaveType = type;

    // Update all voices to point to the selected oscillator type
    for (auto & voice : voices) {
        voice.setOscillator(oscillatorPool[typeIndex].get());
    }
}

WaveType Synth::getWaveType() const {
    return currentWaveType;
}

void Synth::setADSR(float a, float d, float s, float r) {
    for (auto & voice : voices) {
        voice.getEnvelope().setADSR(a, d, s, r);
    }
}

void Synth::noteOn(int noteKey, float frequency) {
    // Find an idle voice or steal oldest
    Voice* targetVoice = nullptr;
    for (auto & voice : voices) {
        if (!voice.isPlaying()) {
            targetVoice = &voice;
            break;
        }
    }
    if (targetVoice == nullptr && !voices.empty()) {
        targetVoice = &voices[0]; // fallback voice stealing
    }

    if (targetVoice != nullptr) {
        targetVoice->playNote(noteKey, frequency);
    }
}

void Synth::noteOff(int noteKey) {
    for (auto & voice : voices) {
        if (voice.isPlaying() && voice.getNoteKey() == noteKey) {
            voice.stopNote();
        }
    }
}

void Synth::audioOut(ofSoundBuffer & buffer) {
    size_t numFrames = buffer.getNumFrames();
    if (monoBuffer.size() != numFrames) {
        monoBuffer.resize(numFrames, 0.0f);
    }

    for (size_t i = 0; i < numFrames; i++) {
        float mixedSample = 0.0f;

        // Iterate through voices and sum active samples
        for (auto & voice : voices) {
            if (voice.isPlaying()) {
                mixedSample += voice.generateSample();
            }
        }

        // Apply master volume and clamp
        mixedSample *= masterVolume;
        mixedSample = ofClamp(mixedSample, -1.0f, 1.0f);

        // Store in monoBuffer for Visualizer to read via getBuffer()
        monoBuffer[i] = mixedSample;

        // Write stereo output to openFrameworks buffer
        buffer[i * 2]     = mixedSample; // Left channel
        buffer[i * 2 + 1] = mixedSample; // Right channel
    }
}
