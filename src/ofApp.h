#pragma once
#include "ofMain.h"
#include "Synth.h"
#include "Visualizer.h"
#include "WaveType.h"
#include <map>

// ==============================================================================
// ofApp.h
// Main openFrameworks Application Controller.
//
// Role in project architecture:
// - Inherits from `ofBaseApp` to connect to window lifecycle and input events.
// - Owns the `Synth` engine and the `Visualizer`.
// - Coordinates computer keyboard and mouse interactions to play musical notes.
// - Connects the openFrameworks audio subsystem `ofSoundStream` to `Synth::audioOut()`.
// ==============================================================================

class ofApp : public ofBaseApp {
public:
    // Core openFrameworks lifecycle functions
    void setup() override;   // Runs once at application launch
    void update() override;  // Runs every frame before drawing (logic updates)
    void draw() override;    // Runs every frame to render 2D/3D graphics

    // Input event handlers
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;

    // Audio stream callback from hardware sound driver
    void audioOut(ofSoundBuffer& buffer) override;

private:
    Synth synth;                  // Polyphonic synthesizer engine
    Visualizer visualizer;        // Real-time waveform & spectrum visualizer
    ofSoundStream soundStream;    // openFrameworks audio hardware stream manager

    int sampleRate;               // Sampling rate (44100 Hz)
    int bufferSize;               // Audio buffer size (512 frames)

    std::map<int, int> keyToNote; // Maps keyboard characters ('a', 's', ...) to MIDI numbers (60, 62, ...)
    int mouseNote;                // Tracks currently playing note from mouse ribbon drag

    // Helper setup and rendering functions
    void buildKeyMap();
    int xToMidiNote(int x) const;
    void drawKeyboardHints();
};

