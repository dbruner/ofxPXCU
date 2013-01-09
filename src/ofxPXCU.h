#include "ofMain.h"
#include "pxcupipeline.h"

class ofxGeoNode
{
public:
	ofPoint imagePos;
	ofPoint worldPos;
	
	ofxGeoNode(PXCGesture::GeoNode pNode);
	~ofxGeoNode();
	void DebugDraw();
	inline int GetOpenness(){return (int)mGeoNode.openness;};
	inline int GetConfidence(){return (int)mGeoNode.confidence};

private:
	PXCGesture::GeoNode mGeoNode;
}

class ofxPXCU
{
public:
	static enum NodeLabel
	{
		HAND_0 = (int)PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY,
		HAND_1 = (int)PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY,
		L_HAND = (int)PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT,
		R_HAND = (int)PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT,
		THUMB_0 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_THUMB),
		THUMB_1 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY|PXCGesture::GeoNode::LABEL_FINGER_THUMB),
		L_THUMB = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT|PXCGesture::GeoNode::LABEL_FINGER_THUMB),
		R_THUMB = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT|PXCGesture::GeoNode::LABEL_FINGER_THUMB),
		INDEX_0 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_INDEX),
		INDEX_1 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY|PXCGesture::GeoNode::LABEL_FINGER_INDEX),
		L_INDEX = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT|PXCGesture::GeoNode::LABEL_FINGER_INDEX),
		R_INDEX = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT|PXCGesture::GeoNode::LABEL_FINGER_INDEX),
		MIDDLE_0 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_MIDDLE),
		MIDDLE_1 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY|PXCGesture::GeoNode::LABEL_FINGER_MIDDLE),
		L_MIDDLE = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT|PXCGesture::GeoNode::LABEL_FINGER_MIDDLE),
		R_MIDDLE = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT|PXCGesture::GeoNode::LABEL_FINGER_MIDDLE),
		RING_0 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_RING),
		RING_1 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY|PXCGesture::GeoNode::LABEL_FINGER_RING),
		L_RING = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT|PXCGesture::GeoNode::LABEL_FINGER_RING),
		R_RING = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT|PXCGesture::GeoNode::LABEL_FINGER_RING),
		PINKY_0 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_PINKY),
		PINKY_1 = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY|PXCGesture::GeoNode::LABEL_FINGER_PINKY),
		L_PINKY = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT|PXCGesture::GeoNode::LABEL_FINGER_PINKY),
		R_PINKY = (int)(PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT|PXCGesture::GeoNode::LABEL_FINGER_PINKY)
	};

	ofxPXCU();
	~ofxPXCU();

	ofTexture RGBTex
	ofTexture DepthTex;
	ofTexture LabelTex;
	ofTexture IRTex;
	ofPoint[] DepthPoints();

	void EnableRGB(bool pIsWXGA);
	void EnableDepth(bool pDrawDepth);
	void EnableGesture(bool pDrawLabel);
	void EnableIR(bool pDrawIR);
	
	bool Start();
	bool Update();
	
	void LoadDepthPoints();

	inline short* GetDepthRef(){return mDepth;};

private:
	PXCUPipeline_Instance mSession;
	PXCPUPipeline mMode;

	bool mReleased, mHasRGB, mHasDepth, mDrawDepth, mDrawLabel, mHasGesture;
	
	int mDepthWidth, mDepthHeight;
	short* mDepth;
	unsigned char* mDepthMap;
	unsigned char* mRGBMap;
	unsigned char* mLabelMap;

	void toTexture(unsigned short* src, unsigned char* dstB, ofTexture& dst);
};
