/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCBASE_H__
#define __PXCBASE_H__
#include "pxcdefs.h"
#include "pxcstatus.h"

#define PXCAPI __stdcall

///////////////////////////////////////////////////////////////////////////////////////

class PXCBase {
public:
    enum { CUID=0 };
    enum { WORKING_PROFILE=0xffffffff };

    virtual void* PXCAPI DynamicCast(pxcUID cuid) = 0;
    template <class T> T* DynamicCast(void) { return (T*)this->DynamicCast(T::CUID); }

    virtual void  PXCAPI Release(void) = 0;
    void operator delete(void* pthis) {
        ((PXCBase*)pthis)->Release();
    }

    PXCBase(void) {}

private:
    /* prohibit using copy & assignment constructors */
    PXCBase(PXCBase const &);
    PXCBase& operator = (PXCBase const &);
};

class PXCCallbackBase {
public:
    enum { CUID=0 };
    virtual void* PXCAPI DynamicCast(pxcUID cuid) { return 0; }
    virtual void  PXCAPI Release(void) { delete this; }
};

#define PXC_CUID_OVERWRITE(X) enum { CUID=X }

///////////////////////////////////////////////////////////////////////////////////////

template <class T>
class PXCBaseImpl:public T {
public:

    virtual ~PXCBaseImpl(void) {}
    enum { CUID=T::CUID+1 };
    virtual void* PXCAPI DynamicCast(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T::CUID) return (T*)this;
        return 0; 
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

template <class T1, class T2>
class PXCBaseImpl2:public T1, public T2 {
public:

    virtual ~PXCBaseImpl2(void) {}
    enum { CUID = T1::CUID ^ T2::CUID };
    virtual void* PXCAPI DynamicCast(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

template <class T1, class T2, class T3>
class PXCBaseImpl3:public T1, public T2, public T3 {
public:

    virtual ~PXCBaseImpl3(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID };
    virtual void* PXCAPI DynamicCast(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

template <class T1, class T2, class T3, class T4>
class PXCBaseImpl4:public T1, public T2, public T3, public T4 {
public:

    virtual ~PXCBaseImpl4(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID ^ T4::CUID };
    virtual void* PXCAPI DynamicCast(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        if (cuid==T4::CUID) return (T4*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

#endif
