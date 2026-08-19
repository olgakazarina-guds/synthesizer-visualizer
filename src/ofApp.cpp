#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowTitle("OpenFrameworks synthesizer");
    ofBackground(20, 24, 30);
    ofSetFrameRate(60);

    sampleRate = 44100;
    bufferSize = 512;
    mouseNote = -1;


    synth.setSampleRate((float) sampleRate);
    visualizer.setup(256);
    buildKeyMap();


    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate   = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

void ofApp::buildKeyMap(){
    // Home-row Keys a s d f g h j k -->C4..C5(MIDI 60..72).

    keyToNote['a'] = 60; keyToNote['s'] = 62; keyToNote['d'] = 64;
    keyToNote['f'] = 65; keyToNote['g'] = 67; keyToNote['h'] = 69;
    keyToNote['j'] = 71; keyToNote['k'] = 72; 
    
}
void ofApp::update(){
    visualizer.update(synth.getLastBuffer());
}

void ofApp::draw(){
    ofSetColor(255);
    ofDrawBitmapString("WAVEFORM", 30, 30);
    visualizer.drawWaveform(30, 40, ofGetWidth() - 60, 220);
    ofDrawBitmapString("SPECTRUM", 30, 300);
    visualizer.drawSpectrum(30, 310, ofGetWidth() -60, 220);

    drawKeyboardHints();
}


void ofApp::drawKeyboardHints(){
    ofSetColor(180);
    std::string wave;
    switch (synth.getWaveType()){
        case WaveType::SINE:      wave = "SINE"; break;
        case WaveType::SQUARE:    wave = "SQUARE"; break;
        case WaveType::SAW:       wave = "SAW"; break;
    }

    ofDrawBitmapString("Keys a s d f g h j k  =  notes C4..C5", 30, 570);
    ofDrawBitmapString("Keys 1 2 3            =  sine / square / saw", 30, 590);
    ofDrawBitmapString("Current wave: " + wave, 30, 610);
    ofDrawBitmapString("Mouse: click + drag horizontally to play a pitch", 30, 630);   
    
}

void ofApp::keyPressed(int key){
    if (key == '1') { synth.setWaveType(WaveType::SINE);    return; }
    if (key == '2') { synth.setWaveType(WaveType::SQUARE);  return; }
    if (key == '3') { synth.setWaveType(WaveType::SAW);     return; }
    auto it = keyToNote.find(key);
    if(it != keyToNote.end()) synth.noteOn(it->second);

}
 
void ofApp::keyReleased(int key){
    auto it = keyToNote.find(key);
    if(it != keyToNote.end()) synth.noteOff(it->second);
    
}
int ofApp::xToMidiNote(int x) const {
    //Window width -> two octaves (MIDI 48..72).

    return (int)ofMap(x, 0, ofGetWidth(), 48, 72, true);
}
void ofApp::mousePressed(int x, int y, int button){
    //
    mouseNote = xToMidiNote(x);
    synth.noteOn(mouseNote);
}
void ofApp::mouseDragged(int x, int y, int button){
    //

    int newNote = xToMidiNote(x);
    if (newNote != mouseNote){
        if(mouseNote != -1) synth.noteOff(mouseNote);
        mouseNote = newNote;
        synth.noteOn(mouseNote);
    }

}
void ofApp::mouseReleased(int x, int y, int button){
    if (mouseNote != -1){
        synth.noteOff(mouseNote);
        mouseNote = -1;
    }
}

void ofApp::audioOut(ofSoundBuffer& buffer){
    int nChannels = buffer.getNumChannels();
    std::vector<float> temp(buffer.getNumFrames() * nChannels, 0.0f);
    synth.audioOut(temp, nChannels);
    for (size_t i = 0; i < temp.size(); ++i) buffer[i] = temp[i];
}