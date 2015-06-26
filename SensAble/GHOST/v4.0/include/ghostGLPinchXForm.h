//=============================================================================
//   Filename : ghostGLPinchXForm.h
// Written by : Chris Tarr (ctarr@sensable.com)
//              Philip Winston (pwinston@sensable.com)
//    Project : GhostGL 1.0
//     Module : GhostGL Action Object : Pinch Transform
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GHOSTGL_PINCH_XFORM_H
#define GHOSTGL_PINCH_XFORM_H

#include <gstBasic.h>
#include <gstPoint.h>
#include <gstVector.h>
#include <gstTransformMatrix.h>
#include <gstPHANToM.h>

#include <ghostGLActionObject.h>

class ghostGLCameraBase;

// Allow the PHANToM position/orientation to change the camera view.  The effect
// is as if clicking the stylus button grabbed/pinched to scene and allowed you
// to manipulate these scene with the PHANToM. But note that the camera is 
// actually moving, the scene is not.
class ghostGLPinchXForm: public ghostGLActionObject
{
public:
    
    // constructor
    ghostGLPinchXForm() {
        m_bPinchXFormActive = FALSE;
    }

    // stop us before we get removed. Remove is called when
    // ghostGLManager::removeActionObject() is called on this object
    virtual void remove() { stop(); }

    virtual ~ghostGLPinchXForm(){}
    
    // checks if we should start or stop
    virtual void preDisplay();
    
protected:

    // enable pinch mode
    void start();
    // end pinch mode
    void stop();
    // call each graphics update while pinch is enabled
    void updatePinchXform();
        
private:

    // fix up matrix
    void orthogonalize(gstTransformMatrix &R);

    // add two matrices
    inline void add(gstTransformMatrix &result,
        gstTransformMatrix &a,
        gstTransformMatrix &b);

    // multiply matrix by a constant
    inline void mulC(gstTransformMatrix &result,
        double C,
        gstTransformMatrix &a);

    
    ghostGLCameraBase *getCamera();
    gstPHANToM *getPhantomNode();
    gstBoolean getPhantomStylusSwitch();
    
    gstBoolean m_bPinchXFormActive;
    
    gstTransformMatrix m_startCameraXform;
    gstTransformMatrix m_startCameraRot;
    gstTransformMatrix m_invStartCameraRot;
    
    gstPoint m_cameraOffset;
    
    gstTransformMatrix m_startPhantomRot;
    gstPoint m_startPhantomPos;
    
    gstPoint m_focalPt_WC;
    
};

inline void ghostGLPinchXForm::add(gstTransformMatrix &result,
                                   gstTransformMatrix &a,
                                   gstTransformMatrix &b)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

inline void ghostGLPinchXForm::mulC(gstTransformMatrix &result,
                                    double C,
                                    gstTransformMatrix &a)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = C * a[i][j];
        }
    }
}

#endif // GHOSTGL_PINCH_XFORM

