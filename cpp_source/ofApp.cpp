#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowTitle("OpenFrameworks Synthesizer & Visualizer");
    ofBackground(20, 24, 30);
    ofSetFrameRate(60);

    sampleRate = 44100;
    bufferSize = 512;
    mouseNote = -1;

    synth.setup(sampleRate, bufferSize, 8);
    visualizer.setup(256);
    buildKeyMap();

    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

void ofApp::buildKeyMap() {
    // Home-row keys a s d f g h j k --> C4..C5 (MIDI 60..72)
    keyToNote['a'] = 60; // C4
    keyToNote['w'] = 61; // C#4
    keyToNote['s'] = 62; // D4
    keyToNote['e'] = 63; // D#4
    keyToNote['d'] = 64; // E4
    keyToNote['f'] = 65; // F4
    keyToNote['t'] = 66; // F#4
    keyToNote['g'] = 67; // G4
    keyToNote['y'] = 68; // G#4
    keyToNote['h'] = 69; // A4
    keyToNote['u'] = 70; // A#4
    keyToNote['j'] = 71; // B4
    keyToNote['k'] = 72; // C5
}

void ofApp::update() {
    visualizer.update(synth.getBuffer());
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("WAVEFORM (TIME DOMAIN)", 30, 30);
    visualizer.drawWaveform(30, 40, ofGetWidth() - 60, 220);

    ofDrawBitmapString("SPECTRUM (DFT / FFT MAGNITUDE)", 30, 290);
    visualizer.drawSpectrum(30, 300, ofGetWidth() - 60, 220);

    drawKeyboardHints();
}

void ofApp::drawKeyboardHints() {
    ofSetColor(200);
    std::string wave;
    switch (synth.getWaveType()) {
        case WaveType::SINE:   wave = "SINE"; break;
        case WaveType::SQUARE: wave = "SQUARE"; break;
        case WaveType::SAW:    wave = "SAW"; break;
    }

    ofDrawBitmapString("Keys [A, S, D, F, G, H, J, K]  = Notes C4..C5", 30, 560);
    ofDrawBitmapString("Keys [1, 2, 3]                  = Waveform (1: Sine, 2: Square, 3: Saw)", 30, 585);
    ofDrawBitmapString("Active Waveform: " + wave, 30, 610);
    ofDrawBitmapString("Mouse Ribbon: Click + drag horizontally anywhere to bend pitch (MIDI 48..72)", 30, 635);
}

void ofApp::keyPressed(int key) {
    if (key == '1') { synth.setWaveType(WaveType::SINE);   return; }
    if (key == '2') { synth.setWaveType(WaveType::SQUARE); return; }
    if (key == '3') { synth.setWaveType(WaveType::SAW);    return; }

    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        synth.noteOn(it->second, 440.0f * std::pow(2.0f, (it->second - 69) / 12.0f));
    }
}

void ofApp::keyReleased(int key) {
    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        synth.noteOff(it->second);
    }
}

int ofApp::xToMidiNote(int x) const {
    return (int)ofMap(x, 0, ofGetWidth(), 48, 72, true);
}

void ofApp::mousePressed(int x, int y, int button) {
    mouseNote = xToMidiNote(x);
    synth.noteOn(mouseNote, 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f));
}

void ofApp::mouseDragged(int x, int y, int button) {
    int newNote = xToMidiNote(x);
    if (newNote != mouseNote) {
        if (mouseNote != -1) synth.noteOff(mouseNote);
        mouseNote = newNote;
        synth.noteOn(mouseNote, 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f));
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (mouseNote != -1) {
        synth.noteOff(mouseNote);
        mouseNote = -1;
    }
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
    synth.audioOut(buffer);
}
