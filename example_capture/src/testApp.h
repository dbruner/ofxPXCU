#pragma once

#include "ofMain.h"
#include "ofxPXCU.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofxPXCU _pxcu;
};
