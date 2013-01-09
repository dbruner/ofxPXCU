#include "ofMain.h"
#include "pxcupipeline.h"

class ofxPXCU
{
public:
	ofxPXCU();
	~ofxPXCU();
	bool Init();

	void EnableRGB();
	void EnableDepth();
	//void EnableGesture();

	bool Update(bool pWait);
	ofTexture& GetRGB();
	ofTexture& GetDepth();
	
private:
	PXCUPipeline_Session mSession;
	PXCPUPipeline mMode;

	bool mHasRGB;
	bool mHasDepth;

	unsigned char* mRGB;
	unsigned char* mDepth;

	ofTexture mRGBTex;
	ofTexture mDepthTex;
};

/*
class ofxGeoNode
{
public:
	ofxGeoNode();
	ofxGeoNode(PXCGesture::GeoNode pNode);

	
};*/