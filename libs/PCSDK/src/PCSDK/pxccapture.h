/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCCAPTURE_H__
#define __PXCCAPTURE_H__
#include "pxcimage.h"
#include "pxcaudio.h"
#include "pxcsession.h"

class PXCCapture:public PXCBase {
public:
    PXC_CUID_OVERWRITE(0xD8971845);

    struct DeviceInfo {
        pxcCHAR         name[256];      /* device name */
        pxcCHAR         did[256];       /* device identifier */
        pxcU32          didx;           /* device index */
        pxcEnum         options;        /* additional options */
        pxcU32          reserved[6];
    };

    class Device : public PXCBase {
    public:

        PXC_CUID_OVERWRITE(0x938401C4);
        PXC_DEFINE_CONST(DEVCAP_LIMIT,128);

        struct StreamInfo {
            pxcU32              sidx;           /* stream index */
            pxcUID              cuid;           /* the base stream interface identifier */
            PXCImage::ImageType imageType;
            pxcU32              reserved[5];
        };

        enum Property {
            /* Single value properties */
            PROPERTY_COLOR_EXPOSURE             =   1,
            PROPERTY_COLOR_BRIGHTNESS           =   2,
            PROPERTY_COLOR_CONTRAST             =   3,
            PROPERTY_COLOR_SATURATION           =   4,
            PROPERTY_COLOR_HUE                  =   5,
            PROPERTY_COLOR_GAMMA                =   6,
            PROPERTY_COLOR_WHITE_BALANCE        =   7,
            PROPERTY_COLOR_SHARPNESS            =   8,
            PROPERTY_COLOR_BACK_LIGHT_COMPENSATION  =   9,
            PROPERTY_COLOR_GAIN                     =   10,
            PROPERTY_AUDIO_MIX_LEVEL            =   100,

            PROPERTY_DEPTH_SATURATION_VALUE     =   200,
            PROPERTY_DEPTH_LOW_CONFIDENCE_VALUE =   201,
            PROPERTY_DEPTH_CONFIDENCE_THRESHOLD =   202,
            PROPERTY_DEPTH_SMOOTHING            =   203,

            /* Two value properties */
            PROPERTY_COLOR_FIELD_OF_VIEW        =   1000,
            PROPERTY_COLOR_SENSOR_RANGE         =   1002,
            PROPERTY_COLOR_FOCAL_LENGTH         =   1006,
            PROPERTY_COLOR_PRINCIPAL_POINT      =   1008,

            PROPERTY_DEPTH_FIELD_OF_VIEW        =   2000,
            PROPERTY_DEPTH_SENSOR_RANGE         =   2002,
            PROPERTY_DEPTH_FOCAL_LENGTH         =   2006,
            PROPERTY_DEPTH_PRINCIPAL_POINT      =   2008,

            /* Three value properties */
            PROPERTY_ACCELEROMETER_READING      =   3000,

            /* Customized properties */
            PROPERTY_CUSTOMIZED=0x04000000,
        };

        struct DeviceCap {
            Property    label;
            pxcF32      value;
        };

        virtual pxcStatus PXCAPI QueryDevice(DeviceInfo *dinfo)=0;
        virtual pxcStatus PXCAPI QueryStream(pxcU32 sidx, StreamInfo *sinfo)=0;

        virtual pxcStatus PXCAPI CreateStream(pxcU32 sidx, pxcUID cuid, void **stream)=0;

        /* Device Properties */
        virtual pxcStatus PXCAPI QueryPropertyInfo(Property label, PXCRangeF32 *range, pxcF32 *step, pxcF32 *def, pxcBool *isAuto) { return PXC_STATUS_FEATURE_UNSUPPORTED; }
        virtual pxcStatus PXCAPI QueryProperty(Property label, pxcF32 *value) { return PXC_STATUS_FEATURE_UNSUPPORTED; }
        pxcStatus __inline QueryPropertyAsPoint(Property label, PXCPointF32 *point) {
            QueryProperty(label,&point->x);
            return QueryProperty((Property)(label+1),&point->y);
        }
        pxcStatus __inline QueryPropertyAs3DPoint(Property label, PXCPoint3DF32 *point) {
            QueryProperty(label,&point->x);
            QueryProperty((Property)(label+1),&point->y);
            return QueryProperty((Property)(label+2),&point->z);
        }
        pxcStatus __inline QueryPropertyAsRange(Property label, PXCRangeF32 *range) {
            QueryProperty(label,&range->min);
            return QueryProperty((Property)(label+1),&range->max);
        }
        pxcStatus __inline QueryPropertyAsSize(Property label, PXCSizeF32 *size) {
            QueryProperty(label,&size->width);
            return QueryProperty((Property)(label+1),&size->height);
        }

