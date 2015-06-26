//-----------------------------------------------------------------
//
// Copyright 1998, SensAble Technologies, Inc.
//
// File: HapticFrustum.h
//
// Author: Josh Handley
// Modified by: Brandon Itkowitz
//
// Created: 12/13/98
// Modified: 08/04/99
//
//-----------------------------------------------------------------

#ifndef _HAPTIC_FRUSTUM_H_
#define _HAPTIC_FRUSTUM_H_

#ifdef _WIN32
#pragma warning( disable: 4251 ) // STL gstPolyPropsContainer warning
#pragma warning( disable: 4244 ) // truncating double to float
#pragma warning( disable: 4305 ) // truncation from 'const double' to 'const float'
#endif

// Haptic representation of view frustum.  A GHOST poly mesh that
// can be created given params of view frustum.  Can set callback when you
// pop through the frustum so you can give phantom back to mouse driver.

class HapticFrustum : public gstTriPolyMeshHaptic {

public :
    static const gstEventType TOUCHED;
    static const gstEventType UNTOUCHED;
    static const gstEventType POPPED_THROUGH;
    
    // constructor
    HapticFrustum();
    
    // destructor
    ~HapticFrustum();
    
    // set ortho view
    void setOrthoView(gstPoint viewer, float left, float right, 
        float top, float bottom, float nearp, float farp,
        float nearOffset, float farOffset, float border);
    
    // set perspective view
    void setPerspectiveView(gstPoint viewer, float viewAngle, float aspectRatio,
        float nearp, float farp, float nearOffset, float farOffset, float border);
    
    // set pop-through threshold
    inline void setThreshold(float t) {m_nThreshold = t;}
    
    // collisionDetect
    virtual gstBoolean collisionDetect(gstPHANToM *phantomNode);
    
    // Intersection
    virtual gstBoolean  intersection(const gstPoint &startPt_WC, const gstPoint &endPt_WC, 
                                     gstPoint &intersectionPt_WC, gstVector &intersectionNormal_WC,
                                     void **data);
    
    static const float DEFAULT_THRESHOLD;
    
protected:
    
    void createMesh(float nearLeft, float nearRight, float nearTop, float nearBottom,
        float farLeft, float farRight, float farTop, float farBottom, float nearp, float farp);
    
    void updateMesh(float nearLeft, float nearRight, float nearTop, float nearBottom,
        float farLeft, float farRight, float farTop, float farBottom, float nearp, float farp);
    
private:
    
    enum Triangle {
        HF_TOP1 = 0,
            HF_TOP2,
            HF_LEFT1,
            HF_LEFT2,
            HF_RIGHT1,
            HF_RIGHT2,
            HF_BOTTOM1,
            HF_BOTTOM2,
            HF_FRONT1,
            HF_FRONT2,
            HF_BACK1,
            HF_BACK2
    };
    
    
    bool isSidePoly(Triangle t) const { return t <= HF_FRONT2; }
    
    gstBoolean m_bPopped;
    gstTriPolyMesh *m_pMesh;
    float m_nThreshold;        
};

#endif
