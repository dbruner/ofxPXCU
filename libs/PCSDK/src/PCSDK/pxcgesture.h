/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#ifndef __PXCGESTURE2_H__
#define __PXCGESTURE2_H__
#include "pxcsession.h"
#include "pxccapture.h"

/// The Finger Tracking Module Interface
class PXCGesture:public PXCBase {
public:
    PXC_CUID_OVERWRITE(PXC_UID('G','S','T','R'));

    /// The Geometric Node Data Structure  
    struct GeoNode {
        typedef pxcEnum Label; // The geometric node identifier (bit-OR'ed of global and local identifiers)
        enum {
            LABEL_ANY=0,                                // No geometric node
            LABEL_MASK_BODY             =0xffffff00,    // AND this mask with the geometric node identifier to get the global identifier
            LABEL_MASK_DETAILS          =0x000000ff,    // AND this mask with the geometric node identifier to get the local identifier

            LABEL_BODY_ELBOW_PRIMARY    =0x00004000,    // Global geometric node: The first tracked elbow
            LABEL_BODY_ELBOW_LEFT       =0x00004000,    // Global geometric node: The left elbow
            LABEL_BODY_ELBOW_SECONDARY  =0x00008000,    // Global geometric node: The second tracked elbow
            LABEL_BODY_ELBOW_RIGHT      =0x00008000,    // Global geometric node: The right elbow
            LABEL_BODY_HAND_PRIMARY     =0x00040000,    // Global geometric node: The first tracked hand/palm
            LABEL_BODY_HAND_LEFT        =0x00040000,    // Global geometric node: The left hand/palm
            LABEL_BODY_HAND_SECONDARY   =0x00080000,    // Global geometric node: The second tracked hand/palm
            LABEL_BODY_HAND_RIGHT       =0x00080000,    // Global geometric node: The right hand/palm

            LABEL_FINGER_THUMB          =1,        // Local geometric node: The thumb finger
            LABEL_FINGER_INDEX,                    // Local geometric node: The index finger
            LABEL_FINGER_MIDDLE,                   // Local geometric node: The middle finger
            LABEL_FINGER_RING,                     // Local geometric node: The ring finger
            LABEL_FINGER_PINKY,                    // Local geometric node: The pinky finger

            LABEL_HAND_FINGERTIP,                  // Local geometric node: The fingertip
            LABEL_HAND_UPPER,                      // Local geometric node: The upper grabbing point
            LABEL_HAND_MIDDLE,                     // Local geometric node: The middle grabbing point
            LABEL_HAND_LOWER,                      // Local geometric node: The lower grabbing point
        };

        /// The body side enumerator
        enum Side {
            LABEL_SIDE_ANY=0,                      // Unknown body side
            LABEL_LEFT,                            // The left body side
            LABEL_RIGHT,                           // The right body side
        };

        /// The palm openness state enumerator
        enum Openness {
            LABEL_OPENNESS_ANY=0,                 // Unknown palm openness state
            LABEL_CLOSE,                          // Palm is closed
            LABEL_OPEN,                           // Palm is open
        };

        pxcU64          timeStamp;                // Time stamp in 100ns when the geometric node data is recorded
        pxcUID          user;                     // reserved; must be zero
        Label           body;                     // The geometric node identifier
        Side            side;                     // The geometric node side information
        pxcU32          confidence;               // The confidence score ranging from 0 to 100
        PXCPoint3DF32   positionWorld;            // The geometric node position in the world coordinates
        PXCPoint3DF32   positionImage;            // The geometric node position in the depth coordinates
        pxcU32          reserved2[4];

        union {
            pxcU32      reserved3[16];
            struct { // fingertip
                pxcF32          radiusWorld;      // The fingertip radius in the world coordinates
                pxcF32          radiusImage;      // The fingertip radius in the depth coordinates
            };
            struct { // hand
                PXCPoint3DF32   massCenterWorld;  // The hand center of mass in the world coordinates
                PXCPoint3DF32   massCenterImage;  // The hand center of mass in the depth coordinates
                PXCPoint3DF32   normal;           // The palm center normal vector in the world coordinates
                pxcU32          openness;         // The palm openness value from 0 (close) to 100 (open)
                Openness        opennessState;    // The palm openness state
            };
        };
    };

    /// The Blob data structure
    struct Blob {
        enum Label {            // The blob identifier
            LABEL_ANY=0,        // No blob
            LABEL_SCENE=1,      // Blob: The scene labelmap
        };

        pxcU64  timeStamp;            // Time stamp in 100ns when the blob is corresponding to
        Label   name;                 // The blob identifier

