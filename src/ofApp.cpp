#include "ofApp.h"

// ==============================================================================
// ofApp.cpp
// Implementation of openFrameworks lifecycle, graphics drawing, and interaction.
// ==============================================================================

// 1. SETUP: Called once when the app starts
void ofApp::setup() {
    ofSetWindowTitle("OpenFrameworks Synthesizer & Visualizer");
    ofBackground(20, 24, 30);
    ofSetFrameRate(60);

    sampleRate = 44100;
    bufferSize = 512;
    mouseNote = -1;

    // Initialize synthesizer engine (44.1kHz, 512 buffer, 8 polyphonic voices)
    synth.setup(sampleRate, bufferSize, 8);

    // Initialize visualizer with 256-point DFT analyzer
    visualizer.setup(256);

    // Build map linking keyboard letters to musical MIDI notes
    buildKeyMap();

    // Configure openFrameworks audio hardware stream
    ofSoundStreamSettings settings;
    settings.setOutListener(this);      // this class listens for audioOut() calls
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;     // Stereo output (Left & Right)
    settings.numInputChannels = 0;      // No microphone needed
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

// Map home-row computer keyboard keys to MIDI pitches (C4 to C5)
void ofApp::buildKeyMap() {
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

// 2. UPDATE: Runs before every frame to refresh visualizer with latest audio data
void ofApp::update() {
    visualizer.update(synth.getBuffer());
}

// 3. DRAW: Renders oscilloscope, FFT spectrum bars, and on-screen control hints
void ofApp::draw() {
    // Waveform panel
    ofSetColor(255);
    ofDrawBitmapString("WAVEFORM (TIME DOMAIN)", 30, 30);
    visualizer.drawWaveform(30, 40, ofGetWidth() - 60, 220);

    // Spectrum panel
    ofDrawBitmapString("SPECTRUM (DFT / FFT MAGNITUDE)", 30, 290);
    visualizer.drawSpectrum(30, 300, ofGetWidth() - 60, 220);

    // Control instructions
    drawKeyboardHints();
}

// Draw helpful keyboard and mouse instructions at the bottom of the screen
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

// Handle computer key press: waveform selection ('1','2','3') or playing a note
void ofApp::keyPressed(int key) {
    if (key == '1') { synth.setWaveType(WaveType::SINE);   return; }
    if (key == '2') { synth.setWaveType(WaveType::SQUARE); return; }
    if (key == '3') { synth.setWaveType(WaveType::SAW);    return; }

    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        // Standard formula converting MIDI note number to frequency in Hz: f = 440 * 2^((midi - 69) / 12)
        float freq = 440.0f * std::pow(2.0f, (it->second - 69) / 12.0f);
        synth.noteOn(it->second, freq);
    }
}

// Handle computer key release: stop the corresponding note
void ofApp::keyReleased(int key) {
    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        synth.noteOff(it->second);
    }
}

// Convert horizontal mouse X position into a continuous pitch (MIDI 48..72)
int ofApp::xToMidiNote(int x) const {
    return (int)ofMap(x, 0, ofGetWidth(), 48, 72, true);
}

// Mouse click: trigger ribbon pitch
void ofApp::mousePressed(int x, int y, int button) {
    mouseNote = xToMidiNote(x);
    float freq = 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f);
    synth.noteOn(mouseNote, freq);
}

// Mouse drag: pitch-bend across ribbon
void ofApp::mouseDragged(int x, int y, int button) {
    int newNote = xToMidiNote(x);
    if (newNote != mouseNote) {
        if (mouseNote != -1) synth.noteOff(mouseNote);
        mouseNote = newNote;
        float freq = 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f);
        synth.noteOn(mouseNote, freq);
    }
}

// Mouse release: stop ribbon pitch
void ofApp::mouseReleased(int x, int y, int button) {
    if (mouseNote != -1) {
        synth.noteOff(mouseNote);
        mouseNote = -1;
    }
}

// Audio output stream callback from openFrameworks hardware sound engine
void ofApp::audioOut(ofSoundBuffer& buffer) {
    synth.audioOut(buffer);
}

