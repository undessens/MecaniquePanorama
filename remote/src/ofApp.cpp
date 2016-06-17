#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofAddListener(serial.NEW_MESSAGE, this, &ofApp::onNewMessage);
	serialInitiated = false;
	reconnectSerial();

	ofSetVerticalSync(false);

	ofSetFrameRate(120);

	sender.setup("localhost", 12345);

	

	impulse = false;
	isSending = false;
	impulseCount = 0;

	//Listener
	reset.addListener(this, &ofApp::resetVideo);
	reconnect.addListener(this, &ofApp::reconnectSerial);
	media.addListener(this, &ofApp::sendOscMessageMedia);

	gui.setup();

	gui.add(pause.setup("pause", false));
	gui.add(reset.setup("reset"));
	gui.add(reconnect.setup("serial"));
	gui.add(ratio.setup("imp/frame" ,1, 1, 10));
	gui.add(ratio2.setup("frame/imp", 1, 1, 10));
	gui.add(invert.setup("invert", false));
	gui.add(media.setup("media", 1, 1, 10));

	//TODO 
	/*
	Gerer la gestion des médias par message
	pourquoi pas des effets vidéos
	le transport
	
	calculer la vitesse de rotation
	decider du rapport entre le nombre d'impulsions 

	*/
	


}

//--------------------------------------------------------------
void ofApp::update(){

	int frame = ofGetFrameRate();
}



//--------------------------------------------------------------
void ofApp::draw(){

	if(serialInitiated){
		ofBackground(ofColor::green);
	}else{
		ofBackground(ofColor::gray);
	}
	gui.draw();

	if(!serialInitiated){
	ofSetColor(ofColor::red);
	ofDrawBitmapString("Serial not connected", 250, 30);
	}


	//Draw impulse
	if(impulse==1){

		int radius = 50;
		ofSetColor(ofColor::white);
		ofFill();
		ofCircle(300,100, radius);
		ofSetColor(ofColor::black);
		ofNoFill();
		ofCircle(300, 100, radius);

		impulse = 0;
		

	}
		if(impulse==2){

		int radius = 50;
		ofSetColor(ofColor::black);
		ofFill();
		ofCircle(300,100, radius);
		ofSetColor(ofColor::white);
		ofNoFill();
		ofCircle(300, 100, radius);

		impulse = 0;
		

	}

	//Draw OSC message
	if(isSending){

		ofSetColor(ofColor::red);
		ofFill();
		ofRect( 400, 70, 70, 70 );
		ofSetColor(ofColor::black);
		ofDrawBitmapString("OSC", 400, 105);

		isSending = false;
	}


}

//--------------------------------------------------------------
void ofApp::onNewMessage(string &msg){

	if(msg=="+"){
		
		if(invert)impulseNext();
		else impulsePrev();


	}
	if(msg=="-"){

		if(invert)impulsePrev();
		else impulseNext();

	}


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		switch (key)
	{
	case '+':
		if(invert)impulseNext();
		else impulsePrev();
		break;
	case '-':
		if(invert)impulsePrev();
		else impulseNext();
		break;
	case 'e':
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------
void ofApp::impulseNext(){
	impulse = 1;
	impulseCount++;

	if(impulseCount>=ratio){
		sendOscMessageTransport(NEXT);
		impulseCount = 0;
	}
}

//-------------------------------------------------------------
void ofApp::impulsePrev(){
	impulse = 2;
	impulseCount++;

		if(impulseCount>=ratio){
		sendOscMessageTransport(PREV);
		impulseCount = 0;
	}
	

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
//--------------------------------------------------------------
void ofApp::sendOscMessageTransport(int i){

	if(!pause){
	
	ofxOscMessage msg;
	
	msg.setAddress("/transport");
	msg.addIntArg(i);
	msg.addIntArg(ratio2);

	sender.sendMessage(msg);
	isSending = true;

	}


}

//--------------------------------------------------------------
void ofApp::sendOscMessageMedia(int &i){

	
	ofxOscMessage msg;
	
	msg.setAddress("/media");
	msg.addIntArg(i);
	
	if(ofGetFrameNum() > 800 )
	sender.sendMessage(msg);
	isSending = true;

	impulse = false;
	impulseCount = 0;

	


}


//--------------------------------------------------------------
void ofApp::reconnectSerial(){

	
	
	vector <ofSerialDeviceInfo> info = serial.getDeviceList();
	vector <ofSerialDeviceInfo>::iterator it;

	int num=0;

	for(it=info.begin(); it<info.end(); it++){

		if( it->getDeviceName() == "tty.usbmodemd1d181"){
			serialInitiated = serial.setup(num, 57600);
		}
		else{
			num++;
		}
	}

	serial.startContinuousRead(false);

	

}
//-------------------------------------------------------------
void ofApp::resetVideo(){

	sendOscMessageTransport(RESET);

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
