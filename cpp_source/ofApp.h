#pragma once
#include "ofMain.h"
#include "Synth.h"
#include "Visualizer.h"
#include "WaveType.h"
#include <map>

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

    void audioOut(ofSoundBuffer& buffer) override;

private:
    Synth synth;
    Visualizer visualizer;
    ofSoundStream soundStream;

    int sampleRate;
    int bufferSize;

    std::map<int, int> keyToNote;
    int mouseNote;

    void buildKeyMap();
    int xToMidiNote(int x) const;
    void drawKeyboardHints();
};
