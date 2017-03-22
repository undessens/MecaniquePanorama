/**
 *  ofApp.cpp
 *
 *	ofxImageSequence example project
 *
 * Created by James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
 *		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!) 
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * simple ofxImageSequence example using Muybridge horse
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//First of all : change the data path directory to another disk
	//ofSetDataPathRoot("1/");
	


	//File format for the example frames is
	//frame01.png 
	//this creates a method call where the parameters
	//prefix is frame, file type is png, from frame 1 to 11, 2 digits in the number
	receiver.setup(12345);
    ofSetFrameRate(60);

	currentSequence = 1;
	isLoading = false;
	loadingDuration = 2.5;





	sequence.enableThreadedLoad(true);

	listNumSequence();
	loadSequence(1);
	
	indexFrame = 0;
	//sequence.preloadAllFrames();	//this way there is no stutter when loading frames

	//sequence.loadSequence("Decoup-test-sqjpeg");

	//sequence.setFrameRate(10); //set to ten frames per second for Muybridge's horse.
	
	playingMouse = false; //controls if playing automatically, or controlled by the mouse
	isFullScreen = false;
    smoothPos = 0;
    smoothAlpha = 0.;


}

//--------------------------------------------------------------
void ofApp::update(){
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/transport/next" && !isLoading){

			int step = m.getArgAsInt(0) +1;

			if (indexFrame <  (sequence.getTotalFrames()-step) ){

				indexFrame+=step;
			}
			else{

				indexFrame = 0;
			}
		}
		if(m.getAddress() == "/transport/previous" && !isLoading){

			int step = m.getArgAsInt(0) +1;

			if( indexFrame > step){

				indexFrame-=step;
			}
			else{

				indexFrame = sequence.getTotalFrames();
			}
		}
        if(m.getAddress() == "/transport/percent" && !isLoading){
            
            int pos = (1.0 - m.getArgAsFloat(0)) * ofGetWidth();
            
            hardPos = pos;
        }
		if(m.getAddress() == "/transport/changeSeq" && !isLoading){

			int newSeq = m.getArgAsInt(0) ;

			loadSequence(newSeq);



		}


	}
    
    if(playingMouse) hardPos = mouseX;
    
    smoothPos = (1.0f-smoothAlpha)*hardPos+ (smoothAlpha)*smoothPos;


	//loading, image presentation
	loadingTime = ofGetElapsedTimef() - loadingStartTime;
	if(loadingTime > loadingDuration){

		isLoading = false;
	}




}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if ( currentSequence > 0){

		if(sequence.isLoading() || isLoading){
			ofBackground(255,0,0);
			ofSetColor(255);
			listOfVignette[currentSequence-1].draw(0,0,ofGetWidth(), ofGetHeight());

		}
		else{
			ofBackground(0);
			ofSetColor(255);
				//get the frame based on the current time and draw it
				//get the sequence frame that maps to the mouseX position
				float percent = ofMap(smoothPos, 0, ofGetWidth(), 0, 1.0, true);
			
				//draw it.
				sequence.getTextureForPercent(percent).draw(0, 0, ofGetWidth(), ofGetHeight());
                //debug
                //ofDrawBitmapString(ofToString(percent*100)+"%", ofGetWidth()/2, ofGetHeight()/2);
				
		}

	} else {
		// Image de presentation
		imagePresentation.draw(0,0);

	}


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//hit any key to toggle playing

	switch(key){

		case 'm': playingMouse = !playingMouse;
        smoothPos = ofMap(mouseX, 0, ofGetWidth(), 0, 1.0, true);
		break;
		case 'f': isFullScreen = !isFullScreen;
		ofSetFullscreen( isFullScreen);
		break;
		case '1':loadSequence(1);
		break;
		case '2':loadSequence(2);
		break;
		case '3':loadSequence(3);
		break;
		case '4':loadSequence(4);
		break;
		case '0':loadSequence(0);
		break;
	}
	
}

//--------------------------------------------------------------
void ofApp::loadSequence(int num){


if( num > 0 && !sequence.isLoading() ){
	
	if(sequence.isLoaded()){

		sequence.unloadSequence();
	}

	string path = ofToString(num)+"/";

	switch(num){

		case 1: sequence.loadSequence(path, "jpg", 11608, 12236, 6);
		break;
		case 2: sequence.loadSequence(path, "jpg",0, 12817, 6 );
		break;
		case 3: sequence.loadSequence(path, "jpg",0, 12551, 6 );
		break;
		case 4: sequence.loadSequence(path, "jpg",0, 2049, 5 );
		break;


	}

	loadingTime = 0;
	loadingStartTime = ofGetElapsedTimef();
	isLoading = true;
	currentSequence = num;
	
	
} else if (num == 0){

cout << "\n Image de demarrage" ;
currentSequence = 0;

} else {

cout << "\n Not allowed to load this sequence " << ofToString(num);

}


}

//--------------------------------------------------------------
void ofApp::listNumSequence(){
	//start this function at the beggining, calculate the number of
	//sequence, according to number of foler placed in data folder

	cout<< "\n List Num Sequence : working directory";
	cout<< ofToDataPath("", true);


	ofDirectory dir = ofDirectory("");
	totalNumSequence = 1;

	string size = "/"+ofToString(IMGSIZE);
	imagePresentation.load("titre"+ofToString(IMGSIZE)+".jpg");

	while ( dir.doesDirectoryExist( ofToString(totalNumSequence))){

		totalNumSequence++;
	}

	for (int i = 1; i <(totalNumSequence); i++ ){

		ofImage img ;
		if( !img.load(ofToString(i)+size+"/intro.jpg")){
			img.allocate(1280, 720 , OF_IMAGE_COLOR);
		}
		listOfVignette.push_back(img);
	}

	cout<< "\n nb of sequence :" << ofToString(totalNumSequence);


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