        pxcU32  labelBackground;      // The labelmap pixel value for the background: 0-255. If -1, the background is not available
        pxcU32  labelLeftHand;        // The labelmap pixel value for the left hand: 0-255. If -1 the left hand is not available
        pxcU32  labelRightHand;       // The labelmap pixel value for the right hand: 0-255. If -1, the right hand is not available
        pxcU32  reserved[26];
    };

    /// The pose/gesture data structure
    struct Gesture {
        typedef pxcEnum Label;          // The pose/gesture identifier (bit-OR'ed value of set and detailed identifiers)
        enum {
            LABEL_ANY=0,                                // No gesture
            LABEL_MASK_SET          =   0xffff0000,     // AND this mask with the pose/gesture identifier to get the pose/gesture set.
            LABEL_MASK_DETAILS      =   0x0000ffff,     // AND this mask with the pose/gesture identifier to get the pose/gesture details within a set.

            LABEL_SET_HAND          =   0x00010000,     // Set of hand gestures
            LABEL_SET_NAVIGATION    =   0x00020000,     // Set of navigation gestures
            LABEL_SET_POSE          =   0x00040000,     // Set of poses
            LABEL_SET_CUSTOMIZED    =   0x00080000,     // Set of customized poses/gestures

            /* predefined nativation gestures */
            LABEL_NAV_SWIPE_LEFT    =   LABEL_SET_NAVIGATION+1,     // The swipe left navigation gesture
            LABEL_NAV_SWIPE_RIGHT,                                  // The swipe right navigation gesture

            /* predefined common hand gestures */
            LABEL_HAND_WAVE         =    LABEL_SET_HAND+1,          // The wave hand gesture

            /* predefined common hand poses */
            LABEL_POSE_THUMB_UP     =    LABEL_SET_POSE+1,          // The thumb up pose
            LABEL_POSE_THUMB_DOWN,                                  // The thumb down pose
            LABEL_POSE_PEACE,                                       // The peace/victory pose
        };

        pxcU64          timeStamp;          // Time stamp in 100ns when the pose/gesture occurs
        pxcUID          user;               // reserved; must be zero
        GeoNode::Label  body;               // The geometric node that the pose/gesture associates with
        Label           label;              // The pose/gesture identifier
        pxcU32          confidence;         // The confidence value of the pose/gesture maturity, ranging from 0 to 100.
        pxcBool         active;             // Pose only: The pose event is a found or lost event.
        pxcU32          reserved[9];

        /// The pose/gesture event handler interface
        class Handler:public PXCCallbackBase {
        public:
            PXC_CUID_OVERWRITE(PXC_UID('G','H','D','L'));

            /// The pose/gesture event handler function
            /// The application should not block execution by implementing lengthy process in this function
            /// gesture        The pose/gesture data structure
            virtual void PXCAPI OnGesture(Gesture *gesture)=0;
        };
    };

    /// The alert data structure
    struct Alert {
        typedef pxcEnum Label;    // The alert event identifier
        enum {
            LABEL_ANY               =            0,  // No alert
            LABEL_FOV_LEFT          =   0x00000001,  // Alert: The tracking object touches the left field of view
            LABEL_FOV_RIGHT         =   0x00000002,  // Alert: The tracking object touches the right field of view
            LABEL_FOV_TOP           =   0x00000004,  // Alert: The tracking object touches the top field of view
            LABEL_FOV_BOTTOM        =   0x00000008,  // Alert: The tracking object touches the bottom field of view
            LABEL_FOV_BLOCKED       =   0x00000010,  // Alert: The field of view is blocked
            LABEL_FOV_OK            =   0x00000020,  // Alert: The tracking object is within field of view
            LABEL_GEONODE_ACTIVE    =   0x00000040,  // Alert: The tracking object is found
            LABEL_GEONODE_INACTIVE  =   0x00000080,  // Alert: The tracking object is lost
        };

        pxcU64          timeStamp;                   // Time stamp in 100ns when the alert occurs
        pxcUID          user;                        // reserved; must be zero
        GeoNode::Label  body;                        // The geometric node identifier that the alert associates with
        Label           label;                       // The alert event identifier
        pxcU32          reserved[3];

        /// The Alert Handler Interface
        class Handler:public PXCCallbackBase {
        public:
            PXC_CUID_OVERWRITE(PXC_UID('A','H','D','L'));

            /// The alert callback function that the application must implement.
            /// The application should not block the execution by implementing processing inside this function. 
            /// alert        The alert data structure
            virtual void PXCAPI OnAlert(Alert *alert)=0;
        };
    };

