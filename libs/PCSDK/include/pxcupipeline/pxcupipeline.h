/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#pragma once
#include "pxcgesture.h"
#include "pxcface.h"

extern "C" {

enum PXCUPipeline {
	PXCU_PIPELINE_GESTURE       =0x00000002,
	PXCU_PIPELINE_FACE_LOCATION =0x00000004,
	PXCU_PIPELINE_FACE_LANDMARK =0x00000008,
	PXCU_PIPELINE_COLOR_VGA		=0x00000001,
	PXCU_PIPELINE_COLOR_WXGA	=0x00000010,
	PXCU_PIPELINE_DEPTH_QVGA	=0x00000020,
};

typedef struct _PXCUPipeline_Instance *PXCUPipeline_Instance;
PXCUPipeline_Instance __stdcall PXCUPipeline_Init(PXCUPipeline mode);
bool __stdcall PXCUPipeline_AcquireFrame(PXCUPipeline_Instance instance, bool wait);
bool __stdcall PXCUPipeline_IsDisconnected(PXCUPipeline_Instance instance);
void __stdcall PXCUPipeline_ReleaseFrame(PXCUPipeline_Instance instance);
void __stdcall PXCUPipeline_Close(PXCUPipeline_Instance instance);

/* raw data */
bool __stdcall PXCUPipeline_QueryRGBSize(PXCUPipeline_Instance instance, int *width, int *height);
bool __stdcall PXCUPipeline_QueryRGB(PXCUPipeline_Instance instance, unsigned char *data);
bool __stdcall PXCUPipeline_QueryDepthMapSize(PXCUPipeline_Instance instance, int *width, int *height);
bool __stdcall PXCUPipeline_QueryDepthMap(PXCUPipeline_Instance instance, short *data);
bool __stdcall PXCUPipeline_QueryIRMapSize(PXCUPipeline_Instance instance, int *width, int *height);
bool __stdcall PXCUPipeline_QueryIRMap(PXCUPipeline_Instance instance, short *data);
bool __stdcall PXCUPipeline_QueryUVMapSize(PXCUPipeline_Instance instance, int *width, int *height);
bool __stdcall PXCUPipeline_QueryUVMap(PXCUPipeline_Instance instance, float *data);           
bool __stdcall PXCUPipeline_QueryLabelMapSize(PXCUPipeline_Instance instance, int *width, int *height);
bool __stdcall PXCUPipeline_QueryLabelMap(PXCUPipeline_Instance instance, unsigned char *data, int *labels);
bool __stdcall PXCUPipeline_QueryDeviceProperty(PXCUPipeline_Instance instance, PXCCapture::Device::Property pty, int npty, float *data);
bool __stdcall PXCUPipeline_SetDeviceProperty(PXCUPipeline_Instance instance, PXCCapture::Device::Property pty, int npty, float *data);

/* gesture functions */
bool __stdcall PXCUPipeline_QueryGeoNode(PXCUPipeline_Instance instance, PXCGesture::GeoNode::Label body, PXCGesture::GeoNode *data);
bool __stdcall PXCUPipeline_QueryGesture(PXCUPipeline_Instance instance, PXCGesture::GeoNode::Label body, PXCGesture::Gesture *data);

/* face analysis functions */
bool __stdcall PXCUPipeline_QueryFaceID(PXCUPipeline_Instance instance, int fidx, pxcUID *face, pxcU64 *timeStamp);
bool __stdcall PXCUPipeline_QueryFaceLocationData(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Detection::Data *data);
bool __stdcall PXCUPipeline_QueryFaceLandmarkPose(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Landmark::PoseData *data);
bool __stdcall PXCUPipeline_QueryFaceLandmarkData(PXCUPipeline_Instance instance, pxcUID face, PXCFaceAnalysis::Landmark::Label label, int lidx, PXCFaceAnalysis::Landmark::LandmarkData *data);

};
