/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCSESSION_H__
#define __PXCSESSION_H__
#include "pxcbase.h"
#include "pxcscheduler.h"
#include "pxcaccelerator.h"

class PXCSession:public PXCBase {
public:
    PXC_CUID_OVERWRITE(PXC_UID('S','E','S',' '));

    struct ImplVersion {
        pxcU16  major;
        pxcU16  minor;
    };

    typedef pxcEnum ImplGroup;
    enum {
        IMPL_GROUP_ANY                  =    0,

        IMPL_GROUP_OBJECT_RECOGNITION   =    0x00000001,
        IMPL_GROUP_SPEECH_RECOGNITION   =    0x00000002,
        IMPL_GROUP_SENSOR               =    0x00000004,

        IMPL_GROUP_CORE                 =    0x80000000,
        IMPL_GROUP_USER                 =    0x40000000,  /* user defined algorithms */
    };

    /* bit OR'ed values */
    typedef pxcEnum ImplSubgroup;
    enum {
        IMPL_SUBGROUP_ANY                   = 0,

        /* object recognition building blocks */
        IMPL_SUBGROUP_FACE_ANALYSIS         =    0x00000001,
        IMPL_SUBGROUP_GESTURE_RECOGNITION   =    0x00000010,

        /* sensor building blocks */
        IMPL_SUBGROUP_AUDIO_CAPTURE         =    0x00000001,
        IMPL_SUBGROUP_VIDEO_CAPTURE         =    0x00000002,

        /* speech recognition building blocks */
        IMPL_SUBGROUP_VOICE_RECOGNITION     =    0x00000001,
        IMPL_SUBGROUP_VOICE_SYNTHESIS       =    0x00000002,
    };

    struct ImplDesc {
        ImplGroup           group;              /* implementation group */
        ImplSubgroup        subgroup;           /* implementation sub-group */
        pxcUID              algorithm;          /* algorithm identifier */      
        pxcUID              iuid;               /* implementation unique id */
        ImplVersion         version;            /* implementation version */
        PXCAccelerator::AccelType acceleration; /* implementation acceleration support */
        pxcU32              merit;              /* implementation merit */
        pxcU32              vendor;             /* vendor */
        pxcUID              cuids[4];           /* interfaces supported by implementation */
        pxcCHAR             friendlyName[256];  /* friendly name */
        pxcU32              reserved[12];
    };

    virtual pxcStatus PXCAPI QueryVersion(ImplVersion *version)=0;
    virtual pxcStatus PXCAPI QueryImpl(ImplDesc *templat, pxcU32 idx, ImplDesc *desc)=0;

    virtual pxcStatus PXCAPI CreateScheduler(PXCScheduler **instance)=0;
    virtual pxcStatus PXCAPI CreateAccelerator(PXCAccelerator::AccelType type, PXCAccelerator **instance)=0;
    pxcStatus __inline CreateAccelerator(PXCAccelerator **instance) { return CreateAccelerator(PXCAccelerator::ACCEL_TYPE_CPU, instance); }

    virtual pxcStatus PXCAPI CreateImpl(PXCScheduler *sch, PXCAccelerator *accel, ImplDesc *desc, pxcUID iuid, pxcUID cuid, void **instance)=0;
    pxcStatus __inline CreateImpl(PXCScheduler *sch, PXCAccelerator *accel, ImplDesc *desc, pxcUID cuid, void **instance) { return CreateImpl(sch,accel,desc,0,cuid,instance); }
    pxcStatus __inline CreateImpl(pxcUID iuid, pxcUID cuid, void **instance) { return CreateImpl(0,0,0,iuid,cuid,instance); }
    pxcStatus __inline CreateImpl(pxcUID cuid, void **instance) { return CreateImpl(0,0,0,0,cuid,instance); }
    pxcStatus __inline CreateImpl(ImplDesc *desc, pxcUID cuid, void **instance) { return CreateImpl(0,0,desc,0,cuid,instance); }

    virtual pxcStatus PXCAPI LoadImplFromFile(pxcCHAR *moduleName)=0;
    virtual pxcStatus PXCAPI UnloadImplFromFile(pxcCHAR *moduleName)=0;
};

extern "C" pxcStatus PXCAPI PXCSession_Create(PXCSession **instance);

#endif