/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCACCELERATOR_H__
#define __PXCACCELERATOR_H__
#include "pxcbase.h"
#include "pxcimage.h"
#include "pxcaudio.h"

class PXCAccelerator:public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x492D6A23);

    /* acceleration type: bit-OR'ed values */
    typedef pxcEnum AccelType;
    enum {
        ACCEL_TYPE_ANY         = 0,
        ACCEL_TYPE_CPU         = 0x000000FF,
        ACCEL_TYPE_GPU         = 0x0000FF00,
        ACCEL_TYPE_GPU_VIA_DX9 = 0x00000100,
    };

    typedef pxcEnum HandleType;
    enum {
        HANDLE_TYPE_DX9_DEVICE     =   0x00000002, /* IDirect3DDeviceManager9* */
        HANDLE_TYPE_DX11_DEVICE    =   0x00000008, /* ID3D11Device* */
        HANDLE_TYPE_OCL_DEVICE     =   0x00000020, /* cl_context* */
    };

    virtual AccelType PXCAPI QueryAccelType(void)=0;
    virtual pxcI32    PXCAPI QueryDevice(void)=0;
    virtual pxcStatus PXCAPI QueryHandle(HandleType type, pxcHDL *handle)=0;

    virtual pxcStatus PXCAPI SetDevice(pxcI32 device)=0;
    virtual pxcStatus PXCAPI SetHandle(HandleType type, pxcHDL handle)=0;

    virtual pxcStatus PXCAPI CreateImage(PXCImage::ImageInfo *info, PXCImage::ImageOption options, PXCImage::ImageData *data, PXCImage **image)=0;
    virtual pxcStatus PXCAPI CreateAudio(PXCAudio::AudioInfo *info, PXCAudio::AudioOption options, PXCAudio::AudioData *data, PXCAudio **audio)=0;
    pxcStatus __inline CreateImage(PXCImage::ImageInfo *info, PXCImage **image) { return CreateImage(info, 0, 0, image); }
    pxcStatus __inline CreateAudio(PXCAudio::AudioInfo *info, PXCAudio **audio) { return CreateAudio(info, 0, 0, audio); }
};

#endif
