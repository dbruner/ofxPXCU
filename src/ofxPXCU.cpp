#include "ofxPXCU.h"

ofxPXCU::ofxPXCU()
{
	mMode = -1;
}

ofxPXCU::~ofxPXCU()
{
	if(mSession)
		PXCUPipeline_Close(mSession);
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
		mRGBTex = ofTexture();
		PXCUPipeline_QueryRGBSize(mSession, &w, &h);
		mRGB = new unsigned char[w*h*4];
		mRGBTex.allocate(w,h,GL_RGBA);
	}
	if(mHasDepth)
	{
		mDepthTex = ofTexture();
		PXCUPipeline_QueryDepthMapSize(mSession, &w, &h);
		mDepth = new short[w*h];
		mDepthMap = new unsigned char[w*h];
		mDepthTex.allocate(w,h,GL_LUMINANCE);
	}
	return true;
}

void ofxPXCU::EnableRGB(std::string pMode)
{
	PXCUPipeline rgbMode;
	switch(pMode)
	{
	case "VGA":
		{
			rgbMode = (PXCUPipeline)PXCU_PIPELINE_COLOR_VGA;
			break;
		}
	case "WXGA":
		{
			rgbMode = (PXCUPipeline)PXCU_PIPELINE_COLOR_WXGA;
			break;
		}
	}

	if(mMode<0)
	{
		mMode = rgbMode;
	}
	else
	{
		mMode = (PXCUPipeline)(mMode|rgbMode);
	}
	mHasRgb = true;
}

void ofxPXCU::EnableDepth()
{
	if(mMode<0)
	{
		mMode = (PXCUPipeline)PXCU_PIPELINE_DEPTH_QVGA;
	}
	else
	{
		mMode = (PXCUPipeline)(mMode|PXCU_PIPELINE_DEPTH_QVGA);
	}
	mHasDepth = true;
}

//void ofxPXCU::EnableGesture();

bool ofxPXCU::Update()
{
	if(!PXCUPipeline_AcquireFrame(mSession, true))
		return false;
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
		{
			toTexture((unsigned short*)mDepth, mDepthMap, mDepthTex);	
		}
	}
	PXCUPipeline_ReleaseFrame(mSession);
}

ofTexture& ofxPXCU::GetRGB()
{
	return mRGBTex;
}

ofTexture& ofxPXCU::GetDepth()
{
	return mDepthTex;
}
	
void ofxPXCU::toTexture(unsigned short *src, unsigned char *dstB, ofTexture &dst)
{
    float minC = 0xffff;
    float maxC = -0xffff;
	int txsize = dst->getWidth()*dst->getHeight();
    for(int k=0;k<txsize;k++)
	{
        float vC = (float)src[k]/0xffff;
        if (minC>vC) minC = vC;
        if (maxC<vC) maxC = vC;
    }
    for(int i=0;i<txsize;i++) {
        float val = (float)src[i]/0xffff;
        val = 255.f*sqrt((val-minC)/(maxC-minC));
        dstB[i]=255-(unsigned char)val;
    }
	dst->loadData(dstB,dst->getWidth(),dst->getHeight(),GL_LUMINANCE);
}
