#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	_pxcu.EnableRGB(false);
	_pxcu.EnableDepth(true);
	_pxcu.EnableGesture(true);
	_pxcu.EnableIR(true);
	_pxcu.Start();
	ofSetWindowShape(1280,960);
}

//--------------------------------------------------------------
void testApp::update()
{
	_pxcu.Update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	_pxcu.RGBTex.draw(0,0,640,480);
	_pxcu.LabelTex.draw(640,0,640,480);
	_pxcu.DepthTex.draw(0,480,640,480);
	_pxcu.IRTex.draw(640,480,640,480);
}