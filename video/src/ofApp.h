#pragma once

#include "ofMain.h"
#include "ofxOSC.h"

#define NUMMEDIA 9
//Git test

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
		void changeMedia(int i);
		void resetMedia();


		ofVideoPlayer player;

		//OSC
		ofxOscReceiver receiver;
		void useTransportMessage(ofxOscMessage msg);
		void useMediaMessage(ofxOscMessage msg);

		//screen
		bool isFullScreen;
		bool isDebug;

		
};
