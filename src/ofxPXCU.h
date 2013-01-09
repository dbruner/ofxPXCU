#include "ofMain.h"
#include "pxcupipeline.h"

class ofxPXCU
{
public:
	ofxPXCU();
	~ofxPXCU();

	bool Start();
	void EnableRGB(std::string pMode);
	void EnableDepth();
	void EnableGesture();

	bool Update();
	ofTexture& GetRGBMap();
	ofTexture& GetDepthMap();
	ofTexture& GetLabelMap();
	
private:
	PXCUPipeline_Session mSession;
	PXCPUPipeline mMode;

	bool mHasRGB;
	bool mHasDepth;
	bool mHasLabels;

	short* mDepth;
	unsigned char* mDepthMap;
	unsigned char* mRGBMap;
	unsigned char* mLabelMap;

	ofTexture mRGBTex;
	ofTexture mDepthTex;
	ofTexture mLabelTex;
	
	void toTexture(unsigned short* src, unsigned char* dstB, ofTexture& dst);
};
