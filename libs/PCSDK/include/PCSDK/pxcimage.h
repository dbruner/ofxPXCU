/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCIMAGE_H__
#define __PXCIMAGE_H__
#include "pxcbase.h"

class PXCAccelerator;
class PXCImage:public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x24740F76);

    typedef pxcEnum ImageType;
    enum {
        IMAGE_TYPE_COLOR    =0x00010000,
        IMAGE_TYPE_DEPTH    =0x00020000,
        IMAGE_TYPE_MASK     =0xffff0000
    };

    typedef pxcEnum ColorFormat;
    enum {
        COLOR_FORMAT_YUY2       =   IMAGE_TYPE_COLOR,
        COLOR_FORMAT_NV12,
        COLOR_FORMAT_RGB32,     /* BGRA layout on little-endian machines */
        COLOR_FORMAT_RGB24,     /* BGR layout on little-endian machines */
        COLOR_FORMAT_GRAY,
        COLOR_FORMAT_DEPTH      =   IMAGE_TYPE_DEPTH,
        COLOR_FORMAT_VERTICES,
    };

    typedef pxcOption ImageOption;
    enum {
        IMAGE_OPTION_NO_UV_MAP          =   0x00000001,
        IMAGE_OPTION_NO_CONFIDENCE_MAP  =   0x00000002,
    };

    struct ImageInfo {
        pxcU32      width;
        pxcU32      height;
        ColorFormat format;
        pxcU32      reserved;
    };

    typedef pxcEnum SurfaceType;
    enum {
        SURFACE_TYPE_SYSTEM_MEMORY   =   0x00010000, /* if used in ImageData: planes[], pitches[] */
        SURFACE_TYPE_DX9_SURFACE     =   0x00020000, /* if used in ImageData: surfaceHandle with type IDirect3DSurface9* */
        SURFACE_TYPE_DX11_SURFACE    =   0x00040000, /* if used in ImageData: surfaceHandle with type ID3D11Texture2D* */
        SURFACE_TYPE_OCL_MEMOBJ      =   0x00080000, /* if used in ImageData: surfaceHandle with type cl_mem* */
    };

    struct ImageData {
        ColorFormat     format;
        SurfaceType     type;
        pxcU64          reserved;
        union {
            struct {    /* if type=SURFACE_TYPE_SYSTEM_MEMORY */
                pxcI32  pitches[4];
                pxcBYTE *planes[4];
            };
            pxcHDL surfaceHandle;
        };
    };

    enum Access {
        ACCESS_READ         = 1,
        ACCESS_WRITE        = 2,
        ACCESS_READ_WRITE   = ACCESS_READ | ACCESS_WRITE,
    };
    
    /* Query functions */
    virtual pxcStatus   PXCAPI  QueryInfo(ImageInfo *info)=0;
    virtual ImageOption PXCAPI  QueryOption()=0;
    virtual pxcU64      PXCAPI  QueryTimeStamp()=0;
    virtual PXCRectU32  PXCAPI  QueryROI()=0;

    /* Set functions */
    virtual pxcStatus   PXCAPI  SetOption(ImageOption options)=0;
    virtual pxcStatus   PXCAPI  SetTimeStamp(pxcU64 ts)=0;
    virtual pxcStatus   PXCAPI  SetROI(PXCRectU32 roi)=0;
    
    /* Copy data from another image */
    virtual pxcStatus   PXCAPI  CopyData(PXCImage *src_image)=0;

    /* Functions expose the internal buffer storage */ 
    virtual pxcStatus   PXCAPI  AcquireAccess(Access access, PXCAccelerator *accelerator, ColorFormat format, ImageData *data)=0;
    pxcStatus __inline AcquireAccess(Access access, PXCAccelerator *accelerator, ImageData *data) { return AcquireAccess(access, accelerator, (ColorFormat)0, data); }
    pxcStatus __inline AcquireAccess(Access access, ColorFormat format, ImageData *data) { return AcquireAccess(access, 0, format, data); }
    pxcStatus __inline AcquireAccess(Access access, ImageData *data) { return AcquireAccess(access, 0, (ColorFormat)0, data); }
    virtual pxcStatus   PXCAPI  TryAccess(Access access, PXCAccelerator *accelerator, ColorFormat format, ImageData *data)=0;
    pxcStatus __inline TryAccess(Access access, PXCAccelerator *accelerator, ImageData *data) { return TryAccess(access, accelerator, (ColorFormat)0, data); }
    pxcStatus __inline TryAccess(Access access, ColorFormat format, ImageData *data) { return TryAccess(access, 0, format, data); }
    pxcStatus __inline TryAccess(Access access, ImageData *data) { return TryAccess(access, 0, (ColorFormat)0, data); }
    virtual pxcStatus   PXCAPI  ReleaseAccess(ImageData *data)=0;
};

#endif
