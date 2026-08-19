#pragma once
#include "ofMain.h"
#include "Synth.h"
#include "Visualizer.h"
#include "WaveType.h"
#include <map>

// ofApp is the top-level application controller and openFrameworks entry
// point. It owns the synthesizer and visualizer, routes keyboard + mouse
// input to the synth, and drives the audio + drawing callbacks.
class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;

    void audioOut(ofSoundBuffer& buffer) override; // audio-thread callback

private:
    Synth         synth;
    Visualizer    visualizer;
    ofSoundStream soundStream;

    int sampleRate;
    int bufferSize;

    std::map<int, int> keyToNote; // computer key --> MIDI note
    int mouseNote;

    void buildKeyMap();
    int  xToMidiNote(int x) const;       // mouse X --> pitch
    float midiToFreq(int midiNote) const; // MIDI note --> frequency
    void drawKeyboardHints();
};
