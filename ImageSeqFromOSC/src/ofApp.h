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
#include "ofxXmlSettings.h"
#include "ofxGLWarper.h"
#define IMGSIZEW (1920)
#define IMGSIZEH (1080)
#define EXPO (1.8f)
#define SCREEN_SAVER (350)



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
	void loadCurrentImage();
	
	ofxImageSequence sequence;
	ofImage background;
    ofImage currentImg;
	ofxOscReceiver receiver;
	ofxOscSender sender;
	int nbMsgReceived;
	bool playingMouse;
	int indexFrame;
	int lastIndexFrame;
	int currentSequence;
	bool isFullScreen;
	int totalNumSequence;
    float smoothPos;
    int hardPos;
    float smoothAlpha;
	bool isPrintFps;
	bool isTestMode;

	//Directory management
    ofxXmlSettings XML;
	ofDirectory dir;
	string path(string);

	//Transition, image presentation
	float loadingTime;
	float loadingStartTime;
	float loadingDuration;
	bool isLoading;
	vector<ofImage> listOfVignette;
    vector<ofImage> listOfIcon;
	vector<int> listOfNbImage;
    vector<int> listOfStartImage;
    vector<int> listOfNbDigit;
	vector<int> listOfCurrentFrame;


	ofImage imagePresentation;
    ofVideoPlayer vidPresentation;
    
    //Quad Warping
    ofxGLWarper warper;
	ofXml xml_warper;
    
    //Blur fbo
    ofShader shaderBlurX;
    ofShader shaderBlurY;
    
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;

	//Screen saver
	long double screenSaverTime;
    
    float blur;
    
};

