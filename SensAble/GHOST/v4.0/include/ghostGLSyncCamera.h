//=============================================================================
//   Filename : ghostGLSyncCamera.h
// Written by : Brandon Itkowitz (bitkowitz@sensable.com)
//              Chris Tarr (ctarr@sensable.com)
//    Project : GhostGL 1.0
//     Module : GhostGL Sync Camera
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GHOSTGL_SYNC_CAMERA_H
#define GHOSTGL_SYNC_CAMERA_H

#include <gstBasic.h>
#include <gstPoint.h>
#include <gstVector.h>
#include <gstTransformMatrix.h>
#include <gstPHANToM.h>
#include <gstBoundaryCube.h>

#include <ghostGLManager.h>
#include <ghostGLCameraBase.h>

// Automatically determines an optimal viewing transform and/or haptic
// orientation transform. This camera offers two principal modes of
// operation that get performed whenever a reshape or update occurs.
//
// By default, the sync camera operates in SYNC_WORKSPACE_TO_CAMERA mode, which
// works as a synchronization mechanism for setting the PHANToM base transform
// to align with the current camera transform. (i.e. what you feel is aligned
// with what you see).
//
// NOTE : You must have a distinct parent separator above the PHANToM for this
//        mechanism to work. Just having a root separator isn't enough.
//
//                  geometry
// root separator <                  
//                  PHANToM parent <
//                                   gstPHANToM instance
//
// The second mode of operation is called SYNC_CAMERA_TO_WORKSPACE. This method
// requires that you use a gstBoundaryCube to define a haptic boundary on your
// PHANToM. (Your scene graph requires a special configuration for this to work.
// The easiest way to set this up is to call the method attachMaximalBoundary()
// on your gstPHANToM instance. This method will attach a gstBoundaryCube as a
// sibling of the PHANToM in the scene.
//
// NOTE : You must have a distinct parent separator above the PHANToM for this
//        mechanism to work. Just having a root separator isn't enough.
//
//                  geometry
// root separator <                  gstBoundaryCube
//                  PHANToM parent <
//                                   gstPHANToM instance
//
// You will also need to explicitly set the workspace boundary that you want to
// use with the sync camera by calling setWorkspaceBoundary().
//
// Once this setup is complete, the camera will take care of modifying its
// transform and viewing parameters to see the entirety of the workspace. 
// If you wanted to modify how the PHANToM is oriented in the scene, you
// can change the transform of the PHANToM's parent and the camera will adapt
// to the new workspace transform. This mode allows you to "see what you feel".
//
// There is also a third useful option called SyncToWindow that works along
// with both of the previously mentioned sync modes. This option will adapt the
// workspace dimensions in X and Y to maximally fit the aspect ratio of your
// viewport. This option is enabled by default.
class ghostGLSyncCamera: public ghostGLCameraBase
{
public:
    
    typedef enum {
        SYNC_WORKSPACE_TO_CAMERA,
        SYNC_CAMERA_TO_WORKSPACE
    } CameraSyncMode;
    
    ghostGLSyncCamera();
    virtual ~ghostGLSyncCamera(){}
    
    // Allows you to enable/disable camera syncing altogether
    void setSyncEnabled(gstBoolean bEnable) {
        m_bSyncEnabled = bEnable;
    }
    
    gstBoolean isSyncEnabled() const {
        return m_bSyncEnabled;    
    }

    // Accessor methods for setting the principal camera sync mode
    void setSyncMode(CameraSyncMode syncMode) {
        m_syncMode = syncMode;
    }
    
    CameraSyncMode getSyncMode() const {
        return m_syncMode;
    }
    
    // Accessor methods for the workspace boundary
    void setWorkspaceBoundary(gstBoundaryCube *boundary) {
        m_pBoundary = boundary;
    }
    
    gstBoundaryCube *getWorkspaceBoundary() const {
        return m_pBoundary;
    }

    // Accessor methods for workspace offset used by SYNC_WORKSPACE_TO_CAMERA
    void setWorkspaceOffset(gstPoint &offset) {
        m_workspaceOffset = offset;
    }
    
    gstPoint getWorkspaceOffset() const {
        return m_workspaceOffset;
    }
            
    // Accessor methods for the SyncToWindow state
    void setSyncToWindow(gstBoolean bEnable) {
        m_bSyncToWindow = bEnable;
    }
    
    gstBoolean getSyncToWindow() const {
        return m_bSyncToWindow;
    }
    
    // Accessor methods for front plane or back plane syncing
    void setViewEntireWorkspace(gstBoolean bEnable) {
        m_bViewEntireWorkspace = bEnable;
    }
    
    gstBoolean getViewEntireWorkspace() const {
        return m_bViewEntireWorkspace;
    }
    
    // Accessor methods for OpenGL z-clipping of the workspace
    void setClipToWorkspace(gstBoolean bEnable) {
        m_bClipToWorkspace = bEnable;
        
        // Reset the z-clipping to the default values
        if (!bEnable)
            setClippingPlanes(kDefaultNearClip, kDefaultFarClip, FALSE);
    }
    
    gstBoolean getClipToWorkspace() const {
        return m_bClipToWorkspace;
    }
    
    virtual void reshape(int width, int height);
    virtual void update();

protected:

    gstPHANToM *getPhantomNode();
    
    virtual void syncWorkspaceToCamera(); // method used for SYNC_WORKSPACE_TO_CAMERA
    virtual void syncCameraToWorkspace(); // method used for SYNC_CAMERA_TO_WORKSPACE
    virtual void syncWorkspaceToWindow(); // method used for the SyncToWindow option
    virtual float setViewParameters();    // used by syncCameraToWorkspace
    
    gstBoolean m_bSyncEnabled;      // enable/disable all syncing activity

    CameraSyncMode m_syncMode;      // the mode of synchronization being used

    gstBoundaryCube *m_pBoundary;   // the phantom workspace boundary to view

    gstVector m_workspaceOffset;    // the offset between the camera and the workspace
            
    gstBoolean m_bSyncToWindow;     // enable/disable syncing of the workspace
                                    // dimensions to the viewport's aspect ratio
    
    gstBoolean m_bViewEntireWorkspace; // controls whether the camera is positioned
                                       // to view the entirety of the workspace
                                       // or just the back plane
    
    gstBoolean m_bClipToWorkspace;  // set the zClipping to clip to the workspace
                                    // front and back
};

#endif // GHOSTGL_SYNC_CAMERA_H
