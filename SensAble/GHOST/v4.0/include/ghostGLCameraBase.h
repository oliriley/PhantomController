//=============================================================================
//   Filename : ghostGLCameraBase.h
// Written by : Brandon Itkowitz (bitkowitz@sensable.com)
//              Chris Tarr (ctarr@sensable.com)
//    Project : GhostGL 1.0
//     Module : GhostGL Camera Base Interface
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GHOSTGL_CAMERA_BASE_H
#define GHOSTGL_CAMERA_BASE_H

#include <gstBasic.h>
#include <gstPoint.h>
#include <gstVector.h>
#include <gstTransformMatrix.h>

class ghostGLManager;

// A flexible OpenGL interface for a basic camera viewing transform.
// The transform can either be set explicitly or determined by
// setting camera properties, such as: position, look at, view angle, etc.
// 
// Note: Changing a property about the camera will automatically cause an
// update on the internally maintained transform matrix. It is advisable to
// disable updates when modifying multiple properties and then force an update
// when you're done.
class ghostGLCameraBase {

public:

    ghostGLCameraBase() {
        m_windowWidth = m_windowHeight = 0;
        m_pGLManager = NULL;
        initDefault();
    }
    
    virtual ~ghostGLCameraBase(){}
    
    // Accessor methods for camera's LookAt
    virtual void setLookAt(
        const gstPoint &point,
        gstBoolean bUpdate = TRUE);
    
    gstPoint getLookAt() const {
        return m_lookAt;
    }

    // Accessor methods for camera's up vector
    virtual void setUpVector(
        const gstVector &upVec,
        gstBoolean bUpdate = TRUE);
    
    gstPoint getUpVector() const {
        return m_upVec;
    }

    // Accessor methods for the camera's near / far Z clipping planes
    virtual void setClippingPlanes(
        const double nearClip,
        const double farClip,
        gstBoolean bUpdate = TRUE);
    
    double getNearClippingPlane() const {
        return m_nearClip;
    }
    
    double getFarClippingPlane() const {
        return m_farClip;
    }
    
    // Accessor methods for the camera's view angle (field of view)
    virtual void setViewAngle(
        const double viewAngle,
        gstBoolean bUpdate = TRUE);
    
    double getViewAngle() const {
        return m_viewAngle;
    }

    // Accessor methods for the camera's position
    virtual void setPosition(
        const gstPoint &position,
        gstBoolean bUpdate = TRUE);
    
    gstPoint getPosition() const;
    
    // Accessor methods for the camera's transform matrix
    virtual void setTransformMatrix(
        gstTransformMatrix &cameraXform,
        gstBoolean bInverseForm = FALSE,
        gstBoolean bUpdate = TRUE);

    gstTransformMatrix getTransformMatrix(gstBoolean bInverseForm = FALSE);

    // Getter methods for the window dimensions
    int getWindowWidth() const {
        return m_windowWidth;   
    }
    
    int getWindowHeight() const {
        return m_windowHeight;
    }
    
    // Accessor methods for the camera's glManager instance
    void setGLManager(ghostGLManager *pGLManager) {
        m_pGLManager = pGLManager;
    }    

    ghostGLManager *getGLManager() const {
        return m_pGLManager;
    }

    virtual void reshape(int width, int height);
    virtual void update();
    virtual void applyTransform();
    virtual void loadCameraTransform();
    
protected:
    
    virtual void initDefault();
    
    static const double kDefaultCameraX; // default X position
    static const double kDefaultCameraY; // default Y position
    static const double kDefaultCameraZ; // default Z position
    
    static const double kDefaultFov;     // default camera field of view
    static const double kDefaultNearClip;// default near Z clipping plane
    static const double kDefaultFarClip; // default far Z clipping plane
    
    ghostGLManager *m_pGLManager;
    
    gstPoint m_lookAt;
    gstVector m_upVec;
    double m_viewAngle;
    double m_nearClip;
    double m_farClip;    
    
    gstTransformMatrix m_transform; // homogeneous transform matrix
    
    // Updated whenever a reshape event occurs
    int m_windowWidth;
    int m_windowHeight;
    
    // These flags are used to enable updates when parameters change
    gstBoolean m_bUpdateTransform;
    gstBoolean m_bReshapeView;
	
    // Allow the ghostGLManager class to directly access camera properties
    friend class ghostGLManager;

};

#endif // GHOSTGL_CAMERA_BASE_H
