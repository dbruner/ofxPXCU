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
	mHasIR = false;
	mDrawIR = false;
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
		mHasRGB = true;
    	if(mMode<0)
    	{
    		mMode = rgbMode;
    	}
    	else
    	{
    		mMode = (int)(mMode|rgbMode);
    	}
		RGBTex = ofTexture();
	}
}

void ofxPXCU::EnableDepth(bool pDrawDepth)
{
    if(!mHasDepth)
    {
    	mHasDepth = true;
		mDrawDepth = pDrawDepth;
    	if(mMode<0)
    		mMode = PXCU_PIPELINE_DEPTH_QVGA;
    	else
    		mMode = (int)(mMode|PXCU_PIPELINE_DEPTH_QVGA);
		if(mDrawDepth)
			DepthTex = ofTexture();
    }
}

void ofxPXCU::EnableGesture(bool pDrawLabel)
{
    if(!mHasGesture)
    {
		mHasGesture = true;
		mDrawLabel = pDrawLabel;
        if(mMode<0)
            mMode = (PXCUPipeline)PXCU_PIPELINE_GESTURE;
        else
            mMode = (PXCUPipeline)(mMode|PXCU_PIPELINE_GESTURE);
		if(mDrawLabel)
			LabelTex = ofTexture();
    }
}

void ofxPXCU::EnableIR(bool pDrawIR)
{
	if(!mHasIR)
	{
		mHasIR = true;
		mDrawIR = pDrawIR;
		if(mDrawIR)
			IRTex = ofTexture();
	}
}

bool ofxPXCU::Start()
{
	int w,h;
	
	if(mMode==-1)
	{
		mMode = (PXCUPipeline)PXCU_PIPELINE_COLOR_VGA;
		mReleased = false;
		mHasRGB = false;
		mHasDepth = false;
		mDrawDepth = false;
		mDrawLabel = false;
		mHasGesture = false;
		mHasIR = false;
		mDrawIR = false;
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
			LabelTex.allocate(w,h,GL_LUMINANCE);
		}
	}    
	if(mHasIR)
	{
		PXCUPipeline_QueryIRMapSize(mSession, &w, &h);
		mIR = new short[w*h];
		if(mDrawIR)
		{
			mIRMap = new unsigned char[w*h];
			IRTex.allocate(w,h,GL_LUMINANCE);
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
			toTexture((unsigned short*)mDepth, mDepthMap, &DepthTex, true);
	}
	if(mHasGesture)
	{
		PXCUPipeline_QueryLabelMap(mSession, mLabelMap, 0);
		if(mLabelMap&&mDrawLabel)
			LabelTex.loadData(mLabelMap, LabelTex.getWidth(), LabelTex.getHeight(), GL_LUMINANCE);

	}
	if(mHasIR)
	{
		PXCUPipeline_QueryIRMap(mSession, mIR);
		if(mIR&&mDrawIR)
			toTexture((unsigned short*)mIR, mIRMap, &IRTex, false);

	}
	PXCUPipeline_ReleaseFrame(mSession);
	mReleased = true;
	return true;
}

void ofxPXCU::LoadDepthPoints()
{
}

void ofxPXCU::toTexture(unsigned short* pSrc, unsigned char* pDstB, ofTexture* pDst, bool pInvert)
{
    float minC = 0xffff;
    float maxC = -0xffff;
	int tx = pDst->getWidth();
	int ty = pDst->getHeight();

	for(int k=0;k<tx*ty;k++)
	{
        float vC = (float)pSrc[k]/0xffff;
        if (minC>vC) minC = vC;
        if (maxC<vC) maxC = vC;
    }
    for(int i=0;i<tx*ty;i++)
	{
        float val = (float)pSrc[i]/0xffff;
        val = 255.f*sqrt((val-minC)/(maxC-minC));
        pDstB[i]=pInvert?255-(unsigned char)val:(unsigned char)val;
    }
	pDst->loadData(pDstB,tx,ty,GL_LUMINANCE);
}
