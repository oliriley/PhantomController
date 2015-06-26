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

#include "ghostGLPinchXForm.h"

#include <ghostGLManager.h>
#include <ghostGLCameraBase.h>

#include <gstScene.h>

ghostGLCameraBase *ghostGLPinchXForm::getCamera()
{
    if (!getGLManager())
        return(NULL);
    
    return(getGLManager()->getCamera());
}

gstPHANToM *ghostGLPinchXForm::getPhantomNode()
{
    if (!getGLManager())
        return(NULL);
    
    return getGLManager()->getPhantomNode();
}

gstBoolean ghostGLPinchXForm::getPhantomStylusSwitch()
{
    gstPHANToM *phantom = getPhantomNode();
    
    if (!phantom)
        return(FALSE);
    
    return phantom->getStylusSwitch();
}

void ghostGLPinchXForm::preDisplay()
{
    if (!m_bPinchXFormActive && getPhantomStylusSwitch())
        start();
    else if (m_bPinchXFormActive && !getPhantomStylusSwitch())
        stop();
    
    updatePinchXform();
}

void ghostGLPinchXForm::start()
{
    gstTransformMatrix phantomCumulative = getGLManager()->getPhantomCumulativeTransform();
    gstTransformMatrix phantomTransform = getGLManager()->getPhantomTransform();
    
    m_focalPt_WC = phantomCumulative.getTranslation();    
    m_startPhantomPos = phantomTransform.getTranslation();    
    
    m_startCameraXform = getCamera()->getTransformMatrix();    
    
    m_startPhantomRot = phantomTransform;    
    m_startPhantomRot[3][0] = 0.0;
    m_startPhantomRot[3][1] = 0.0;
    m_startPhantomRot[3][2] = 0.0;
    
    m_startCameraRot = m_startCameraXform;
    m_startCameraRot[3][0] = 0.0;
    m_startCameraRot[3][1] = 0.0;
    m_startCameraRot[3][2] = 0.0;
    
    m_invStartCameraRot = m_startCameraRot;
    m_invStartCameraRot.inverse();
    
    m_bPinchXFormActive = TRUE;    
    
    getGLManager()->setTouchableByPHANToM(FALSE);
}

void ghostGLPinchXForm::stop()
{
    m_bPinchXFormActive = FALSE;
    getGLManager()->setTouchableByPHANToM(TRUE);
}

void ghostGLPinchXForm::updatePinchXform()
{
    if (!m_bPinchXFormActive || !getPhantomNode())
        return;	
    
    /*	
    Find delta rotation transform of the Phantom from m_startPhantomRot
    to its current transform.
        
    We will use this delta rotation matrix to rotate camera around the
    focal point by applying this transformation before the current camera
    xforms.  This transformation will take place then in the world reference
    frame.  To rotate around the focal point we will have to use the focal
    point in world reference coorinates as the center of rotation.
          
    But we need the delta rotation transform in the world reference frame.
    In the phantom reference frame the delta is inverse(Mp2)*Mp1 where Mp1
    is the phantom rotation matrix and Mp2 is the current phantom rotation
    matrix.  To get the world reference frame  delta rotation transform we
    must multiply by the starting camera rotation matrix and inverse matrix 
    at the front and back of this delta matrix
    */
    
    gstTransformMatrix ptmMat = getGLManager()->getPhantomTransform();
    gstPoint phantomPos = ptmMat.getTranslation();
    
    ptmMat[3][0] = 0.0;
    ptmMat[3][1] = 0.0;
    ptmMat[3][2] = 0.0;
    gstTransformMatrix invPtmMat = ptmMat;
    invPtmMat.inverse();
    
    gstTransformMatrix temp1, temp2, deltaRot_WC;
    
    // deltaRot_WC =
    //     m_invStartCameraRot * invPtmMat * m_startPhantomRot * m_startCameraRot
    mulM(temp1, m_startPhantomRot, m_startCameraRot);
    mulM(temp2, invPtmMat, temp1);    
    mulM(deltaRot_WC, m_invStartCameraRot, temp2);		
    
    // The orthogonalize calls are needed to prevent skew (or other degradation?)
    // from being introduced into the camera matrix by the calculations.  It is
    // possible that the calculation could be restructured to not need these
    orthogonalize(deltaRot_WC);
    
    // Create to/from translation matrices
    gstTransformMatrix toFocalPtMat, fromFocalPtMat;
    toFocalPtMat.setTranslate(m_focalPt_WC);
    fromFocalPtMat.setTranslate(-m_focalPt_WC);
    
    // newCameraMat =
    //      m_startCameraXform * fromFocalPtMat * deltaRot_WC * toFocalPtMat
    gstTransformMatrix newCameraMat;
    mulM(temp1, deltaRot_WC, toFocalPtMat);
    mulM(temp2, fromFocalPtMat, temp1);		
    mulM(newCameraMat, m_startCameraXform, temp2);
    
    // Now add translation delta vector to camera
    gstVector deltaVec = phantomPos - m_startPhantomPos;
    deltaVec = getPhantomNode()->getParent()->toWorld(deltaVec);
    
    // newCameraMat still has camera at starting position
    // so we subtract off DeltaVec of phantom 
    newCameraMat[3][0] = newCameraMat[3][0] - deltaVec[0];
    newCameraMat[3][1] = newCameraMat[3][1] - deltaVec[1];
    newCameraMat[3][2] = newCameraMat[3][2] - deltaVec[2];
    
    // see above comment about orthogonalize
    orthogonalize(newCameraMat);
    
    // Update the camera xform matrix now    
    getCamera()->setTransformMatrix(newCameraMat);
}

// From Graphics Gems (I) 
// Description page 464, code page 765
void ghostGLPinchXForm::orthogonalize(gstTransformMatrix &R)
{
    static const int kNumCoeff = 10;
    static double coeffArray[kNumCoeff] = {
        1.0,
        -1.0 / 2.0,
        3.0 / 8.0,
        -5.0 / 16.0,
        35.0 / 128.0,
        -63.0 / 256.0,
        231.0 / 1024.0,
        -429.0 / 2048.0,
        6435.0 / 32768.0,
        -12155.0 / 65536.0
    };
    
    gstTransformMatrix I, Temp, Temp2, X, X_power, Sum;
    int i, j;
    
    // transpose: Temp = Rt
    for (i = 0; i <3; i++)
        for (j = 0; j < 3; j++)
            Temp[i][j] = R[j][i];
        
    // X = RtR - I
    mulM(X, Temp, R);
        
    // subtract out identity
    for (i = 0; i < 4; i++) {
        X[i][i] -= 1.0;
    }
        
    for (int power = 1; power < kNumCoeff; power++) {
        mulM(X_power, X, X_power);
        mulC(Temp, coeffArray[power], X_power);
        add(Sum, Sum, Temp);
    }
        
    mulM(Temp, R, Sum);
    R = Temp;
    R[3][3] = 1.0;
}

