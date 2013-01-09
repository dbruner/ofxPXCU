/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCDEFS_H__
#define __PXCDEFS_H__

#if defined (WIN32) || defined(WIN64)
#include <wchar.h>
typedef wchar_t pxcCHAR;
#else
typedef char pxcCHAR;
#endif

typedef short               pxcI16;
typedef int                 pxcUID;
typedef int                 pxcI32;
typedef unsigned int        pxcU32;
typedef unsigned __int64    pxcU64;
typedef unsigned short int  pxcU16;
typedef unsigned char       pxcU8;
typedef unsigned char       pxcBYTE;
typedef int                 pxcEnum;
typedef int                 pxcBool;
typedef int                 pxcOption;
typedef void*               pxcHDL;
typedef float               pxcF32;
typedef double              pxcF64;

struct PXCRectU32 {
    pxcU32 x, y, w, h;
};

struct PXCSizeU32 {
    pxcU32 width, height;
};

struct PXCSizeF32 {
    pxcF32 width, height;
};

struct PXCRatioF32 {
    pxcF32 denominator;
    pxcF32 numerator;
};

struct PXCRatioU32 {
    pxcU32 denominator;
    pxcU32 numerator;
};

struct PXCPointF32 {
    pxcF32 x, y;
};

struct PXCPointU32 {
    pxcU32 x, y;
};

struct PXCRangeU32 {
    pxcU32 min, max;
};

struct PXCRangeI32 {
    pxcU32 min, max;
};

struct PXCRangeF32 {
    pxcF32 min, max;
};

struct PXCPoint3DF32 {
    pxcF32 x, y, z;
};

#define PXC_UID(X1,X2,X3,X4) ((pxcUID)(((unsigned int)(X4)<<24)+((unsigned int)(X3)<<16)+((unsigned int)(X2)<<8)+(unsigned int)(X1)))
#define PXC_DEFINE_CONST(Y,X) enum {Y=X}
#define PXC_DEFINE_UID(Y,X1,X2,X3,X4) enum {Y=PXC_UID(X1,X2,X3,X4)}

#endif