//=============================================================================
//     Filename : ghostGLCameraBase.cpp
//   Written by : Chris Tarr (ctarr@sensable.com)
//                Brandon Itkowitz (bitkowitz@sensable.com)
//      Project : GhostGL 1.0
//     Contains : GhostGL Camera Base Interface
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include "ghostGLCameraBase.h"

#include <ghostGLManager.h>

const double ghostGLCameraBase::kDefaultCameraX(0.0);
const double ghostGLCameraBase::kDefaultCameraY(0.0);
const double ghostGLCameraBase::kDefaultCameraZ(350.0);

const double ghostGLCameraBase::kDefaultFov(40.0);
const double ghostGLCameraBase::kDefaultNearClip(1.0);
const double ghostGLCameraBase::kDefaultFarClip(1000.0);

//=============================================================================
// Member Function : setLookAt
// ---------------------------
// Specify a gstPoint in world coordinates for the camera to look at.
//
// Note : If the bUpdate flag is not set to FALSE, the routine will
//        automatically update the current camera transform matrix
//=============================================================================
void ghostGLCameraBase::setLookAt(const gstPoint &point, gstBoolean bUpdate)
{
    m_lookAt = point;
    m_bUpdateTransform = TRUE;
    
    // force a camera view update
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : setUpVector
// -----------------------------
// Specify a gstVector in world coordinates that indicates the UP direction. 
// This will affect how the camera is oriented
//
// Note : If the bUpdate flag is not set to FALSE, the routine will
//        automatically update the current camera transform matrix
//=============================================================================
void ghostGLCameraBase::setUpVector(const gstVector &upVec, gstBoolean bUpdate)
{
    m_upVec = upVec;
    m_bUpdateTransform = TRUE;
    
    // force a camera view update
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : setClippingPlanes
// -----------------------------------
// Sets the near and far Z clipping planes. The values represent positive Z
// offsets from the camera
//
// Note : If the bUpdate flag is not set to FALSE, the routine will
//        automatically update the current camera transform matrix
//=============================================================================
void ghostGLCameraBase::setClippingPlanes(const double nearClip,
                                          const double farClip,
                                          gstBoolean bUpdate)
{
    m_nearClip = nearClip;
    m_farClip = farClip;
    m_bReshapeView = TRUE;
    
    // force a camera view update
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : setViewAngle
// ------------------------------
// Sets the field of view for the camera. This angle should be in degrees and
// is assumed to be an angle in the YZ plane.
//
// Note : If the bUpdate flag is not set to FALSE, the routine will
//        automatically update the current camera transform matrix
//=============================================================================
void ghostGLCameraBase::setViewAngle(const double viewAngle, gstBoolean bUpdate)
{
    m_viewAngle = viewAngle;
    m_bReshapeView = TRUE;
    
    // force a camera view update
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : setPosition
// -----------------------------
// Specify a gstPoint in world coordinates as the desired camera position
//
// Note : If the bUpdate flag is not set to FALSE, the routine will
//        automatically update the current camera transform matrix
//=============================================================================
void ghostGLCameraBase::setPosition(const gstPoint &position, gstBoolean bUpdate)
{
    // no need to recompute the transform for a translation
    //m_bUpdateTransform = TRUE;
    
    m_transform.set(3, 0, position.x());
    m_transform.set(3, 1, position.y());
    m_transform.set(3, 2, position.z());
    
    // force a camera view update
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : getPosition
// -----------------------------
// Gets the current camera position in world coordinates and returns a const
// gstPoint reference
//=============================================================================
gstPoint ghostGLCameraBase::getPosition() const
{
    gstPoint position;

    // update the matrix before getting its data directly
    //m_transform.update();
    
    // return the inverse of the position
    position[0] = m_transform[3][0];
    position[1] = m_transform[3][1];
    position[2] = m_transform[3][2];
    
    return position;
}

//=============================================================================
// Member Function : setTransformMatrix
// ------------------------------------
// Specify your own transform matrix for the camera. If the bInverseForm flag
// is set, the transform matrix is assumed to be in inverse form, which is how
// it is used when applied to the OpenGL matrix stack.
//
// Note : If the bUpdate flag is not set to FALSE, the routine will cause an
//        update, but will not modify the transform.
//=============================================================================
void ghostGLCameraBase::setTransformMatrix(gstTransformMatrix &cameraXform,
                                           gstBoolean bInverseForm,
                                           gstBoolean bUpdate)
{
    if (bInverseForm) {
        gstTransformMatrix invTransform = cameraXform;
        
        invTransform.inverse();
         
        m_transform = invTransform;
    } else {
        m_transform = cameraXform;
    }
    
    // don't update when the transform is set explicitly
    m_bUpdateTransform = FALSE;
    
    if (bUpdate)
        update();
}

//=============================================================================
// Member Function : getTransformMatrix
// ------------------------------------
// Returns the current transform matrix being used for drawing the camera. If
// bInverseForm is set to TRUE, then the actual inverse form used for drawing
// the camera against the OpenGL matrix stack is returned.
//=============================================================================
gstTransformMatrix ghostGLCameraBase::getTransformMatrix(gstBoolean bInverseForm)
{
    // make sure the matrix is current
    if (m_bUpdateTransform)
        loadCameraTransform();
    
    if (bInverseForm) {
        gstTransformMatrix invTransform = m_transform;
        
        // return the inverse of the inverse to the user
        invTransform.inverse();
        
        return invTransform;
    } else {
        return m_transform;
    }
}

//=============================================================================
// Member Function : initDefault
// -----------------------------
// Initializes the camera to its default state.
//
// Position = (0, 0, 350)
// Look At = (0, 0, 0)
// Up Vector = (0, 1, 0)
// View Angle = 40 degrees
// Near Clipping Plane = 1
// Far Clipping Plane = 1000
//
//=============================================================================
void ghostGLCameraBase::initDefault()
{
    // initialize the default camera position and orientation
    gstPoint pos(kDefaultCameraX, kDefaultCameraY, kDefaultCameraZ);
    setPosition(pos, FALSE);
    m_lookAt.init(0, 0, 0);
    m_upVec.init(0, 1, 0);
    m_viewAngle = kDefaultFov;
    m_nearClip = kDefaultNearClip;
    m_farClip = kDefaultFarClip;
    m_bUpdateTransform = TRUE;
    m_bReshapeView = TRUE;
}

//=============================================================================
// Member Function : reshape
// -------------------------
// Update the OpenGL viewport and perspective settings as a result of a
// change in width, height, view angle, or clipping planes
//=============================================================================
void ghostGLCameraBase::reshape(int width, int height)
{
    GLfloat aspectRatio;
    
    // if the width or height is negative, then this is an invalid reshape
    if (width < 0 || height < 0)
        return;
    
    // Correct for zero height/width
    m_windowHeight = height ? height : 1;
    m_windowWidth = width ? width : 1;
    
    aspectRatio = (GLfloat) m_windowWidth / m_windowHeight;
    
    // Resize viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    
    // initialize the GL matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(m_viewAngle,	       	   // field of view
                   aspectRatio,		       // aspect ratio
                   m_nearClip, m_farClip); // zNear and zFar clipping planes
}

//=============================================================================
// Member Function : update
// ------------------------
// Performs transform and view updates depending on whether a particular camera
// property changed.
//
// Note: If you choose to delay an update until all the camera settings are
//       made, this routine will determine whether to update the camera
//       transform and/or reshape the view
//=============================================================================
void ghostGLCameraBase::update()
{
    if (m_bUpdateTransform) {
        loadCameraTransform();
        m_bUpdateTransform = FALSE;
    }
    
    if (m_bReshapeView) {
        ghostGLCameraBase::reshape(m_windowWidth, m_windowHeight);
        m_bReshapeView = FALSE;
    } 

    // Specify the new location for the light source
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // draw the camera transform as the first scene transform
    applyTransform();

    // set the position of the point light source
    gstPoint cameraPos = getPosition();
    GLfloat light0_position[] = {cameraPos.x(), cameraPos.y(), cameraPos.z(), 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glPopMatrix();
}

//=============================================================================
// Member Function : applyTransform
// --------------------------------
// Draws the current camera transform onto the OpenGL matrix stack
//=============================================================================
void ghostGLCameraBase::applyTransform()
{
    static gstTransformMatrix transformMatrix; // only instantiate this object once
    
    // make sure the transform is current before drawing it
    if (m_bUpdateTransform)
        loadCameraTransform();
    
    transformMatrix = getTransformMatrix(TRUE);
    ghostGLDraw::drawTransformMatrix(transformMatrix);
}

//=============================================================================
// Member Function : setPosition
// -----------------------------
// Uses the current camera position, look at and up vector settings to determine
// the camera transform matrix
//=============================================================================
void ghostGLCameraBase::loadCameraTransform()
{
    float matrix[16];
    gstPoint cameraPos = getPosition();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(),
        m_lookAt.x(), m_lookAt.y(), m_lookAt.z(),
        m_upVec.x(), m_upVec.y(), m_upVec.z());
    
    glGetFloatv(GL_PROJECTION_MATRIX, matrix);
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m_transform.set(i, j, matrix[i * 4 + j]);
        
    // gluLookAt provides the inverse viewing transform, but we want to
    // maintain the actual transform
    m_transform.inverse();
}
