//=============================================================================
//   Filename : ghostGLSyncCamera.cpp
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

#include "ghostGLSyncCamera.h"

#include <ghostGLManager.h>
#include <gstScene.h>

//=============================================================================
// Constructor : ghostGLSyncCamera
// -------------------------------
// Sets some default parameters for camera synchronization
//=============================================================================
ghostGLSyncCamera::ghostGLSyncCamera()
{
    m_pBoundary = NULL;
    m_bSyncEnabled = TRUE;                  // controls whether camera / workspace syncing is performed
    m_syncMode = SYNC_WORKSPACE_TO_CAMERA;  // specify the default sync mode
    m_workspaceOffset = getPosition();      // the initial workspace offset coincides with the default position
    m_bSyncToWindow = TRUE;                 // by default, the workspace will be updated to match the window
    m_bViewEntireWorkspace = TRUE;          // position the camera to view the entire workspace
    m_bClipToWorkspace = FALSE;             // do not auto-set the OpenGL clipping planes by default 
}

//=============================================================================
// Member Function : getPhantomNode
// --------------------------------
// Returns the gstPHANToM instance associated with the glManager
//=============================================================================
gstPHANToM *ghostGLSyncCamera::getPhantomNode()
{
    if (!m_pGLManager)
        return NULL;
    
    return m_pGLManager->getPhantomNode();
}

//=============================================================================
// Member Function : syncWorkspaceToCamera
// ---------------------------------------
// This sync method is used by the SYNC_WORKSPACE_TO_CAMERA mode. It uses the
// current workspace offset setting to pretranslate the PHANToM's base relative
// to the camera transform. The main utility of this sync method is that it
// ensures the haptics are aligned with the camera view (i.e. what you see
// is what you feel).
//=============================================================================
void ghostGLSyncCamera::syncWorkspaceToCamera()
{
    gstPHANToM *phantom = getPhantomNode();
    if (!phantom)
        return;
    
    gstSeparator *phantomBaseNode = (gstSeparator *) phantom->getParent();
    if (!phantomBaseNode)
        return;
    
    gstTransformMatrix resultMat, offsetMat;
    gstTransformMatrix cameraMat = getTransformMatrix();
    
    // pretranslate the workspace by an offset before applying the
    // current camera transform to determine the new workspace transform
    offsetMat.setTranslate(-m_workspaceOffset);
    mulM(resultMat, offsetMat, cameraMat);        
    
    phantomBaseNode->setTransformMatrix(resultMat);
}

//=============================================================================
// Member Function : syncCameraToWorkspace
// ---------------------------------------
// This sync method is used by the SYNC_CAMERA_TO_WORKSPACE mode. This sync
// mode requires that you've specified a gstBoundaryCube as a workspace for
// syncing. The main utility of this sync method is that it ensures the camera
// can see whatever is enclosed in the workspace boundary (i.e. what you feel
// is what you see). 
//=============================================================================
void ghostGLSyncCamera::syncCameraToWorkspace()
{
    float zOffset;

    // make sure the boundary cube is set
    if (!m_pBoundary)
        return;
    
    gstPHANToM *phantom = getPhantomNode();
    if (!phantom)
        return;
    
    gstSeparator *phantomBaseNode = (gstSeparator *) phantom->getParent();
    if (!phantomBaseNode)
        return;
    
    gstTransformMatrix boundaryMat = m_pBoundary->getTransformMatrix();
    gstTransformMatrix boundaryBase = phantomBaseNode->getTransformMatrix();
    gstTransformMatrix resultMat;
    
    // determine the zOffset / FOV for the camera to fit the boundary into view
    zOffset = setViewParameters();
        
    m_workspaceOffset = boundaryMat.getTranslation();
    
    // adjust the workspace zOffset to either view both the front and back planes
    // or just the back plane
    if (m_bViewEntireWorkspace)
        m_workspaceOffset[2] += zOffset + m_pBoundary->getLength() / 2.0;
    else
        m_workspaceOffset[2] += zOffset - m_pBoundary->getLength() / 2.0;
    
    // set the clipping planes to match the bounds of the workspace
    if (m_bClipToWorkspace)
        setClippingPlanes(m_workspaceOffset.z() - m_pBoundary->getLength() / 2.0, // zNear
		      m_workspaceOffset.z() + m_pBoundary->getLength() / 2.0,             // zFar
              FALSE);
    
    // apply the workspace offset
    boundaryMat.set(3, 0, m_workspaceOffset[0]);
    boundaryMat.set(3, 1, m_workspaceOffset[1]);
    boundaryMat.set(3, 2, m_workspaceOffset[2]);
    
    // now apply the parent transform to reorient the camera in space
    mulM(resultMat, boundaryMat, boundaryBase);
    
    // update the camera transform
    setTransformMatrix(resultMat, FALSE, FALSE);
}

