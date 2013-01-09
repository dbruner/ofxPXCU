#include "ofMain.h"
#include "pxcupipeline.h"

class ofxPXCU
{
public:
	ofxPXCU();
	~ofxPXCU();

	bool Init();
	void EnableRGB(std::string pMode);
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


	short *mDepth;
	unsigned char *mDepthMap;
	unsigned char *mRGB;
	

	ofTexture mRGBTex;
	ofTexture mDepthTex;

	void toTexture(unsigned short* src, unsigned char* dstB, ofTexture& dst);
};

/*
class ofxGeoNode
{
public:
	ofxGeoNode();
	ofxGeoNode(PXCGesture::GeoNode pNode);

	
};*/
