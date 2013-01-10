#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	//call EnableRGB with false to set VGA mode, true to set WXGA mode
	_pxcu.EnableRGB(false);

	//call EnableDepth, EnableGesture, and EnableIR with false to capture raw data,
	//call with true to capture raw data and load the data into ofTextures
	_pxcu.EnableDepth(true);
	_pxcu.EnableGesture(true);
	_pxcu.EnableIR(true);

	//Start the device and sdk session
	_pxcu.Start();
	ofSetWindowShape(1280,960);
}

//--------------------------------------------------------------
void testApp::update()
{
    //Update the device's data buffers and textures
	_pxcu.Update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    //If texture capture was specified, each map is available as an ofTexture
	_pxcu.RGBTex.draw(0,0,640,480);
	_pxcu.LabelTex.draw(640,0,640,480);
	_pxcu.DepthTex.draw(0,480,640,480);
	_pxcu.IRTex.draw(640,480,640,480);
}