//=============================================================================
// Member Function : syncWorkspaceToWindow
// ---------------------------------------
// Maximizes the workspace dimensions while constraining along the XY axes to
// the viewport's aspect ratio
//=============================================================================
void ghostGLSyncCamera::syncWorkspaceToWindow()
{
    const gstPHANToMInfoStruct *phantomInfo;
    float aspectRatio;
    float xmin, xmax,
          ymin, ymax,
          zmin, zmax;
    
    phantomInfo = getPhantomNode()->getInfo();
    
    // determine what the maximal bounds are
    xmin = phantomInfo->maxUsableWorkspace.xMin;
    xmax = phantomInfo->maxUsableWorkspace.xMax;
    ymin = phantomInfo->maxUsableWorkspace.yMin;
    ymax = phantomInfo->maxUsableWorkspace.yMax;
    zmin = phantomInfo->maxUsableWorkspace.zMin;
    zmax = phantomInfo->maxUsableWorkspace.zMax;
    
    m_pBoundary->setWidth(xmax - xmin);    // width along the x axis
    m_pBoundary->setHeight(ymax - ymin);   // height along the y axis
    m_pBoundary->setLength(zmax - zmin);   // length along the z axis
    
    // adjust the dimensions of the workspace to match the specified aspect ratio    
    aspectRatio = (float) getWindowWidth() / (float) getWindowHeight();
    
    // preserve the smaller of the two dimensions but don't exceed the max dimensions
    if (m_pBoundary->getWidth() < m_pBoundary->getHeight()) {
        if (m_pBoundary->getWidth() / aspectRatio > m_pBoundary->getHeight())
            m_pBoundary->setWidth(m_pBoundary->getHeight() * aspectRatio);
        else {
            m_pBoundary->setHeight(m_pBoundary->getWidth() / aspectRatio);
        }
    } else {
        if (m_pBoundary->getHeight() * aspectRatio > m_pBoundary->getWidth())
            m_pBoundary->setHeight(m_pBoundary->getWidth() / aspectRatio);
        else {
            m_pBoundary->setWidth(m_pBoundary->getHeight() * aspectRatio);
        }
    }
    
    // translate the workspace so that its logical center coincides with the reset position
    m_pBoundary->setTranslate(0.0, 0.0, 0.0);
    m_pBoundary->translate((xmax + xmin) / 2.0 * (m_pBoundary->getWidth() / (xmax - xmin)), 0.0, 0.0);
    m_pBoundary->translate(0.0, (ymax + ymin) / 2.0 * (m_pBoundary->getHeight() / (ymax - ymin)), 0.0);
    m_pBoundary->translate(0.0, 0.0, (zmax + zmin) / 2.0 * (m_pBoundary->getLength() / (zmax - zmin)));
    
}

//=============================================================================
// Member Function : reshape
// -------------------------
// A reshape of the viewport triggers a SyncToWindow update
//=============================================================================
void ghostGLSyncCamera::reshape(int width, int height)
{
    CameraSyncMode tempSyncMode;
    
    ghostGLCameraBase::reshape(width, height);
    
    // before calling update, make sure that if the workspace is being sync'd
    // to the window's aspect ratio then the camera needs to be sync'd to the
    // workspace.
    if (m_bSyncToWindow && m_syncMode != SYNC_CAMERA_TO_WORKSPACE) {
        tempSyncMode = m_syncMode;
        m_syncMode = SYNC_CAMERA_TO_WORKSPACE;
        update();
        m_syncMode = tempSyncMode;
    } else {
        update();
    }
}

