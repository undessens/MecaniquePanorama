#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(false);
	ofSetFrameRate(80);


	player.loadMovie("media/1.mov");
	player.setPaused(true);

	isFullScreen = false;
	isDebug = true;

	//OSC
	receiver.setup(12345);


}

//--------------------------------------------------------------
void ofApp::update(){

	player.update();

	if(receiver.hasWaitingMessages()){

		ofxOscMessage msg;
		receiver.getNextMessage(&msg);

		if(msg.getAddress() == "/transport") useTransportMessage(msg);

		if(msg.getAddress() == "/media" ) useMediaMessage(msg);

		

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(ofColor::white);

	player.draw(0,0, ofGetWidth(), ofGetHeight());

	if(isDebug)
	ofDrawBitmapStringHighlight(ofToString(player.getCurrentFrame()), ofGetWidth()/2, ofGetHeight()/2, ofColor::white, ofColor::black);


}

//--------------------------------------------------------------
void ofApp::useTransportMessage(ofxOscMessage msg){

	int transp = msg.getArgAsInt32(0);
	int value = msg.getArgAsInt32(1);

	
	switch (transp)
	{
	case PREV:
		for(int i=0; i<value; i++){
				if(player.getCurrentFrame()==0){
					player.setFrame(player.getTotalNumFrames()-1);
				}else{	
					player.previousFrame();
				}
	
		}
		break;
	case NEXT:
		for(int i=0; i<value; i++){
			
			player.nextFrame();
			if(player.getIsMovieDone()){
				player.firstFrame();
			}


		}		
		break;
	case RESET:
		player.firstFrame();
	
	default:
		break;
	}


}

//--------------------------------------------------------------
void ofApp::useMediaMessage(ofxOscMessage msg){

	int numMedia = msg.getArgAsInt32(0);

	if(numMedia>0 && numMedia<= NUMMEDIA){

		player.closeMovie();
		if(numMedia<5)
		player.loadMovie("media/"+ofToString(numMedia)+".mov");
		if(numMedia>4)
		player.loadMovie("media/"+ofToString(numMedia)+".mp4");
		player.firstFrame();
		player.setPaused(true);

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'f':
		isFullScreen = !isFullScreen;
		ofSetFullscreen(isFullScreen);
	break;
	case 'd':
		isDebug = !isDebug;
	break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