        virtual pxcStatus PXCAPI SetPropertyAuto(Property pty, pxcBool ifauto) { return PXC_STATUS_FEATURE_UNSUPPORTED; }
        virtual pxcStatus PXCAPI SetProperty(Property pty, pxcF32 value) { return PXC_STATUS_FEATURE_UNSUPPORTED; }
    };

    class VideoStream : public PXCBase {
    public:
        PXC_CUID_OVERWRITE(0x83A60463);
        PXC_DEFINE_CONST(STREAM_LIMIT,4);
        PXC_DEFINE_CONST(DEVCAP_LIMIT,Device::DEVCAP_LIMIT);
        typedef PXCImage *Images[STREAM_LIMIT];

        struct ProfileInfo {
            PXCImage::ImageInfo     imageInfo;
            PXCRatioU32             frameRateMin;
            PXCRatioU32             frameRateMax;
            PXCImage::ImageOption   imageOptions;           // image creation options
            pxcU32                  reserved[8];
        };

        struct DataDesc {
            struct StreamDesc {
                PXCSizeU32              sizeMin;
                PXCSizeU32              sizeMax;
                PXCImage::ColorFormat   format;
                PXCImage::ImageOption   options;
                pxcU32                  reserved[10];
            };
            typedef Device::DeviceCap DeviceCap;
            StreamDesc          streams[STREAM_LIMIT];
            Device::DeviceCap   devCaps[Device::DEVCAP_LIMIT];
        };

        virtual pxcStatus PXCAPI QueryStream(Device::StreamInfo *sinfo)=0;
        virtual pxcStatus PXCAPI QueryProfile(pxcU32 pidx, ProfileInfo *pinfo)=0;
        pxcStatus __inline QueryProfile(ProfileInfo *pinfo) { return QueryProfile((pxcU32)WORKING_PROFILE,pinfo); }
        virtual pxcStatus PXCAPI SetProfile(ProfileInfo *pinfo)=0;
        virtual pxcStatus PXCAPI ReadStreamAsync(PXCImage **image, PXCScheduler::SyncPoint **sp)=0;
    };

    class AudioStream : public PXCBase {
    public:
        PXC_CUID_OVERWRITE(0x8C951392);

        struct ProfileInfo {
            PXCAudio::AudioInfo audioInfo;
            pxcOption           audioOptions;
            pxcU32              reserved[7];
        };

        struct DataDesc {
            PXCAudio::AudioInfo info;
            pxcOption           options;
            pxcU32              reserved[7+128*2];
        };

        virtual pxcStatus PXCAPI QueryStream(Device::StreamInfo *sinfo)=0;
        virtual pxcStatus PXCAPI QueryProfile(pxcU32 pidx, ProfileInfo *profile)=0;
        pxcStatus __inline QueryProfile(ProfileInfo *profile) { return QueryProfile((pxcU32)WORKING_PROFILE,profile); }
        virtual pxcStatus PXCAPI SetProfile(ProfileInfo *profile)=0;
        virtual pxcStatus PXCAPI ReadStreamAsync(PXCAudio **audio, PXCScheduler::SyncPoint **sp)=0;
    };

//    class Projection: public PXCBase { /* extends Device */
//    public:
//        PXC_CUID_OVERWRITE(0x494A8537);
//
//VERTICES:     virtual pxcStatus PXCAPI ProjectImageToRealWorld(PXCImage *image, pxcU32 npoints, PXCPointF32 *pos2d, PXCPoint3DF32 *pos3d)=0;
//MISSING:      virtual pxcStatus PXCAPI ProjectRealWorldToImage(pxcU32 npoints, PXCPoint3DF32 *pos3d, PXCPointF32 *pos2d)=0;
//UVMAP:        virtual pxcStatus PXCAPI MapDepthToColorCoordinates(PXCImage *image, pxcU32 npoints, PXCPointF32 *posd, PXCPointF32 *posc)=0;
//    };

    virtual pxcStatus PXCAPI QueryDevice(pxcU32 didx, DeviceInfo *dinfo)=0;
    virtual pxcStatus PXCAPI CreateDevice(pxcU32 didx, Device **device)=0;
};

#endif
