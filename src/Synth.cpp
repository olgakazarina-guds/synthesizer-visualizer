#include "Synth.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SawOscillator.h"


// Constructor: sets default parameters
Synth::Synth()
    : sampleRate(44100), bufferSize(256), masterVolume(0.8f), currentWaveType(WaveType::SINE) {
}

Synth::~Synth() {}

// Set up audio system, initialize oscillator pool, and allocate voices
void Synth::setup(int sr, int bufSize, int polyphony) {
    sampleRate = sr;
    bufferSize = bufSize;
    monoBuffer.resize(bufferSize, 0.0f);

    // 1. Create one instance of each concrete oscillator in our pool
    oscillatorPool.clear();
    oscillatorPool.push_back(std::make_unique<SineOscillator>((float)sampleRate));
    oscillatorPool.push_back(std::make_unique<SquareOscillator>((float)sampleRate));
    oscillatorPool.push_back(std::make_unique<SawOscillator>((float)sampleRate));

    // 2. Allocate polyphonic voices and assign default oscillator pointer
    voices.clear();
    for (int i = 0; i < polyphony; i++) {
        Voice v((float)sampleRate);
        v.setOscillator(oscillatorPool[static_cast<int>(currentWaveType)].get());
        voices.push_back(v);
    }
}

// Switch the waveform generator used by all voices
void Synth::setWaveType(WaveType type) {
    int typeIndex = static_cast<int>(type);
    if (typeIndex < 0 || typeIndex >= (int)oscillatorPool.size()) return;
    currentWaveType = type;

    // Point every voice's oscillator pointer to the newly selected waveform
    for (auto & voice : voices) {
        voice.setOscillator(oscillatorPool[typeIndex].get());
    }
}

WaveType Synth::getWaveType() const {
    return currentWaveType;
}

// Update the ADSR volume envelope times across all voices
void Synth::setADSR(float a, float d, float s, float r) {
    for (auto & voice : voices) {
        voice.getEnvelope().setADSR(a, d, s, r);
    }
}

// Play a musical note: search for an idle voice, or steal the first voice if all are busy
void Synth::noteOn(int noteKey, float frequency) {
    Voice* targetVoice = nullptr;

    // Look for an available, idle voice
    for (auto & voice : voices) {
        if (!voice.isPlaying()) {
            targetVoice = &voice;
            break;
        }
    }

    // Voice stealing fallback if all voices are currently sounding
    if (targetVoice == nullptr && !voices.empty()) {
        targetVoice = &voices[0];
    }

    // Trigger the note on the selected voice
    if (targetVoice != nullptr) {
        targetVoice->playNote(noteKey, frequency);
    }
}

// Stop a musical note: find all voices playing this noteKey and trigger their release
void Synth::noteOff(int noteKey) {
    for (auto & voice : voices) {
        if (voice.isPlaying() && voice.getNoteKey() == noteKey) {
            voice.stopNote();
        }
    }
}

// openFrameworks audio streaming callback: generates audio samples in real-time
void Synth::audioOut(ofSoundBuffer & buffer) {
    size_t numFrames = buffer.getNumFrames();

    // Ensure our internal visualizer buffer matches the frame count
    if (monoBuffer.size() != numFrames) {
        monoBuffer.resize(numFrames, 0.0f);
    }

    // Process sample-by-sample for every audio frame in the buffer
    for (size_t i = 0; i < numFrames; i++) {
        float mixedSample = 0.0f;

        // Sum (mix) the output from all active polyphonic voices
        for (auto & voice : voices) {
            if (voice.isPlaying()) {
                mixedSample += voice.generateSample();
            }
        }

        // Apply master gain volume
        mixedSample *= masterVolume;

        // Clamp audio between -1.0 and +1.0 to protect ears and prevent digital clipping
        mixedSample = ofClamp(mixedSample, -1.0f, 1.0f);

        // Store in mono buffer for the visualizer to inspect
        monoBuffer[i] = mixedSample;

        // Write stereo output channels (Left and Right)
        buffer[i * 2]     = mixedSample;
        buffer[i * 2 + 1] = mixedSample;
    }
}