//=============================================================================
// Member Function : update
// ------------------------
// Performs an update on the camera base and then synchronizes the camera via
// an update on the camera transform and/or PHANToM base transform.
//=============================================================================
void ghostGLSyncCamera::update()
{
    gstBoolean bStartServoLoop = FALSE;
    
    // handle updating the camera transform and viewport
    ghostGLCameraBase::update();
    
    // test whether any syncing should be performed
    if (!m_bSyncEnabled)
        return;
    
    // handle disabling the servoloop because of an impending change
    if (m_bSyncToWindow || m_syncMode == SYNC_WORKSPACE_TO_CAMERA)
        if (m_pGLManager && m_pGLManager->getScene() && !m_pGLManager->getScene()->getDoneServoLoop()) {
            m_pGLManager->getScene()->lock();
            bStartServoLoop = TRUE;
        }
        
    // update the dimensions of the workspace boundary
    if (m_pBoundary && m_bSyncToWindow && m_syncMode == SYNC_CAMERA_TO_WORKSPACE)
        syncWorkspaceToWindow();
    
    if (m_syncMode == SYNC_WORKSPACE_TO_CAMERA) 
        syncWorkspaceToCamera();
    else
        syncCameraToWorkspace();
    
    // test whether the servoloop needs to be re-enabled
    if (bStartServoLoop) {
        m_pGLManager->getScene()->unlock();
    }

    // updating the camera transform and viewport again if anything changed
    ghostGLCameraBase::update();
}

//=============================================================================
// Member Function : setViewParameters
// -----------------------------------
// Determines an offset for the camera and an appropriate view angle to
// optimally fit the workspace boundary into view.
//=============================================================================
float ghostGLSyncCamera::setViewParameters()
{
    float length, height;
    float zOffset, viewAngle;

    // multiple of the workspace length
    // these limits depend on whether the camera will be focusing on
    // the front plane or the back plane of the workspace
    const float kOffsetMinLimit = m_bViewEntireWorkspace ? 0.5 : 2.5;
    const float kOffsetMaxLimit = m_bViewEntireWorkspace ? 3.0 : 5.0;

    // field of view limits in degrees
    const float kViewAngleMinLimit = 0.0;
    const float kViewAngleMaxLimit = 120.0;

    // retrieve the workspace dimensions for easy access
    height = m_pBoundary->getHeight();
    length = m_pBoundary->getLength();

    // determine the zOffset as a first pass
    viewAngle = getViewAngle();
    zOffset = (height / 2.0) / tan(viewAngle / 2.0 * M_PI / 180.0);

    // check if the calculated zOffset is within the allowable bounds
    if (zOffset >= kOffsetMinLimit * length && zOffset <= kOffsetMaxLimit * length)
        return zOffset;

    // clamp the zOffset and determine a new view angle
    if (zOffset < kOffsetMinLimit * length)
        zOffset = kOffsetMinLimit * length;
    else if (zOffset > kOffsetMaxLimit * length)
        zOffset = kOffsetMaxLimit * length;

    viewAngle = 2 * atan2(height / 2.0, zOffset) * 180.0 / M_PI;
    
    // check if the calculated viewAngle is within the allowable bounds
    if (viewAngle >= kViewAngleMinLimit && viewAngle <= kViewAngleMaxLimit) {
        setViewAngle(viewAngle, FALSE);
        return zOffset;
    }
    
    // clamp the view angle and determine a new zOffset
    if (viewAngle < kViewAngleMinLimit)
        viewAngle = kViewAngleMinLimit;
    else if (viewAngle > kViewAngleMaxLimit)
        viewAngle = kViewAngleMaxLimit;

    // this is the best we can do without getting too computationally expensive
    setViewAngle(viewAngle, FALSE);
    zOffset = (height / 2.0) / tan(viewAngle / 2.0 * M_PI / 180.0);

    return zOffset;
}
