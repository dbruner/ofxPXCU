/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/

#ifndef __PXCAUDIO_H__
#define __PXCAUDIO_H__

#include "pxcbase.h"

class PXCAccelerator;

class PXCAudio: public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x395A39C8);
    
    typedef pxcEnum AudioFormat;
    enum {
        AUDIO_FORMAT_PCM        = PXC_UID(16,'P','C','M'),  // 16-bit PCM
        AUDIO_FORMAT_IEEE_FLOAT = PXC_UID(32,'F','L','T'),  // 32-bit float
        AUDIO_FORMAT_SIZE_MASK  = 0xff
    };

    typedef pxcEnum ChannelMask;
    enum {
        CHANNEL_MASK_FRONT_LEFT     =0x00000001,
        CHANNEL_MASK_FRONT_RIGHT    =0x00000002,
        CHANNEL_MASK_FRONT_CENTER   =0x00000004,
        CHANNEL_MASK_LOW_FREQUENCY  =0x00000008,
        CHANNEL_MASK_BACK_LEFT      =0x00000010,
        CHANNEL_MASK_BACK_RIGHT     =0x00000020,
        CHANNEL_MASK_SIDE_LEFT      =0x00000200,
        CHANNEL_MASK_SIDE_RIGHT     =0x00000400,
    };

    struct AudioInfo {
        pxcU32      bufferSize;     /* buffer size in number samples */
        AudioFormat format;
        pxcU32      sampleRate;     /* samples per second */
        pxcU32      nchannels;      /* number of channels */
        ChannelMask channelMask;
        pxcU32      reserved[3];
    };

    typedef pxcEnum SurfaceType;
    enum {
        SURFACE_TYPE_SYSTEM_MEMORY   =   0x00000001, /* if used in AudioData: dataPtr points to buffer in system memory */
        SURFACE_TYPE_DX9_SURFACE     =   0x00000002, /* if used in AudioData: surfaceHandle with type IDirect3DSurface9* */
    };

    struct AudioData {
        AudioFormat     format;
        SurfaceType     type;
        pxcU32          dataSize;   // in number samples
        pxcU32          reserved;
        union {
            struct {    /* if type=SURFACE_TYPE_SYSTEM_MEMORY */
                pxcBYTE *dataPtr;
            };
            struct {
                pxcHDL  surfaceHandle;
            };
        };
    };

    typedef pxcOption AudioOption;

    enum Access {
        ACCESS_READ         = 1,
        ACCESS_WRITE        = 2,
        ACCESS_READ_WRITE   = ACCESS_READ | ACCESS_WRITE,
    };

    /* Query functions */
    virtual pxcStatus   PXCAPI  QueryInfo(AudioInfo *info)=0;
    virtual AudioOption PXCAPI  QueryOption(void)=0;
    virtual pxcU64      PXCAPI  QueryTimeStamp(void)=0;

    /* Set functions */
    virtual pxcStatus   PXCAPI  SetOption(AudioOption options)=0;
    virtual pxcStatus   PXCAPI  SetTimeStamp(pxcU64 ts)=0;

    /* Move data from another audio */
    virtual pxcStatus   PXCAPI  CopyData(PXCAudio *src_audio)=0;

    /* Functions expose the internal buffer storage */ 
    virtual pxcStatus   PXCAPI  AcquireAccess(Access access, PXCAccelerator *accelerator, AudioFormat format, AudioData *data)=0;
    pxcStatus __inline AcquireAccess(Access access, PXCAccelerator *accelerator, AudioData *data) { return AcquireAccess(access, accelerator, (AudioFormat)0, data); }
    pxcStatus __inline AcquireAccess(Access access, AudioFormat format, AudioData *data) { return AcquireAccess(access, 0, format, data); }
    pxcStatus __inline AcquireAccess(Access access, AudioData *data) { return AcquireAccess(access, 0, (AudioFormat)0, data); }
    virtual pxcStatus   PXCAPI  TryAccess(Access access, PXCAccelerator *accelerator, AudioFormat format, AudioData *data)=0;
    pxcStatus __inline TryAccess(Access access, PXCAccelerator *accelerator, AudioData *data) { return TryAccess(access, accelerator, (AudioFormat)0, data); }
    pxcStatus __inline TryAccess(Access access, AudioFormat format, AudioData *data) { return TryAccess(access, 0, format, data); }
    pxcStatus __inline TryAccess(Access access, AudioData *data) { return TryAccess(access, 0, (AudioFormat)0, data); }
    virtual pxcStatus   PXCAPI  ReleaseAccess(AudioData *data)=0;
};

#endif
