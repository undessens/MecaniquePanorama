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
 */



#include "ofApp.h"

std::string getCurrentUsername() {
	char username[256];

#ifdef _WIN32
	DWORD username_len = sizeof(username);
	if (GetUserName(username, &username_len)) {
		return std::string(username);
	}
	else {
		throw std::runtime_error("Unable to get username.");
	}
#else
	struct passwd* pw;
	uid_t uid = geteuid();
	pw = getpwuid(uid);
	if (pw) {
		return std::string(pw->pw_name);
	}
	else {
		throw std::runtime_error("Unable to get username.");
	}
#endif
}

//--------------------------------------------------------------
void ofApp::setup(){
	
	cout << "PRINT YOUR CURRENT OS : \n";
	//First of all : change the data path directory to another disk
#ifdef __APPLE__
	cout << "MAC OS CONFIGURATION \n";
    const string mydatapath = "/Volumes/FAT32/MecaniquePanorama";
    dir = ofDirectory(mydatapath);
    ofSetDataPathRoot(mydatapath);
#elif _WIN32
	cout << "WINDOWS CONFIGURATION \n";
	std::string username = getCurrentUsername();
	filesystem::path mydatapath = "C:/Users/" + username + "/Documents/MecaniquePanorama/ImageSeqFromOSC/bin/data";
	dir = ofDirectory(mydatapath);
	ofSetDataPathRoot(mydatapath);
#else
	cout << "LINUX CONFIGURATION \n";	
	const string mydatapath = "/media/nano/FAT32/MecaniquePanorama/";
 	dir = ofDirectory(mydatapath);
	ofSetDataPathRoot(mydatapath);
#endif

    
    //Out the current directory
    cout << "\n Directory of MP data ( video + image ) : " + dir.getOriginalDirectory();
    cout << "\n nb file : " + ofToString(dir.listDir()) + "\n";


    // OSC-config
	receiver.setup(12345);
	nbMsgReceived = 0;
    
    // FrameRate ?
    //ofSetFrameRate(30);
	ofSetVerticalSync(true);
	isPrintFps = false;
	isTestMode = false;
    
    //Video Presentation

	
	string absouluteVideoPath = path("intro.mp4");
	cout << "\n chargement video " + absouluteVideoPath ;
	ofFile myFile;
	if (myFile.doesFileExist(absouluteVideoPath, false)) {
		cout << " Le fichier video existe\n ";
	}

	vidPresentation.load(absouluteVideoPath);
	//vidPresentation.load("intro.mp4");
	if (!vidPresentation.isLoaded()) {
		cout << "\n Erreur chargement video : path : ";
		cout << absouluteVideoPath + "\n";
	}
    vidPresentation.setLoopState(OF_LOOP_NORMAL);
    vidPresentation.stop();
    
    // Load xml file from directory scanning
	if (XML.load(dir.getAbsolutePath() + "/scan.xml")) {
		cout << "\n chargement du fichier XML ";
	}
	else {
		cout << "\n ERREUR du fichier XML ";
	}
    

    // Image sequence init
	currentSequence = 0;
	isLoading = false;
	loadingDuration = 2;
	
	/*******************************
	Load threaded load ( only using preload frames )
	********************************/
	//sequence.enableThreadedLoad(true);
	sequence.enableThreadedLoad(false);

	/*******************************
	List Num Sequence ( 2nd security with scan.xml that list all sequences )
	********************************/
	cout << "\n list Num Frames ";
	listNumSequence();

	/*******************************
	Load sequence 0 ( conflict with arduino that start sequence 1 at start )
	********************************/
	cout << "\n Load sequence 0 ";
	loadSequence(0);

	/*******************************
	Screen saver 
	********************************/
	screenSaverTime = 0;

	/*******************************
	index frame = 0
	********************************/
	indexFrame = 0;
	
	/*******************************
	Mouse playing : not used
	********************************/
	playingMouse = false; //controls if playing automatically, or controlled by the mouse
    smoothPos = 0;
    smoothAlpha = 0.;
    
	/*******************************
	Blur fbo
	********************************/
#ifdef TARGET_OPENGLES
    shaderBlurX.load(path("shadersES2/shaderBlurX"));
    shaderBlurY.load(path("shadersES2/shaderBlurY"));
#else
    if(ofIsGLProgrammableRenderer()){
        shaderBlurX.load(path("shadersGL3/shaderBlurX"));
        shaderBlurY.load(path("shadersGL3/shaderBlurY"));
    }else{
        shaderBlurX.load(path("shadersGL2/shaderBlurX"));
        shaderBlurY.load(path("shadersGL2/shaderBlurY"));
    }
#endif
    fboBlurOnePass.allocate(IMGSIZEW, IMGSIZEH);
    fboBlurTwoPass.allocate(IMGSIZEW, IMGSIZEH);
    blur = 0.0f;
    
	/*******************************
	Quad warper
	********************************/
	warper.setup(0, 0, IMGSIZEW, IMGSIZEH);
	warper.load(path("corner_settings.xml"));
	//warper.activate();

	/*******************************
	Hide Mouse
	********************************/
	ofHideCursor();

    

}

