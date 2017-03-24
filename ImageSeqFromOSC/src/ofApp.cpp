/**
 *  Mecanique Panorama
 * Association Un Des Sens
 * github.com/undessens/mecanique_panorama
 *
 *  Inspired from
 *	ofxImageSequence example project
 *
 * Created by James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
 *		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!) 
 *
 *
 * ----------------------
 *
 * simple ofxImageSequence example using Muybridge horse
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//First of all : change the data path directory to another disk
#ifdef __APPLE__
   // ofSetDataPathRoot("1/");
#else
	ofSetDataPathRoot("/media/conilux/Data/8FabLab/");
#endif


    // OSC-config
	receiver.setup(12345);
    
    // FrameRate ?
    //ofSetFrameRate(60);
    
    //Video Presentation
    vidPresentation.load("intro.mp4");
    vidPresentation.setLoopState(OF_LOOP_NORMAL);
    vidPresentation.stop();

    // Image sequence init
	currentSequence = 1;
	isLoading = false;
	loadingDuration = 2.5;
	sequence.enableThreadedLoad(true);

    // Really usefull ?
	listNumSequence();
 #ifdef __APPLE__
	loadSequence(1);
#else
    // On linux, load the menu - video presentation
    loadSequence(0);
#endif
	
	indexFrame = 0;
	sequence.preloadAllFrames();	//this way there is no stutter when loading frames
	
	playingMouse = false; //controls if playing automatically, or controlled by the mouse
	isFullScreen = false;
    smoothPos = 0;
    smoothAlpha = 0.;
    
    //Blur fbo
#ifdef TARGET_OPENGLES
    shaderBlurX.load("shadersES2/shaderBlurX");
    shaderBlurY.load("shadersES2/shaderBlurY");
#else
    if(ofIsGLProgrammableRenderer()){
        shaderBlurX.load("shadersGL3/shaderBlurX");
        shaderBlurY.load("shadersGL3/shaderBlurY");
    }else{
        shaderBlurX.load("shadersGL2/shaderBlurX");
        shaderBlurY.load("shadersGL2/shaderBlurY");
    }
#endif
    fboBlurOnePass.allocate(1920, 1080);
    fboBlurTwoPass.allocate(1920, 1080);
    blur = 0.0f;


}

//--------------------------------------------------------------
void ofApp::update(){
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/transport/next"){

			int step = m.getArgAsInt(0) +1;

			if (indexFrame <  (sequence.getTotalFrames()-step) ){

				indexFrame+=step;
			}
			else{

				indexFrame = 0;
			}
		}
		if(m.getAddress() == "/transport/previous"){

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
		if(m.getAddress() == "/transport/changeSeq" ){

			int newSeq = m.getArgAsInt(0) ;

			loadSequence(newSeq);



		}


	}
    
    if(playingMouse) hardPos = mouseX;
    
    smoothPos = (1.0f-smoothAlpha)*hardPos+ (smoothAlpha)*smoothPos;


	//loading, image presentation ( currentSequence > 0 )
	loadingTime = ofGetElapsedTimef() - loadingStartTime;
	
    if(loadingTime > loadingDuration){
	    if(blur < 0.3 ){

			isLoading = false;
			blur = 0;
		}
	    else{
	        blur = blur * 0.993;
	    }
	}
    
    //Video presentation
    if (currentSequence == 0 ){
        vidPresentation.update();
    }




}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if ( currentSequence > 0){

		

		
		
			ofBackground(0);
			ofSetColor(255);
				//get the frame based on the current time and draw it
				//get the sequence frame that maps to the mouseX position

			float percent = ofMap(mouseX, 0, ofGetWidth(), 0.0, 1.0, true);
				//draw it.
            //----------------------------------------------------------
			
            fboBlurOnePass.begin();
            
            shaderBlurX.begin();
            shaderBlurX.setUniform1f("blurAmnt", blur);
            
            if(playingMouse){
            sequence.getTextureForPercent(percent).draw(0, 0, ofGetWidth(), ofGetHeight());
            }
            else{
            sequence.getTextureForFrame(indexFrame).draw(0, 0, ofGetWidth(), ofGetHeight());	
            }

            shaderBlurX.end();
            
            fboBlurOnePass.end();
            
            //----------------------------------------------------------
            fboBlurTwoPass.begin();
            
            shaderBlurY.begin();
            shaderBlurY.setUniform1f("blurAmnt", blur);
            
            fboBlurOnePass.draw(0, 0);
            
            shaderBlurY.end();
            
            fboBlurTwoPass.end();
            
            //----------------------------------------------------------
            //sequence.getTextureForFrame(indexFrame).draw(0, 0);
            fboBlurTwoPass.draw(0, 0);
				
            //sequence.getTextureForPercent(percent).
                //debug
                //ofDrawBitmapString(ofToString(percent*100)+"%", ofGetWidth()/2, ofGetHeight()/2);
        
        
        if(sequence.isLoading() || isLoading){
            //ofBackground(255,0,0);
            ofSetColor(255);
            ofEnableAlphaBlending();
            listOfVignette[currentSequence-1].draw(0,0);
            ofDisableAlphaBlending();
            
        }
				
		

	} else {
		// Image  presentation
		//imagePresentation.draw(0,0);
        
        // Video presentation
        vidPresentation.draw(0, 0, ofGetWidth(), ofGetHeight());

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

if( num > 0 ){
//if( num > 0 && !sequence.isLoading() ){
	
	if(sequence.isLoaded()){

		sequence.unloadSequence();
  }
  

	string path = ofToString(num)+"/";

	switch(num){


		case 1: sequence.loadSequence(path, "jpg", 0, 10352, 6);
		break;
		case 2: sequence.loadSequence(path, "jpg",0, 13261, 6 );
		break;
		case 3: sequence.loadSequence(path, "jpg",0, 12947, 6 );
		break;
		case 4: sequence.loadSequence(path, "jpg",0, 12551, 6 );


	}
	sequence.preloadAllFrames();
	loadingTime = 0;
	loadingStartTime = ofGetElapsedTimef();
	isLoading = true;
	currentSequence = num;
    blur = 2.0f;
    cout << "\n  load this sequence " << ofToString(num);
	
	
} else if (num == 0){

cout << "\n Image de demarrage" ;
    currentSequence = 0;
    vidPresentation.firstFrame();
    vidPresentation.play();

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
        //if( !img.load(ofToString(i)+size+"/intro.png")){

		if( !img.load(ofToString(i)+"/intro.png")){
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

