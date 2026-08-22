#include "ofApp.h"


// 1. SETUP: Called once when the app starts
void ofApp::setup() {
    ofSetWindowTitle("OpenFrameworks Synthesizer & Visualizer");
    ofBackground(18, 22, 28);
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

// 3. DRAW: Renders UI header, oscilloscope, FFT spectrum, pitch ribbon, and piano keybed
void ofApp::draw() {
    float margin = 24.0f;
    float contentW = ofGetWidth() - margin * 2.0f;

    // Header HUD
    drawHeader(margin, 20.0f, contentW);

    // Top: Oscilloscope & Spectrum Side-by-Side or Stacked
    float visY = 70.0f;
    float visH = 180.0f;
    float halfW = (contentW - 16.0f) / 2.0f;

    // Oscilloscope Panel (Time Domain)
    ofSetColor(148, 163, 184);
    ofDrawBitmapString("WAVEFORM (TIME DOMAIN - OSCILLOSCOPE)", margin, visY - 6.0f);
    visualizer.drawWaveform(margin, visY, halfW, visH);

    // Spectrum Analyzer Panel (Frequency Domain DFT)
    ofSetColor(148, 163, 184);
    ofDrawBitmapString("SPECTRUM (DFT / FFT HARMONIC MAGNITUDE)", margin + halfW + 16.0f, visY - 6.0f);
    visualizer.drawSpectrum(margin + halfW + 16.0f, visY, halfW, visH);

    // Middle: Pitch Ribbon Controller
    ribbonX = margin;
    ribbonY = visY + visH + 28.0f;
    ribbonW = contentW;
    ribbonH = 44.0f;
    drawPitchRibbon(ribbonX, ribbonY, ribbonW, ribbonH);

    // Lower-Middle: Hardware Piano Keyboard Controller
    pianoX = margin;
    pianoY = ribbonY + ribbonH + 28.0f;
    pianoW = contentW;
    pianoH = 175.0f;
    drawPianoKeyboard(pianoX, pianoY, pianoW, pianoH);

    // Bottom: Instructions & Waveform Selector HUD
    drawControlPanel(margin, pianoY + pianoH + 16.0f, contentW, 70.0f);
}

// Draw Top Header HUD Bar
void ofApp::drawHeader(float x, float y, float w) {
    ofPushStyle();

    // App Title
    ofSetColor(241, 245, 249);
    ofDrawBitmapString("OPENFRAMEWORKS SYNTHESIZER & FFT VISUALIZER", x, y + 14.0f);

    // Audio Status Badge
    ofFill();
    ofSetColor(16, 185, 129);
    ofDrawCircle(x + w - 160.0f, y + 10.0f, 4.0f);
    ofSetColor(52, 211, 153);
    ofDrawBitmapString("AUDIO ENGINE ACTIVE", x + w - 148.0f, y + 14.0f);

    ofPopStyle();
}

// Draw Interactive Graphical Piano Keyboard
void ofApp::drawPianoKeyboard(float x, float y, float w, float h) {
    ofPushStyle();

    // Chassis Box
    ofFill();
    ofSetColor(10, 13, 17);
    ofDrawRectRounded(x, y, w, h, 10);
    ofNoFill();
    ofSetColor(30, 41, 59);
    ofDrawRectRounded(x, y, w, h, 10);

    // Red Felt Dampener Strip
    ofFill();
    ofSetColor(153, 27, 27);
    ofDrawRectRounded(x + 4.0f, y + 4.0f, w - 8.0f, 6.0f, 2);

    float keysY = y + 12.0f;
    float keysH = h - 16.0f;

    // 8 White Keys: C4 (60), D4 (62), E4 (64), F4 (65), G4 (67), A4 (69), B4 (71), C5 (72)
    int whiteMidis[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
    std::string whiteNames[8] = { "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5" };
    std::string whiteKeys[8] = { "A", "S", "D", "F", "G", "H", "J", "K" };
    float whiteW = (w - 12.0f) / 8.0f;

    for (int i = 0; i < 8; ++i) {
        int midi = whiteMidis[i];
        bool isActive = (activeMidiNotes.find(midi) != activeMidiNotes.end());
        float kx = x + 6.0f + i * whiteW;

        ofFill();
        if (isActive) {
            // Emerald green highlight when pressed
            ofSetColor(52, 211, 153);
        } else {
            // Ivory white key
            ofSetColor(241, 245, 249);
        }
        ofDrawRectRounded(kx, keysY, whiteW - 2.0f, keysH, 4);

        // Key Bottom Bevel
        ofSetColor(isActive ? ofColor(16, 185, 129) : ofColor(203, 213, 225));
        ofDrawRectRounded(kx, keysY + keysH - 6.0f, whiteW - 2.0f, 6.0f, 2);

        // Label on white key: Shortcut & Note Name
        ofSetColor(isActive ? ofColor(15, 23, 42) : ofColor(71, 85, 105));
        ofDrawBitmapString("[" + whiteKeys[i] + "]", kx + (whiteW / 2.0f) - 10.0f, keysY + keysH - 24.0f);
        ofSetColor(isActive ? ofColor(15, 23, 42) : ofColor(15, 23, 42));
        ofDrawBitmapString(whiteNames[i], kx + (whiteW / 2.0f) - 8.0f, keysY + keysH - 10.0f);
    }

    // 5 Black Keys: C#4 (61), D#4 (63), F#4 (66), G#4 (68), A#4 (70)
    int blackMidis[5] = { 61, 63, 66, 68, 70 };
    std::string blackNames[5] = { "C#", "D#", "F#", "G#", "A#" };
    std::string blackKeys[5] = { "W", "E", "T", "Y", "U" };
    int blackOffsets[5] = { 0, 1, 3, 4, 5 }; // Index relative to white keys
    float blackW = whiteW * 0.58f;
    float blackH = keysH * 0.60f;

    for (int i = 0; i < 5; ++i) {
        int midi = blackMidis[i];
        bool isActive = (activeMidiNotes.find(midi) != activeMidiNotes.end());
        float kx = x + 6.0f + (blackOffsets[i] + 1) * whiteW - (blackW / 2.0f);

        ofFill();
        if (isActive) {
            // Vivid green highlight
            ofSetColor(16, 185, 129);
        } else {
            // Ebony matte black
            ofSetColor(24, 29, 38);
        }
        ofDrawRectRounded(kx, keysY, blackW, blackH, 3);

        // Key Border
        ofNoFill();
        ofSetColor(isActive ? ofColor(52, 211, 153) : ofColor(51, 65, 85));
        ofDrawRectRounded(kx, keysY, blackW, blackH, 3);

        // Label on black key
        ofSetColor(isActive ? ofColor(255) : ofColor(148, 163, 184));
        ofDrawBitmapString(blackKeys[i], kx + (blackW / 2.0f) - 4.0f, keysY + blackH - 18.0f);
        ofDrawBitmapString(blackNames[i], kx + (blackW / 2.0f) - 7.0f, keysY + blackH - 6.0f);
    }

    ofPopStyle();
}

// Draw Continuous Pitch Ribbon Controller
void ofApp::drawPitchRibbon(float x, float y, float w, float h) {
    ofPushStyle();

    // Ribbon Container
    ofFill();
    ofSetColor(15, 23, 42);
    ofDrawRectRounded(x, y, w, h, 6);
    ofNoFill();
    ofSetColor(30, 41, 59);
    ofDrawRectRounded(x, y, w, h, 6);

    // Gradient bar
    ofFill();
    ofSetColor(59, 130, 246, 30);
    ofDrawRectRounded(x + 2.0f, y + 2.0f, w - 4.0f, h - 4.0f, 4);

    // Center Label
    ofSetColor(148, 163, 184);
    ofDrawBitmapString("CONTINUOUS PITCH RIBBON CONTROLLER (CLICK & DRAG HORIZONTALLY ➔ MIDI 48..72 / 130Hz..523Hz)", x + 16.0f, y + h / 2.0f + 4.0f);

    // Active touch cursor indicator
    if (mouseNote != -1) {
        float cursorX = ofMap((float)mouseNote, 48.0f, 72.0f, x + 8.0f, x + w - 8.0f, true);
        ofFill();
        ofSetColor(52, 211, 153);
        ofDrawCircle(cursorX, y + h / 2.0f, 8.0f);
        ofSetColor(241, 245, 249);
        ofDrawBitmapString("MIDI " + ofToString(mouseNote), cursorX - 22.0f, y - 6.0f);
    }

    ofPopStyle();
}

// Draw Control Panel HUD (Active waveform, keyboard shortcuts)
void ofApp::drawControlPanel(float x, float y, float w, float h) {
    ofPushStyle();

    ofFill();
    ofSetColor(15, 23, 42);
    ofDrawRectRounded(x, y, w, h, 8);
    ofNoFill();
    ofSetColor(30, 41, 59);
    ofDrawRectRounded(x, y, w, h, 8);

    // Waveform indicator
    std::string waveStr;
    ofColor waveColor;
    switch (synth.getWaveType()) {
        case WaveType::SINE:   waveStr = "[1] SINE"; waveColor = ofColor(52, 211, 153); break;
        case WaveType::SQUARE: waveStr = "[2] SQUARE"; waveColor = ofColor(96, 165, 250); break;
        case WaveType::SAW:    waveStr = "[3] SAWTOOTH"; waveColor = ofColor(251, 146, 60); break;
    }

    ofSetColor(waveColor);
    ofDrawBitmapString("ACTIVE WAVEFORM: " + waveStr, x + 16.0f, y + 22.0f);

    ofSetColor(148, 163, 184);
    ofDrawBitmapString("Press Keys [1] Sine | [2] Square | [3] Sawtooth to toggle waveform engine", x + 16.0f, y + 42.0f);
    ofDrawBitmapString("Press Computer Keys [A W S E D F T G Y H U J K] or Click On-Screen Piano Keys to play", x + 16.0f, y + 58.0f);

    ofPopStyle();
}

// Handle computer key press: waveform selection ('1','2','3') or playing a note
void ofApp::keyPressed(int key) {
    if (key == '1') { synth.setWaveType(WaveType::SINE);   return; }
    if (key == '2') { synth.setWaveType(WaveType::SQUARE); return; }
    if (key == '3') { synth.setWaveType(WaveType::SAW);    return; }

    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        int midi = it->second;
        activeMidiNotes.insert(midi);
        float freq = 440.0f * std::pow(2.0f, (midi - 69) / 12.0f);
        synth.noteOn(midi, freq);
    }
}

// Handle computer key release: stop the corresponding note
void ofApp::keyReleased(int key) {
    auto it = keyToNote.find(key);
    if (it != keyToNote.end()) {
        int midi = it->second;
        activeMidiNotes.erase(midi);
        synth.noteOff(midi);
    }
}

// Convert horizontal mouse X position into a continuous pitch (MIDI 48..72)
int ofApp::xToMidiNote(int x) const {
    return (int)ofMap((float)x, ribbonX, ribbonX + ribbonW, 48.0f, 72.0f, true);
}

// Mouse click: trigger ribbon or piano keys
void ofApp::mousePressed(int x, int y, int button) {
    // Check if clicked in pitch ribbon
    if (y >= ribbonY && y <= ribbonY + ribbonH && x >= ribbonX && x <= ribbonX + ribbonW) {
        mouseNote = xToMidiNote(x);
        activeMidiNotes.insert(mouseNote);
        float freq = 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f);
        synth.noteOn(mouseNote, freq);
        return;
    }

    // Check if clicked on piano keybed
    if (y >= pianoY && y <= pianoY + pianoH && x >= pianoX && x <= pianoX + pianoW) {
        float relX = x - pianoX - 6.0f;
        float whiteW = (pianoW - 12.0f) / 8.0f;
        int keyIndex = (int)(relX / whiteW);
        if (keyIndex >= 0 && keyIndex < 8) {
            int whiteMidis[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
            mouseNote = whiteMidis[keyIndex];
            activeMidiNotes.insert(mouseNote);
            float freq = 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f);
            synth.noteOn(mouseNote, freq);
        }
    }
}

// Mouse drag: pitch-bend across ribbon
void ofApp::mouseDragged(int x, int y, int button) {
    if (mouseNote != -1) {
        int newNote = xToMidiNote(x);
        if (newNote != mouseNote) {
            activeMidiNotes.erase(mouseNote);
            synth.noteOff(mouseNote);
            mouseNote = newNote;
            activeMidiNotes.insert(mouseNote);
            float freq = 440.0f * std::pow(2.0f, (mouseNote - 69) / 12.0f);
            synth.noteOn(mouseNote, freq);
        }
    }
}

// Mouse release: stop ribbon pitch
void ofApp::mouseReleased(int x, int y, int button) {
    if (mouseNote != -1) {
        activeMidiNotes.erase(mouseNote);
        synth.noteOff(mouseNote);
        mouseNote = -1;
    }
}

// Audio output stream callback from openFrameworks hardware sound engine
void ofApp::audioOut(ofSoundBuffer& buffer) {
    synth.audioOut(buffer);
}


