/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#include <vector>
#include "pxcupipeline.h"
#include "util_pipeline.h"
#include "ippi.h"
#include "ippcore.h"

struct _PXCUPipeline_Instance {
	UtilPipeline	pipeline;
};

PXCUPipeline_Instance __stdcall PXCUPipeline_Init(PXCUPipeline mode) {
	PXCUPipeline_Instance instance=new _PXCUPipeline_Instance;
	if (!instance) return NULL;

    ippInit();
	if (mode&PXCU_PIPELINE_COLOR_VGA)     instance->pipeline.EnableImage(PXCImage::COLOR_FORMAT_RGB32,640,480);
	if (mode&PXCU_PIPELINE_COLOR_WXGA)    instance->pipeline.EnableImage(PXCImage::COLOR_FORMAT_RGB24,1280,720);
	if (mode&PXCU_PIPELINE_DEPTH_QVGA)    instance->pipeline.EnableImage(PXCImage::COLOR_FORMAT_DEPTH,320,240);
	if (mode&PXCU_PIPELINE_GESTURE)       instance->pipeline.EnableGesture();
	if (mode&PXCU_PIPELINE_FACE_LOCATION) instance->pipeline.EnableFaceLocation();
	if (mode&PXCU_PIPELINE_FACE_LANDMARK) instance->pipeline.EnableFaceLandmark();
	if (instance->pipeline.Init()) return instance;
	instance->pipeline.Close();
	delete instance;
	return NULL;
}

bool __stdcall PXCUPipeline_AcquireFrame(PXCUPipeline_Instance instance, bool wait) {
	return instance->pipeline.AcquireFrame(wait);
}

void __stdcall PXCUPipeline_ReleaseFrame(PXCUPipeline_Instance instance) {
	instance->pipeline.ReleaseFrame();
}

bool __stdcall PXCUPipeline_IsDisconnected(PXCUPipeline_Instance instance) {
	return instance->pipeline.IsDisconnected();
}

bool __stdcall PXCUPipeline_QueryRGB(PXCUPipeline_Instance instance, unsigned char *data) {
	if (instance->pipeline.IsDisconnected()) return false;
    PXCImage *image=instance->pipeline.QueryImage(PXCImage::IMAGE_TYPE_COLOR);
    if (!image) return false;

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data2;
    pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,PXCImage::COLOR_FORMAT_RGB32,&data2);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    IppiSize roi={ info.width, info.height };
    int dstOrder[3]={2,1,0};
    ippiSwapChannels_8u_AC4R(data2.planes[0],data2.pitches[0],data,data2.pitches[0],roi,dstOrder);
    image->ReleaseAccess(&data2);
    return true;
}

bool __stdcall PXCUPipeline_QueryRGBSize(PXCUPipeline_Instance instance, int *width, int *height) {
	return instance->pipeline.QueryImageSize(PXCImage::IMAGE_TYPE_COLOR,*(pxcU32*)width,*(pxcU32*)height);
}

bool __stdcall PXCUPipeline_QueryDepthMapSize(PXCUPipeline_Instance instance, int *width, int *height) {
    return instance->pipeline.QueryImageSize(PXCImage::IMAGE_TYPE_DEPTH,*(pxcU32*)width,*(pxcU32*)height);
}

bool __stdcall PXCUPipeline_QueryIRMapSize(PXCUPipeline_Instance instance, int *width, int *height) {
    return instance->pipeline.QueryImageSize(PXCImage::IMAGE_TYPE_DEPTH,*(pxcU32*)width,*(pxcU32*)height);
}

bool __stdcall PXCUPipeline_QueryUVMapSize(PXCUPipeline_Instance instance, int *width, int *height) {
	return instance->pipeline.QueryImageSize(PXCImage::IMAGE_TYPE_DEPTH,*(pxcU32*)width,*(pxcU32*)height);
}

bool __stdcall PXCUPipeline_QueryLabelMapSize(PXCUPipeline_Instance instance, int *width, int *height) {
    return instance->pipeline.QueryImageSize(PXCImage::IMAGE_TYPE_DEPTH,*(pxcU32*)width,*(pxcU32*)height);
}

bool __stdcall PXCUPipeline_QueryDepthMap(PXCUPipeline_Instance instance, short *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCImage *image=instance->pipeline.QueryImage(PXCImage::IMAGE_TYPE_DEPTH);
    if (!image) return false;

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data2;
    pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,&data2);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    if (data2.planes[0]) {
        IppiSize roi={ info.width/2, info.height };
        ippiCopy_32f_C1R((Ipp32f*)data2.planes[0],data2.pitches[0],(Ipp32f*)data,data2.pitches[0],roi);
        image->ReleaseAccess(&data2);
        return true;
    } else {
        image->ReleaseAccess(&data2);
        return false;
    }
}

bool __stdcall PXCUPipeline_QueryIRMap(PXCUPipeline_Instance instance, short *data) {
	if (instance->pipeline.IsDisconnected()) return false;
    PXCImage *image=instance->pipeline.QueryImage(PXCImage::IMAGE_TYPE_DEPTH);
    if (!image) return false;

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data2;
    pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,&data2);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    if (data2.planes[1]) {
        IppiSize roi={ info.width/2, info.height };
        ippiCopy_32f_C1R((Ipp32f*)data2.planes[1],data2.pitches[1],(Ipp32f*)data,data2.pitches[1],roi);
        image->ReleaseAccess(&data2);
        return true;
    } else {
        image->ReleaseAccess(&data2);
        return false;
    }
}