//--------------------------------------------------------------
void ofApp::update(){
	
	/*******************************
	OSC receive message
	********************************/
	nbMsgReceived = 0;
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		nbMsgReceived++;

		//Reset screen saver when osc message is received
		screenSaverTime = ofGetElapsedTimef();
        
		// check for mouse moved message
		if(m.getAddress() == "/transport/next"){

			int step = m.getArgAsInt(0) ;
			if (indexFrame <  (sequence.getTotalFrames()-step) ){
				indexFrame+=step;
			}
			else{
				indexFrame = 0;
			}
			
			
		}
		if(m.getAddress() == "/transport/previous"){

			int step = m.getArgAsInt(0);
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
			int newSeq = m.getArgAsInt(0);
			loadSequence(newSeq);
		}
	}

	/******************************************************************
	Update Screen saver : #define SCREEN_SAVER (350)
	********************************************************************/
	if ((ofGetElapsedTimef() - screenSaverTime) > SCREEN_SAVER && currentSequence>0) {
		ofLogNotice("Screen saver is starting");

		loadSequence(0);
	}

	/******************************************************************
	Update Loading time ( blur amount + title image ( intro.png ) )
	********************************************************************/
	loadingTime = ofGetElapsedTimef() - loadingStartTime;

	/******************************************************************
	Update blur amount ( initiated in loadSequence() ), then decreased here
	********************************************************************/
    if(loadingTime > loadingDuration){
	    if(blur < 0.3 ){
			isLoading = false;
			blur = 0;
		}
	    else{
	        blur = blur * 0.98;
	    }
	}
    
	/**********************************************************
	If no sequence ( sequence = 0 ), update video presentation
	*************************************************************/	
    if (currentSequence == 0 ){
        vidPresentation.update();
    }


	/**********************************************************
	Test mode. Increase frame one by one to test the real performance
	*************************************************************/

	if (currentSequence > 0 && isTestMode) {
		if (indexFrame < (sequence.getTotalFrames()-1 )) {
			indexFrame++;
		}
		else {
			indexFrame = 0;
		}
	}
		
		



}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(0);

	/***********************************
	 Images sequence is actually playing
	**********************************/
	if ( currentSequence > 0){

		ofBackground(0);
		ofSetColor(255);
			

		/***********************************
			Draw : 1 . Fbo blur 1st pass
		**********************************/
        fboBlurOnePass.begin();
        shaderBlurX.begin();
        shaderBlurX.setUniform1f("blurAmnt", blur);
        
		/**************************************************
			Draw : 2 . Draw sequence from mouse or index frame
		***************************************************/
        if(playingMouse){
			float percent = ofMap(mouseX, 0, ofGetWidth(), 0.0, 1.0, true);
            indexFrame = sequence.getFrameIndexAtPercent(percent);
            if(indexFrame==0 || indexFrame != lastIndexFrame)  loadCurrentImage();
			
            currentImg.draw(0, 0, IMGSIZEW, IMGSIZEH);
	    lastIndexFrame = indexFrame;
        }
        else{
			if (indexFrame == 0 || indexFrame != lastIndexFrame) {
				loadCurrentImage();
			}
	    currentImg.draw(0, 0, IMGSIZEW, IMGSIZEH);
		//sequence.getTextureForFrame(indexFrame).draw(0, 0, IMGSIZEW, IMGSIZEH);
		lastIndexFrame = indexFrame;
        }
        shaderBlurX.end();
        fboBlurOnePass.end();
            
		/***********************************
			Draw : 3 . Fbo blur 2nd pass
		********************************/
        fboBlurTwoPass.begin();
        shaderBlurY.begin();
        shaderBlurY.setUniform1f("blurAmnt", blur);
        fboBlurOnePass.draw(0, 0);
        shaderBlurY.end();
        fboBlurTwoPass.end();
            
		/***********************************
			Draw : 4 . Draw fbo inside quad warper
		********************************/
		warper.begin();
		fboBlurTwoPass.draw(0, 0, ofGetWidth(), ofGetHeight());
		
		/***********************************
			Draw : 5 . Draw Vignette if loading
		********************************/
		if (isLoading) {

			ofSetColor(255);
			ofEnableAlphaBlending();
			listOfVignette[currentSequence - 1].draw(0, 0, ofGetWidth(), ofGetHeight());
			ofDisableAlphaBlending();

		}
        /***********************************
         Draw : 6 . Draw Icon number everytime
         ********************************/
            //ofSetColor(255);22222ofDisableAlphaBlending();
        
		/***********************************
			Draw : 7 . Draw warper corner if needed
		********************************/
		if (warper.isActive()) {
			warper.draw();
		}
		/***********************************
			Draw : 8 . Draw FPS if needed
		********************************/
		if (isPrintFps) {
			string fps = "fps: " + ofToString(ofGetFrameRate());
			ofDrawBitmapStringHighlight(fps, ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), ofColor::black, ofColor::white);
			ofDrawBitmapStringHighlight(ofToString(indexFrame), ofVec2f(ofGetWidth() / 2, (ofGetHeight() / 2) - 25), ofColor::black, ofColor::white);
			ofDrawBitmapStringHighlight(ofToString(nbMsgReceived), ofVec2f(ofGetWidth() / 2, (ofGetHeight() / 2) + 25), ofColor::black, ofColor::white);
		}
	
		warper.end();
		

	}
    else
    {
		/******************************************************
		If no sequence ( sequence = 0 ), draw video presentation
		********************************************************/
		warper.begin();
		vidPresentation.draw(0, 0, ofGetWidth(), ofGetHeight());
		if (warper.isActive()) {
			warper.draw();
		}
		warper.end();
        
	}
    


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//hit any key to toggle playing

	//Reset screen saver when key is pressed
	screenSaverTime = ofGetElapsedTimef();

	switch(key){

		case 'm': playingMouse = !playingMouse;
        smoothPos = ofMap(mouseX, 0, ofGetWidth(), 0, 1.0, true);
		break;
		case 'f': isPrintFps = !isPrintFps;
		break;
		case 't': isTestMode = !isTestMode;
			break;
        case 'w':
			if (warper.isActive()) {
				warper.deactivate();
				ofHideCursor();
				warper.saveToXml(xml_warper);
				ofLogNotice(xml_warper.toString());
				//xml.setName("war_settings");
				if (!xml_warper.save(path("corner_settings.xml"))) {
					ofLogError() << "Couldn't save points.xml";
				}
			}
			else {
				warper.activate();
				ofShowCursor();
			}
        break;
		case '1':loadSequence(1);
		break;
		case '2':loadSequence(2);
		break;
		case '3':loadSequence(3);
		break;
		case '4':loadSequence(4);
		break;
		case '5':loadSequence(5);
		break;
		case '0':loadSequence(0);
		break;
	}
	
}

