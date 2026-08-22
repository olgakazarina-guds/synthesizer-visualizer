#include "ofMain.h"
#include "ofApp.h"


int main(){
    // Create an OpenGL display window
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    
    // Launch the openFrameworks application
    ofRunApp(new ofApp());
}

