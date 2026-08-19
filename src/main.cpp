#include "ofMain.h"
#include "ofApp.h"

// ==============================================================================
// main.cpp
// Main program entry point for openFrameworks C++ application.
//
// 1. Sets up the OpenGL graphics window (1024x768 pixels).
// 2. Instantiates ofApp and starts the main application loop.
// ==============================================================================

int main(){
    // Create an OpenGL display window
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    
    // Launch the openFrameworks application
    ofRunApp(new ofApp());
}

