#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSimpleSerial.h"
#include "ofxGui.h"

#define NEXT 1
#define PREV 2
#define RESET 3


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void onNewMessage(string &msg);
		void sendOscMessageTransport(int i);
		void sendOscMessageMedia(int &i);
		void resetVideo();
		void reconnectSerial();
		void impulseNext();
		void impulsePrev();

		//OSC
		ofxOscSender sender;
		bool isSending;

		//Serial
		ofxSimpleSerial serial;
	
		bool serialInitiated;

		//Math
		int impulseCount;
		int impulse;  //  0 = rien   1 = sens pour incrémenter  2 = sens pour décrémenter

		//Gui
		ofxPanel  gui;
		ofxButton reset;
		ofxToggle pause;
		ofxToggle invert;
		ofxButton reconnect;
		ofxIntSlider ratio;
		ofxIntSlider ratio2;
		ofxIntSlider media;






		
		
};
