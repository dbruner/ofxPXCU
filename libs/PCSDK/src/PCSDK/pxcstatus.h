/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCSTATUS_H__
#define __PXCSTATUS_H__

enum pxcStatus {
    PXC_STATUS_NO_ERROR=0,

    /* errors */
    PXC_STATUS_FEATURE_UNSUPPORTED=     -1,     /* Unsupported feature */
    PXC_STATUS_PARAM_UNSUPPORTED=       -2,     /* Unsupported parameter(s) */
    PXC_STATUS_ITEM_UNAVAILABLE=        -3,     /* Item not found/not available */

    PXC_STATUS_HANDLE_INVALID=          -101,   /* Invalid session, algorithm instance, or pointer */
    PXC_STATUS_ALLOC_FAILED=            -102,   /* Memory allocation failure */

    PXC_STATUS_DEVICE_FAILED=           -201,   /* device failed due to malfunctioning */
    PXC_STATUS_DEVICE_LOST=             -202,   /* device failed due to unplug or unavailability */
    PXC_STATUS_DEVICE_BUSY=             -203,   /* device busy */

    PXC_STATUS_EXEC_ABORTED=            -301,   /* Execution aborted due to errors in upstream components */
    PXC_STATUS_EXEC_INPROGRESS=         -302,   /* Asynchronous operation is in execution */
    PXC_STATUS_EXEC_TIMEOUT=            -303,   /* Operation time out */

    /* warnings */
    PXC_STATUS_TIME_GAP=                101,    /* time gap in time stamps */
    PXC_STATUS_PARAM_INPLACE=           102,    /* the same parameters already defined */
};

#endif