#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "ofxOsc.h"

#define PORT 40000

class multiPlayer : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		
		// OMX Players
		ofxOMXPlayer* newPlayer(int index);
		
		ofxOMXPlayer* player0;
		ofxOMXPlayer* player1;
		
		int index0;
		int index1;
		
		vector<ofFile> files;

		ofShader shader;

		float alpha; // crossfade (0...1)
		void crossFade(float amount);
		
		float black; // fade to black (0...1)
		void fadeToBlack(float amount);
		
		void changeVideo(ofxOMXPlayer* p, int index);
	
		// OSC Stuff
		ofxOscReceiver receiver;
		
};

