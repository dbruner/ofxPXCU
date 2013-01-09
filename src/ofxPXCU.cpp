#include "ofxPXCU.h"

ofxPXCU::ofxPXCU()
{
	mMode = -1;
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

bool ofxPXCU::Start()
{
	int w,h;
	if(mMode==-1)
	{
		mMode = (PXCUPipeline)PXCU_PIPELINE_COLOR_VGA;
		mHasRGB = true;
		mHasDepth = false;
	}
	mSession = PXCUPipeline_Init(mMode);
	if(!mSession)
		return false;

	if(mHasRGB)
	{
		PXCUPipeline_QueryRGBSize(mSession, &w, &h);
		mRGB = new unsigned char[w*h*4];
		RGBTex = ofTexture();
		RGBTex.allocate(w,h,GL_RGBA);
	}
	if(mHasDepth)
	{
		PXCUPipeline_QueryDepthMapSize(mSession, &w, &h);
		mDepth = new short[w*h];
		if(mDrawDepth)
		{
			mDepthMap = new unsigned char[w*h];
			DepthTex.allocate(w,h,GL_LUMINANCE);
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

// call with pVGARes=0 for VGA, pVGARes=1 for WXGA
void ofxPXCU::EnableRGB(bool pIsWXGA)
{
	PXCUPipeline rgbMode = pIsWXGA?(PXCUPipeline)PXCU_PIPELINE_COLOR_WXGA:(PXCUPipeline)PXCU_PIPELINE_COLOR_VGA;
	if(!mHasRgb)
	{
    	if(mMode<0)
    	{
    		mMode = rgbMode;
    	}
    	else
    	{
    		mMode = (PXCUPipeline)(mMode|rgbMode);
    	}
    	mHasRgb = true;
		RGBTex = ofTexture();
	}
}

void ofxPXCU::EnableDepth(bool pDrawDepth)
{
    if(!mHasDepth)
    {
    	if(mMode<0)
    		mMode = (PXCUPipeline)PXCU_PIPELINE_DEPTH_QVGA;
    	else
    		mMode = (PXCUPipeline)(mMode|PXCU_PIPELINE_DEPTH_QVGA);
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

bool ofxPXCU::Update()
{
	if(!PXCUPipeline_AcquireFrame(mSession, true))
		return false;
	mReleased = false;
	if(mHasRGB)
	{
		PXCUPipeline_QueryRGB(mSession, mRGB);
		if(mRGB)
			mRGBTex.loadData(mRGB, mRGBTex.getWidth(), mRGBTex.getHeight(), GL_RGBA);
	}
	if(mHasDepth)
	{
		PXCUPipeline_QueryDepthMap(mSession, mDepth);
		if(mDepth)
			toTexture((unsigned short*)mDepth, mDepthMap, mDepthTex);	
	}
	PXCUPipeline_ReleaseFrame(mSession);
	mReleased = true;
}

void ofxPXCU::toTexture(unsigned short *src, unsigned char *dstB, ofTexture &dst)
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
