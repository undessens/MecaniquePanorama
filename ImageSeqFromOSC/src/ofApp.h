/**
 *
 *	ofxImageSequence example project
 *
 *  Created by James George ( http://www.jamesgeorge.org ) on 6/15/10.
 *  In collaboration with FlightPhase ( http://www.flightphase.com )
 *		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!) 
 *
 *  Copyright (c) 2010-2015
 */

#pragma once	

#include "ofMain.h"
#include "ofxImageSequence.h"
#include "ofxOsc.h"
#include <math.h>       /* pow */

#define IMGSIZE ( 1080)
#define EXPO (1.8f)



class ofApp : public ofBaseApp
{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void loadSequence( int i);
	void listNumSequence();
	
	ofxImageSequence sequence;
	ofImage background;
	ofxOscReceiver receiver;
	bool playingMouse;
	int indexFrame;
	int currentSequence;
	bool isFullScreen;
	int totalNumSequence;
    float smoothPos;
    int hardPos;
    float smoothAlpha;

	//Directory management
	ofDirectory dir;
	string path(string);

	//Transition, image presentation
	float loadingTime;
	float loadingStartTime;
	float loadingDuration;
	bool isLoading;
	vector<ofImage> listOfVignette;
	vector<int> listOfNbImage;
	ofImage imagePresentation;
    ofVideoPlayer vidPresentation;
    
    //Blur fbo
    ofShader shaderBlurX;
    ofShader shaderBlurY;
    
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;
    
    float blur;
    
};

