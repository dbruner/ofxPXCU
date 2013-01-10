#include "ofxPXCU.h"

ofxPXCU::ofxPXCU()
{
	mMode = -1;
	mReleased = false;
	mHasRGB = false;
	mHasDepth = false;
	mDrawDepth = false;
	mDrawLabel = false;
	mHasGesture = false;
}

ofxPXCU::~ofxPXCU()
{
	if(mSession)
	{
		if(!mReleased)
			PXCUPipeline_ReleaseFrame(mSession);
		PXCUPipeline_Close(mSession);
	}
}

void ofxPXCU::EnableRGB(bool pIsWXGA)
{
	int rgbMode = pIsWXGA?(int)PXCU_PIPELINE_COLOR_WXGA:(int)PXCU_PIPELINE_COLOR_VGA;
	if(!mHasRGB)
	{
    	if(mMode<0)
    	{
    		mMode = rgbMode;
    	}
    	else
    	{
    		mMode = (int)(mMode|rgbMode);
    	}
    	mHasRGB = true;
		RGBTex = ofTexture();
	}
}

void ofxPXCU::EnableDepth(bool pDrawDepth)
{
    if(!mHasDepth)
    {
    	if(mMode<0)
    		mMode = PXCU_PIPELINE_DEPTH_QVGA;
    	else
    		mMode = (int)(mMode|PXCU_PIPELINE_DEPTH_QVGA);
    	mHasDepth = true;
		mDrawDepth = pDrawDepth;
		if(mDrawDepth)
			DepthTex = ofTexture();
    }
}

void ofxPXCU::EnableGesture(bool pDrawLabel)
{
    if(!mHasGesture)
    {
        if(mMode<0)
            mMode = (PXCUPipeline)PXCU_PIPELINE_GESTURE;
        else
            mMode = (PXCUPipeline)(mMode|PXCU_PIPELINE_GESTURE);
		mHasGesture = true;
		mDrawLabel = pDrawLabel;
		if(mDrawLabel)
			LabelTex = ofTexture();
    }
}

void ofxPXCU::EnableIR(bool pDrawIR)
{
}

bool ofxPXCU::Start()
{
	int w,h;
	
	if(mMode==-1)
	{
		mMode = (PXCUPipeline)PXCU_PIPELINE_COLOR_VGA;
		mHasRGB = true;
		mHasDepth = false;
		mHasGesture = false;
	}
	mSession = PXCUPipeline_Init((PXCUPipeline)mMode);
	if(!mSession)
		return false;

	if(mHasRGB)
	{
		PXCUPipeline_QueryRGBSize(mSession, &w, &h);
		mRGBMap = new unsigned char[w*h*4];
		RGBTex = ofTexture();
		RGBTex.allocate(w,h,GL_RGBA);
	}
	if(mHasDepth)
	{
		PXCUPipeline_QueryDepthMapSize(mSession, &mDepthWidth, &mDepthHeight);
		mDepth = new short[mDepthWidth*mDepthHeight];
		if(mDrawDepth)
		{
			mDepthMap = new unsigned char[mDepthWidth*mDepthHeight];
			DepthTex.allocate(mDepthWidth,mDepthHeight,GL_LUMINANCE);
		}
	}
    if(mHasGesture)
	{
		PXCUPipeline_QueryLabelMapSize(mSession, &w, &h);
		if(mDrawLabel)
		{
			mLabelMap = new unsigned char[w*h];
			LabelTex = ofTexture();
			LabelTex.allocate(w,h,GL_LUMINANCE);
		}
	}    
	return true;
}

bool ofxPXCU::Update()
{
	if(!PXCUPipeline_AcquireFrame(mSession, true))
		return false;
	mReleased = false;
	if(mHasRGB)
	{
		PXCUPipeline_QueryRGB(mSession, mRGBMap);
		if(mRGBMap)
			RGBTex.loadData(mRGBMap, RGBTex.getWidth(), RGBTex.getHeight(), GL_RGBA);
	}
	if(mHasDepth)
	{
		PXCUPipeline_QueryDepthMap(mSession, mDepth);
		if(mDepth&&mDrawDepth)
			toTexture((unsigned short*)mDepth, mDepthMap, &DepthTex);
	}
	if(mHasGesture)
	{
		PXCUPipeline_QueryLabelMap(mSession, mLabelMap, 0);
		if(mLabelMap&&mDrawLabel)
			LabelTex.loadData(mLabelMap, LabelTex.getWidth(), LabelTex.getHeight(), GL_LUMINANCE);

	}
	PXCUPipeline_ReleaseFrame(mSession);
	mReleased = true;
}

void ofxPXCU::LoadDepthPoints()
{
}

void ofxPXCU::toTexture(unsigned short* src, unsigned char* dstB, ofTexture* dst)
{
    float minC = 0xffff;
    float maxC = -0xffff;
	int tx = dst->getWidth();
	int ty = dst->getHeight();

	for(int k=0;k<tx*ty;k++)
	{
        float vC = (float)src[k]/0xffff;
        if (minC>vC) minC = vC;
        if (maxC<vC) maxC = vC;
    }
    for(int i=0;i<tx*ty;i++)
	{
        float val = (float)src[i]/0xffff;
        val = 255.f*sqrt((val-minC)/(maxC-minC));
        dstB[i]=255-(unsigned char)val;
    }
	dst->loadData(dstB,tx,ty,GL_LUMINANCE);
}
