#include "ofMain.h"
#include "multiPlayer.h"
//#include "ofGLProgrammableRenderer.h"

int main()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(768, 576, OF_FULLSCREEN);
	ofRunApp( new multiPlayer());
}
