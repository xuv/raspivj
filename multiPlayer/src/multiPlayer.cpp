#include "multiPlayer.h"

//--------------------------------------------------------------
void multiPlayer::setup()
{
	
	ofHideCursor();
	
	ofSetLogLevel(OF_LOG_ERROR);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetVerticalSync(false);

	// OSC setup
	
	// listen on the given port
	cout << "Listening for incoming osc messages on port " << PORT << "\n";
	receiver.setup(40000);
	
	// OMX Player setup
	
	shader.load("shaderExample");
	alpha = 0.5;
	black = 0;
	
	// initialize the 2 videos
	index0 = 0;
	index1 = 1;

	ofDirectory currentVideoDirectory(ofToDataPath("/home/pi/raspivj/videos", true));
	if (currentVideoDirectory.exists()) 
	{
		currentVideoDirectory.listDir();
		currentVideoDirectory.sort();
		files = currentVideoDirectory.getFiles();
		
		player0 = newPlayer( index0 );
		player1 = newPlayer( index1 );
	}

	
}

//--------------------------------------------------------------
void multiPlayer::update()
{
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		// check for player0
		if(m.getAddress() == "/player0"){
			// The argument is the clip ID
			index0 = (int)m.getArgAsFloat(0);
			changeVideo(player0, index0);
		}
		if(m.getAddress() == "/player0/playing"){
			float arg = m.getArgAsFloat(0);
			if ( arg < 0.5 ) {
				player0->setPaused(true);
			} else {
				player0->setPaused(false);
			}
		}
		
		// check for player1
		if(m.getAddress() == "/player1"){
			// The argument is the clip ID
			index1 = (int)m.getArgAsFloat(0);
			changeVideo(player1, index1);
		}
		if(m.getAddress() == "/player1/playing"){
			float arg = m.getArgAsFloat(0);
			if ( arg < 0.5 ) {
				player1->setPaused(true);
			} else {
				player1->setPaused(false);
			}
		}
		
		// check for alpha
		if(m.getAddress() == "/alpha"){
			// The argument is the amount
			alpha = m.getArgAsFloat(0)/255.0;
		}
		
		// check for black
		if(m.getAddress() == "/black"){
			// The argument is the amount
			black = m.getArgAsFloat(0)/255.0;
		}
		
		// Exit application
		if(m.getAddress() == "/stop"){
			ofExit();
		}
			
		/*
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		} */

	}
	
}


//--------------------------------------------------------------
void multiPlayer::draw(){
	ofBackground(ofColor::black);
	/*
	alpha = ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);
	black = ofMap(mouseY, 50, ofGetHeight()-50, 0, 1, true);
	*/
	crossFade(alpha);
	fadeToBlack(black);
	
	/*	
	stringstream fpsInfo;
	fpsInfo <<"\n" <<  "APP FPS: "+ ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(fpsInfo.str(), 60, 20, ofColor(ofColor::red, 90), ofColor::yellow);
	*/
}

//--------------------------------------------------------------
void multiPlayer::keyPressed  (int key){
	switch (key) 
	{
		case 'n':
		{
			index0++;
			if ( index0 >= files.size() )
				index0 = 0;
			bool paused = player0->isPaused();
			player0->loadMovie(files[index0].path());
			if ( paused ) 
			{
				// keep the pause state on if it was set
				player0->setPaused(true);
			}
			break;
		}
		
		case 'a':
		{
			player0->setPaused(!player0->isPaused());
			break;
		}
		
		case 'b':
		{
			player1->setPaused(!player1->isPaused());
			break;
		}
		
		
	}
	
}

ofxOMXPlayer* multiPlayer::newPlayer (int index){
	ofxOMXPlayerSettings settings;
	
	settings.videoPath = files[index].path();
	settings.useHDMIForAudio =  false;	//default true
	settings.enableLooping = true;		//default true
	settings.enableAudio = true;		//default true, save resources by disabling
	settings.enableTexture = true;		//default true
	/*
	if (settings.enableTexture) 
	{
		 settings.doFlipTexture = true;  //in texture mode, images appear flipped
	}
	*/
	
	ofxOMXPlayer* player = new ofxOMXPlayer();
	player->setup(settings);
	
	return player;
}

void multiPlayer::crossFade(float amount){
	if ( player0->isPlaying() && player1->isPlaying() )
	{  
		if ( !player0->isPaused() && !player1->isPaused() && amount > 0 && amount < 1 )
		{
			shader.begin();
				shader.setUniformTexture("tex0", player0->getTextureReference(), player0->getTextureID());
				shader.setUniformTexture("tex1", player1->getTextureReference(), player1->getTextureID());
				shader.setUniform1f("alpha", alpha);
				shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
				player0->draw(0, 0, ofGetWidth(), ofGetHeight());
				player1->draw(0, 0, ofGetWidth(), ofGetHeight());
			shader.end();
		} 
		else if ( player1->isPaused() or amount == 0 )
		{	
			player0->draw(0, 0, ofGetWidth(), ofGetHeight());
		} 
		else if ( player0->isPaused() or amount == 1 )
		{
			player1->draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	}
}

/**********************************************************************
 * FADE TO BLACK                                                      *
 **********************************************************************/
 
void multiPlayer::fadeToBlack(float amount)
{
	if (amount > 0)
	{
		ofPushStyle();
		//ofEnableAlphaBlending();
		ofSetColor(0, 0, 0, ofMap(amount, 1, 0, 255, 0, true));
		ofFill();
		ofPushMatrix();
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		ofPopMatrix();
		//ofDisableAlphaBlending(); 
		ofPopStyle();
	}
}

/**********************************************************************
 * CHANGE VIDEO                                                       *
 **********************************************************************/
 
void multiPlayer::changeVideo(ofxOMXPlayer* p, int index)
{
	if ( index >= files.size() ) {
		cout << "Index of video is out of range, so resetting to 0" << endl;
		index = 0;
	}
	// Store if the player was paused
	bool paused = p->isPaused();
	// load the right movie
	p->loadMovie(files[index].path());
	// keep the pause state on if it was set
	if ( paused ) 
	{
		p->setPaused(true);
	}
}