//--------------------------------------------------------------
void ofApp::loadSequence(int num){

	/**************************
		Inform all the sequence changed
	***************************/


    /*
	if (sender.setup("192.168.1.255", 9999)) {
		ofxOscMessage msg;
		msg.setAddress("/sequence");
		msg.addInt32Arg(num);
		sender.sendMessage(msg);
		ofLogNotice("Sequence changed " + ofToString(num) + " : send osc message");
	}
     */


	
	/**************************
		Load an sequence of images
	***************************/
	if( num > 0 ){
        
        if(sequence.isLoaded()){
			sequence.cancelLoad();
            sequence.unloadSequence();
        }
        if(num > totalNumSequence){
            num = totalNumSequence;
        }

		/*************************
		STOP VIDEO PRESENTATION 
		****************************/
		vidPresentation.stop();

		/**************************
		Choose the folder of images
		***************************/
       		// string folderPath = path(ofToString(num)) +"/processed/";
		string folderPath = path(ofToString(num)) + "/";

		/**************************
		Choose Extension
		***************************/
		sequence.loadSequence(folderPath,
                              "jpg",
							  // "png",
							//"tif",
                              listOfStartImage[num-1],
                              listOfStartImage[num-1] + listOfNbImage[num-1],
                              listOfNbDigit[num-1]);

		
        /**************************
		This preload all frame in an other thread if :sequence.enableThreadedLoad(true);
		***************************/
		//sequence.preloadAllFrames();

        loadingTime = 0;
        loadingStartTime = ofGetElapsedTimef();
        isLoading = true;

		//Save current position of frame
		if (currentSequence > 0) {
			listOfCurrentFrame[currentSequence - 1] = indexFrame;
		}
        currentSequence = num;
		indexFrame = listOfCurrentFrame[num-1];
		lastIndexFrame = 0;
        blur = 2.0f;
        cout << "\n  load this sequence " << ofToString(num);
		if (sequence.isLoading()) {
			cout << "  Actually Loading \n " << ofToString(num);
		}
		else {
			cout << "  Error, not loading \n " << ofToString(num);
		}
        
        
    } 
	/**************************
		Video presentation ( sequence 0 )
	***************************/
    else if (num == 0){

		cout << "\n Image de demarrage" ;
			currentSequence = 0;
			vidPresentation.setLoopState(OF_LOOP_NORMAL);
			vidPresentation.firstFrame();
			vidPresentation.play();
		} else {

		cout << "\n Not allowed to load this sequence " << ofToString(num);

    }


}