bool __stdcall PXCUPipeline_QueryUVMap(PXCUPipeline_Instance instance, float *data) {
	if (instance->pipeline.IsDisconnected()) return false;
    PXCImage *image=instance->pipeline.QueryImage(PXCImage::IMAGE_TYPE_DEPTH);
    if (!image) return false;

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data2;
    pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,&data2);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    if (data2.planes[2]) {
        IppiSize roi={ info.width*2, info.height };
        ippiCopy_32f_C1R((Ipp32f*)data2.planes[2],data2.pitches[2],data,data2.pitches[2],roi);
        image->ReleaseAccess(&data2);
        return true;
    } else {
        image->ReleaseAccess(&data2);
        return false;
    }
}

bool __stdcall PXCUPipeline_QueryLabelMap(PXCUPipeline_Instance instance, unsigned char *data, int *labels) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCGesture *gesture=instance->pipeline.QueryGesture();
	if (!gesture) return false;

    PXCSmartPtr<PXCImage> image;
    pxcStatus sts=gesture->QueryBlobImage(PXCGesture::Blob::LABEL_SCENE,0,&image);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data2;
    sts=image->AcquireAccess(PXCImage::ACCESS_READ,&data2);
    if (sts<PXC_STATUS_NO_ERROR) return false;

    IppiSize roi={ info.width/4, info.height };
    ippiCopy_32f_C1R((Ipp32f*)data2.planes[0],data2.pitches[0],(Ipp32f*)data,data2.pitches[0],roi);
    image->ReleaseAccess(&data2);

	if (labels) {
		PXCGesture::Blob bdata;
		sts=gesture->QueryBlobData(PXCGesture::Blob::LABEL_SCENE,0,&bdata);
		if (sts<PXC_STATUS_NO_ERROR) return false;
		labels[0]=bdata.labelBackground;
		labels[1]=bdata.labelLeftHand;
		labels[2]=bdata.labelRightHand;
	}
    return true;
}

bool __stdcall PXCUPipeline_QueryDeviceProperty(PXCUPipeline_Instance instance, PXCCapture::Device::Property pty, int npty, float *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	UtilCapture *capture=instance->pipeline.QueryCapture();
	if (!capture) return false;
	PXCCapture::Device *device=capture->QueryDevice();
	if (!device) return false;
	for (int i=0;i<npty;i++,pty=(PXCCapture::Device::Property)(pty+1),data++)
		if (device->QueryProperty(pty,data)<PXC_STATUS_NO_ERROR) return false;
	return true;
}

bool __stdcall PXCUPipeline_SetDeviceProperty(PXCUPipeline_Instance instance, PXCCapture::Device::Property pty, int npty, float *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	UtilCapture *capture=instance->pipeline.QueryCapture();
	if (!capture) return false;
	PXCCapture::Device *device=capture->QueryDevice();
	if (!device) return false;
	for (int i=0;i<npty;i++,pty=(PXCCapture::Device::Property)(pty+1),data++)
		if (device->SetProperty(pty,*data)<PXC_STATUS_NO_ERROR) return false;
	return true;
}

bool __stdcall PXCUPipeline_QueryGeoNode(PXCUPipeline_Instance instance, PXCGesture::GeoNode::Label body, PXCGesture::GeoNode *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCGesture *gesture=instance->pipeline.QueryGesture();
	if (!gesture) return false;
    return gesture->QueryNodeData(0,body,data)>=PXC_STATUS_NO_ERROR;
}

bool __stdcall PXCUPipeline_QueryGesture(PXCUPipeline_Instance instance, PXCGesture::GeoNode::Label body, PXCGesture::Gesture *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCGesture *gesture=instance->pipeline.QueryGesture();
	if (!gesture) return false;
	return gesture->QueryGestureData(0,body,0,data)>=PXC_STATUS_NO_ERROR;
}

bool __stdcall PXCUPipeline_QueryFaceID(PXCUPipeline_Instance instance, int fidx, pxcUID *face, pxcU64 *timeStamp) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCFaceAnalysis *analyzer=instance->pipeline.QueryFace();
	if (!analyzer) return false;
	return analyzer->QueryFace(fidx,face,timeStamp)>=PXC_STATUS_NO_ERROR;
}

bool __stdcall PXCUPipeline_QueryFaceLocationData(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Detection::Data *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCFaceAnalysis *analyzer=instance->pipeline.QueryFace();
	if (!analyzer) return false;
	PXCFaceAnalysis::Detection *detection=analyzer->DynamicCast<PXCFaceAnalysis::Detection>();
	if (!detection) return false;
    return detection->QueryData(face,data)>=PXC_STATUS_NO_ERROR;
}

bool __stdcall PXCUPipeline_QueryFaceLandmarkPose(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Landmark::PoseData *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCFaceAnalysis *analyzer=instance->pipeline.QueryFace();
	if (!analyzer) return false;
    PXCFaceAnalysis::Landmark *landmark=analyzer->DynamicCast<PXCFaceAnalysis::Landmark>();
    if (!landmark) return false;
    return landmark->QueryPoseData(face,data)>=PXC_STATUS_NO_ERROR;
}

bool __stdcall PXCUPipeline_QueryFaceLandmarkData(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Landmark::Label label, int idx, PXCFaceAnalysis::Landmark::LandmarkData *data) {
	if (instance->pipeline.IsDisconnected()) return false;
	PXCFaceAnalysis *analyzer=instance->pipeline.QueryFace();
	if (!analyzer) return false;
    PXCFaceAnalysis::Landmark *landmark=analyzer->DynamicCast<PXCFaceAnalysis::Landmark>();
    if (!landmark) return false;
    return landmark->QueryLandmarkData(face,label,idx,data)>=PXC_STATUS_NO_ERROR;
}

void __stdcall PXCUPipeline_Close(PXCUPipeline_Instance instance) {
	instance->pipeline.Close();
    delete instance;
}