    /// The Finger Tracking Module Configuration Structure
    struct ProfileInfo {
        PXCCapture::VideoStream::DataDesc inputs;   // Module input requirements
        Gesture::Label      sets;                   // Bit-OR'ed value to specify sets of poses/gestures that should be tracked
        GeoNode::Label      bodies;                 // Bit-OR'ed value to specify geometric nodes to be tracked
        Blob::Label         blobs;                  // Bit-OR'ed value to specify blobs to be tracked
        Alert::Label        alerts;                 // Bit-OR'ed value to specify alerts to be fired
        GeoNode::Label      nodeAlerts;             // Bit-OR'ed value to specify geometric nodes that Alert::GEONODE_ACTIVE/INACTIVE should fire

        pxcU32              activationDistance;     // activation distance in centimeters
        pxcU32              reserved[6];          
    };

    /// Get available configuration(s).
    /// pidx        Zero-based index to enumerate all available configurations
    /// pinfo       The configuration structure, to be returned.
    virtual pxcStatus PXCAPI QueryProfile(pxcU32 pidx, ProfileInfo *pinfo)=0;

    /// Get the current working configuration
    /// pinfo       The configuration structure, to be returned.
    pxcStatus __inline QueryProfile(ProfileInfo *pinfo) { return QueryProfile(WORKING_PROFILE,pinfo); }

    /// Set the working configuration
    /// pinfo       The configuration structure
    virtual pxcStatus PXCAPI SetProfile(ProfileInfo *pinfo)=0;

    /// This function is reserved.
    virtual pxcStatus PXCAPI QueryUser(pxcU32 idx, pxcUID *user) { *user=0; return (idx)?PXC_STATUS_ITEM_UNAVAILABLE:PXC_STATUS_NO_ERROR; }

    /// Get the blob data
    /// label       The blob identifier
    /// idx         reserved; must be zero
    /// data        The blob data structure, to be returned
    virtual pxcStatus PXCAPI QueryBlobData(Blob::Label label, pxcU32 idx, Blob *data) { return PXC_STATUS_ITEM_UNAVAILABLE; }

    /// Get the blob image
    /// label       The blob identifier
    /// idx         reserved; must be zero
    /// image       The blob image, to be returned
    virtual pxcStatus PXCAPI QueryBlobImage(Blob::Label label, pxcU32 idx, PXCImage **image) { return PXC_STATUS_ITEM_UNAVAILABLE; }

    /// Get the details of the specified geometric node.
    /// user        reserved; must be zero
    /// body        The geometric node identifier
    /// data        The geometric node data structure, to be returned
    virtual pxcStatus PXCAPI QueryNodeData(pxcUID user, GeoNode::Label body, GeoNode *data) { return PXC_STATUS_ITEM_UNAVAILABLE; }

    /// Get the details of an array of geometric nodes.
    /// user        reserved; must be zero
    /// body        The geometric node identifier
    /// ndata       Number of geometric nodes
    /// data        The array of geometric node data structures
    pxcStatus __inline QueryNodeData(pxcUID user, GeoNode::Label body, pxcU32 ndata, GeoNode *data) {
        pxcStatus sts=PXC_STATUS_ITEM_UNAVAILABLE;
        for (pxcU32 i=0;i<ndata;i++,body++) if (QueryNodeData(user,body,&data[i])<PXC_STATUS_NO_ERROR) 
            data[i].body=GeoNode::LABEL_ANY; else sts=PXC_STATUS_NO_ERROR;
        return sts;
    }

    /// Get the active pose/gesture list.
    /// user        reserved; must be zero
    /// body        The geometric node identifier that the pose/gesture(s) associate with
    /// idx         Zero-basd index to enumerate all available poses/gestures
    /// data        The pose/gesture data structure, to be returned
    virtual pxcStatus PXCAPI QueryGestureData(pxcUID user, GeoNode::Label body, pxcU32 idx, Gesture *data) { return PXC_STATUS_ITEM_UNAVAILABLE; }

    /// Process the input image(s) for finger tracking and pose/gesture recognition.
    /// images      The input image arrays
    /// sp          The sync point, to be returned
    virtual pxcStatus PXCAPI ProcessImageAsync(PXCImage *images[], PXCScheduler::SyncPoint **sp)=0;

    /// Subscribe to the alert events.
    /// handler     The alert handler instance
    virtual pxcStatus PXCAPI SubscribeAlert(Alert::Handler *handler) { return PXC_STATUS_NO_ERROR; }

    /// Subscribe to the pose/gesture events.
    /// threshold   reserved; must be 100.
    /// handler     The gesture handler instance
    virtual pxcStatus PXCAPI SubscribeGesture(pxcU32 threshold, Gesture::Handler *handler) { return PXC_STATUS_NO_ERROR; }
};

#endif