//--------------------------------------------------------------
void ofApp::loadCurrentImage(){
	string filename = sequence.filenames[indexFrame];
            currentImg.load(filename);
}

//--------------------------------------------------------------
void ofApp::listNumSequence(){
	//start this function at the beggining, calculate the number of
	//sequence, according to the scan.xml file

	cout<< "\n *** List Num Sequence : working directory : "+dir.getAbsolutePath()+"\n";
	totalNumSequence = 0;

	dir.listDir();
	for (int i = 1; i < 20; i++) {

		if (dir.doesDirectoryExist(path(ofToString(i)))){
			totalNumSequence++;
		}

	}

	cout << "\n NUM OF SEQUENCE = " + ofToString(totalNumSequence)+"\n";

	XML.pushTag("SCAN");

	for (int i = 1; i <(totalNumSequence+1); i++ ){

		ofImage img ;
		// scanning files from OF is extremely long
        ofDirectory dirOfSeq = ofDirectory(path(ofToString(i)));
		//int nbImage = dirOfSeq.listDir() - 2;
        int nbImage = XML.getValue("SEQ:NBFILES", 0, i-1);
        int startImage = XML.getValue("SEQ:START", 0, i-1);
        int nbDigit = XML.getValue("SEQ:NBDIGIT", 0, i-1);
		
		cout<< "\n Folder nb :";
		cout << ofToString(i);
	    cout<< " nb of Image : ";
	    cout<< ofToString(nbImage);
        cout<< " Start Image : ";
        cout<< ofToString(startImage);
        cout<< " Nb of Digit : ";
        cout<< ofToString(nbDigit);
		
		listOfNbImage.push_back(nbImage);
        listOfStartImage.push_back(startImage);
        listOfNbDigit.push_back(nbDigit);
		listOfCurrentFrame.push_back(0);

		if( !img.load(dirOfSeq.getAbsolutePath()+"/intro.png")){
			img.allocate(1280, 720 , OF_IMAGE_COLOR);
		}
		listOfVignette.push_back(img);
        
        ofImage icon ;
        if( !icon.load(path("icon/"+ofToString(i)+".png"))) {
            icon.allocate(200, 200 , OF_IMAGE_COLOR);
        }
        listOfIcon.push_back(icon);
        
        
        
	}

	cout << "\n end of scanning files \n ";

}

//--------------------------------------------------------------
// This function replace ofSetDataPathRoot, which is not working on windows with a change of disk form C: to D:
string ofApp::path(string p) {

	return dir.getAbsolutePath() + "/" + p;
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

	// ON right click
	if (button == 2) {
		// Both right and left click : quit app
		if (ofGetMousePressed(0)) {
			ofExit();
		}
		else {
			if (warper.isActive()) {
				ofHideCursor();
				warper.deactivate();
				warper.saveToXml(xml_warper);
				ofLogNotice(xml_warper.toString());
				//xml.setName("war_settings");
				if (!xml_warper.save(path("corner_settings.xml"))) {
					ofLogError() << "Couldn't save points.xml";
				}
			}
			else {
				ofShowCursor();
				warper.activate();
			}

		}
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

