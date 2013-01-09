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
	static enum Labels
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

	bool Start();
	void EnableRGB(int pVGARes);
	void EnableDepth();
	void EnableGesture(bool pGetLabelMap);

	bool Update();

	void DrawRGBMap(ofPoint pLeftTop);
	void DrawRGBMap(float pLeft, float pTop);
	void DrawRGBMap(ofPoint pLeftTop, ofPoint pWidthHeight);
	void DrawRGBMap(float pLeft, float pTop, float pWidth, float pHeight);

	void DrawDepthMap(ofPoint pLeftTop);
	void DrawDepthMap(float pLeft, float pTop);
	void DrawDepthMap(ofPoint pLeftTop, ofPoint pWidthHeight);
	void DrawDepthMap(float pLeft, float pTop, float pWidth, float pHeight);
	
	void DrawLabelMap(ofPoint pLeftTop);
	void DrawLabelMap(float pLeft, float pTop);
	void DrawLabelMap(ofPoint pLeftTop, ofPoint pWidthHeight);
	void DrawLabelMap(float pLeft, float pTop, float pWidth, float pHeight);
	
	void DrawIRMap(ofPoint pLeftTop);
	void DrawIRMap(float pLeft, float pTop);
	void DrawIRMap(ofPoint pLeftTop, ofPoint pWidthHeight);
	void DrawIRMap(float pLeft, float pTop, float pWidth, float pHeight);

	short GetDepthValue(ofPoint pCoords);
	short GetDepthValue(int px, int py);
	inline short* GetDepthValues(){return mDepth;};

	inline ofTexture& GetRGBMap(){return mRGBTex;};
	inline ofTexture& GetDepthMap(){return mDepthTex;};
	inline ofTexture& GetLabelMap(){return mLabelTex;};
	inline ofTexture& GetIRMap(){return mIRTex;};

private:
	PXCUPipeline_Instance mSession;
	PXCPUPipeline mMode;

	bool mReleased;
	bool mHasRGB;
	bool mHasDepth;
	bool mHasLabel;
	bool mHasGesture;

	short* mDepth;
	unsigned char* mDepthMap;
	unsigned char* mRGBMap;
	unsigned char* mLabelMap;

	ofTexture mRGBTex;
	ofTexture mDepthTex;
	ofTexture mLabelTex;
	ofTexture mIRTex;
	
	void toTexture(unsigned short* src, unsigned char* dstB, ofTexture& dst);
};
