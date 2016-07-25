#include "ofApp.h"




void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //consoleListener.setup(this);
	ofHideCursor();
	
    doPause = false;
    doRestart = false;
    doSeek = false;
    doIncreaseSpeed = false;
    doSetNormalSpeed = false;
    doFrameStep = false;

    receiver.setup(12345);
    
    
    
	string videoPath = ofToDataPath("vid1.mov", true);
	
	settings.videoPath = videoPath;
	settings.useHDMIForAudio = true;	//default true
	settings.enableLooping = true;		//default true
	settings.enableTexture = (ofGetUnixTime() % 2 == 0);		//default true
	//settings.enableTexture = false;		//default true
	omxPlayer.setup(settings);

	ofSetFrameRate(120);
	
}


void ofApp::update()
{
  while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		ofLog(OF_LOG_VERBOSE, "OSC message ");
		
        
		// check for mouse moved message
		if(m.getAddress() == "/transport/setFrame"){
			
			int frame = m.getArgAsInt32(0) % omxPlayer.getTotalNumFrames();
			omxPlayer.stepFrame(frame);
			ofLog(OF_LOG_VERBOSE, " SetFrame %d", frame);
			if(omxPlayer.isPaused()){
			    ofLog(OF_LOG_VERBOSE, " Pause!");
			  }
		}
		if(m.getAddress() == "/transport/nextFrame"){
			// both the arguments are int32's
			int addedFrame = m.getArgAsInt32(0) ;
	       		omxPlayer.stepFrame(omxPlayer.getCurrentFrame()+addedFrame);
			ofLog(OF_LOG_VERBOSE, " AddFrame %d", addedFrame);
		}
		if(m.getAddress() == "/transport/prevFrame"){
			// both the arguments are int32's
			int removedFrame = m.getArgAsInt32(0);
			omxPlayer.stepFrame(omxPlayer.getCurrentFrame()-removedFrame);
			ofLog(OF_LOG_VERBOSE, " Removoed Frame %d", removedFrame);
		}
		if(m.getAddress() == "/transport/restart"){
			// both the arguments are int32's
			omxPlayer.restartMovie();
			ofLog(OF_LOG_VERBOSE, " Restart");
		}
		if(m.getAddress() == "/transport/togglePause"){
			// both the arguments are int32's
			omxPlayer.togglePause();
			ofLog(OF_LOG_VERBOSE, " Toogle pause");
		}
		if(m.getAddress() == "/transport/setPause"){
			// both the arguments are int32's
		        int isPause = m.getArgAsInt32(0) ;
			omxPlayer.setPaused((isPause==1));
			ofLog(OF_LOG_VERBOSE, " Toogle pause");
		}
		if(m.getAddress() == "/transport/togglePause"){
			// both the arguments are int32's
			omxPlayer.togglePause();
			ofLog(OF_LOG_VERBOSE, " Toogle pause");
		}
		
  }

 

    
}

void ofApp::draw()
{
    
    
    ofBackgroundGradient(ofColor::red, ofColor::black);
    omxPlayer.draw(0, 0);
    
    stringstream info;
    info << omxPlayer.getInfo() << endl;
    info << endl;
    info << "COMMANDS" << endl;
    info << "PRESS p TO PAUSE" << endl;
    info << "PRESS f TO STEP FRAME" << endl;
    info << "PRESS r TO RESTART" << endl;
    info << "PRESS i TO INCREASE SPEED" << endl;
    info << "PRESS n FOR NORMAL SPEED" << endl;
    info << "PRESS s FOR RANDOM SEEK" << endl;
	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}


void ofApp::keyPressed  (int key){
	 
	ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	switch (key) 
	{
		
        case 'p':
        {
            doPause = true;
            break;
        }
        case 'r':
        {
            doRestart = true;
            break;
        }
        case 'i':
        {
            doIncreaseSpeed = true;
            break;
        }
        case 'n':
        {
            doSetNormalSpeed = true;
            break;
        }
        case 'f':
        {
            doFrameStep = true;
            break;
        }
        case 's':
        {
            doSeek = true;
            break;
        }
		default:
		{
			break;
		}	
	}
	
}
