#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "ofxOsc.h"

#include "TerminalListener.h"

class ofApp : public ofBaseApp, public KeyListener{
	
public:
	
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	
	ofxOMXPlayer omxPlayer;
	ofxOMXPlayerSettings settings;

	ofxOscReceiver receiver;
		
	
	//allows key commands via Shell
    void onCharacterReceived(KeyListenerEventData& e)
    {
        keyPressed((int)e.character);
    };
    
	TerminalListener consoleListener;
    
    
    bool doPause;
    bool doRestart;
    bool doSeek;
    bool doIncreaseSpeed;
    bool doSetNormalSpeed;
    bool doFrameStep;

};

