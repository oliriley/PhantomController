//-----------------------------------------------------------------
//
// Copyright 1998, SensAble Technologies, Inc.
//
// File: HapticFrustum.cpp
//
// Author: Josh Handley
// Modified by: Brandon Itkowitz (bitkowitz@sensable.com)
//
// Created: 12/13/98
// Modified: 08/04/99 
//
//-----------------------------------------------------------------

#include "gstAfx.h"
#include "HapticFrustum.h"

// Haptic representation of view frustum.  A GHOST poly mesh that
// can be created given params of view frustum.  Can set callback when you
// pop through the frustum so you can give phantom back to mouse driver.

const float HapticFrustum::DEFAULT_THRESHOLD = 0.2;

const gstEventType HapticFrustum::TOUCHED = gstNextUniqueId();
const gstEventType HapticFrustum::UNTOUCHED = gstNextUniqueId();
const gstEventType HapticFrustum::POPPED_THROUGH = gstNextUniqueId();

// constructor
HapticFrustum::HapticFrustum() : gstTriPolyMeshHaptic()
{
    m_bPopped = FALSE;
    m_nThreshold = DEFAULT_THRESHOLD;
    m_pMesh = NULL;
}

// destructor
HapticFrustum::~HapticFrustum()
{
    if (m_pMesh)
        delete m_pMesh;
}

// collisionDetect
gstBoolean HapticFrustum::collisionDetect(gstPHANToM *phantomNode)
{
    if (!m_pMesh) {
        return FALSE;
    }
    
    gstBoolean hadCollision = gstTriPolyMeshHaptic::collisionDetect(phantomNode);
    
    if (!hadCollision) {
        m_bPopped = FALSE;
        return FALSE;
    }
        
    if (m_bPopped) {
        return TRUE;
    }

    gstVector force = phantomNode->getReactionForce_WC();
    if (force.norm() > m_nThreshold) {
        
        // Had contact over threshold - was this with a side polygon or front/back
        bool isHitSide = false;
        for (vector<gstTriPoly*>::const_iterator i = getTouchedPolys().begin();
        i != getTouchedPolys().end(); ++i)
        {
            assert((*i)->getKey() >= HF_TOP1 && (*i)->getKey() <= HF_BACK2);
            isHitSide = isHitSide || isSidePoly(Triangle((*i)->getKey()));
        }
        
        if (isHitSide)
        {
            m_bPopped = TRUE;
            if (eventCallback != NULL) 
            {
                gstEvent event;
                event.id = POPPED_THROUGH;
                (void) addEvent(event);
            }
        }
    }
    
    return TRUE;
}

// Intersection
gstBoolean HapticFrustum::intersection(const gstPoint &startPt_WC, const gstPoint &endPt_WC, 
                                       gstPoint &intersectionPt_WC, gstVector &intersectionNormal_WC,
                                       void **data)
{
    if (!m_pMesh)
        return FALSE;

    return gstTriPolyMeshHaptic::intersection(startPt_WC, endPt_WC, 
                                              intersectionPt_WC, intersectionNormal_WC, data);
}

// set ortho view
void HapticFrustum::setOrthoView(gstPoint viewer, float left, float right, 
                                 float top, float bottom, float nearp, float farp,
                                 float nearOffset, float farOffset, float border)
{
    float nearLeft, nearRight, nearTop, nearBottom, farLeft, farRight, farTop, farBottom;

    // generate near and far plane coords based on the dimensions of the orthogonal view,
    // the location of the viewer and the touch border
    nearLeft = farLeft = left + viewer.x() + border;
    nearRight = farRight = right + viewer.x() - border;
    nearBottom = farBottom = bottom + viewer.y() + border;
    nearTop = farTop = top + viewer.y() - border;

    if (m_pMesh == NULL) {
        createMesh(nearLeft, nearRight, nearTop, nearBottom, 
                   farLeft, farRight, farTop, farBottom,
                   viewer.z() - nearp - nearOffset,
                   viewer.z() - farp + farOffset);
    } else {
        updateMesh(nearLeft, nearRight, nearTop, nearBottom, 
                   farLeft, farRight, farTop, farBottom,
                   viewer.z() - nearp - nearOffset,
                   viewer.z() - farp + farOffset);
    }
}

// set perspective view
void HapticFrustum::setPerspectiveView(gstPoint viewer, float viewAngle, float aspectRatio,
                                       float nearp, float farp, float nearOffset, float farOffset, float border)
{
    float nearLeft, nearRight, nearTop, nearBottom, farLeft, farRight, farTop, farBottom;
    
    float nearHeight = tan((viewAngle / 2.0) * M_PI / 180.0) * nearp;
    float nearWidth = aspectRatio * nearHeight;
    float farHeight = tan((viewAngle / 2.0) * M_PI / 180.0) * farp;
    float farWidth = aspectRatio * farHeight;
    
    nearLeft = -nearWidth + viewer.x() + border;
    nearRight = nearWidth + viewer.x() - border;
    nearBottom = -nearHeight + viewer.y() + border;
    nearTop = nearHeight + viewer.y() - border;

    farLeft = -farWidth + viewer.x() + border;
    farRight = farWidth + viewer.x() - border;
    farBottom = -farHeight + viewer.y() + border;
    farTop = farHeight + viewer.y() - border;
    
    if (m_pMesh == NULL) {
        createMesh(nearLeft, nearRight, nearTop, nearBottom, 
                   farLeft, farRight, farTop, farBottom,
                   viewer.z() - nearp - nearOffset, viewer.z() - farp + farOffset);
    } else {
        updateMesh(nearLeft, nearRight, nearTop, nearBottom, 
                   farLeft, farRight, farTop, farBottom,
                   viewer.z() - nearp - nearOffset, viewer.z() - farp + farOffset);
    } 
}

void HapticFrustum::createMesh(float nearLeft, float nearRight, float nearTop, float nearBottom,
        float farLeft, float farRight, float farTop, float farBottom, float nearp, float farp)
{
    gstTriPolyMesh *mesh = new gstTriPolyMesh();
    
    mesh->createVertex(gstPoint(farRight, farTop, farp),0);
    mesh->createVertex(gstPoint(farLeft, farTop, farp),1);
    mesh->createVertex(gstPoint(farLeft, farBottom, farp),2);
    mesh->createVertex(gstPoint(farRight, farBottom, farp),3);
    
    mesh->createVertex(gstPoint(nearRight, nearTop, nearp),4);
    mesh->createVertex(gstPoint(nearLeft, nearTop, nearp),5);
    mesh->createVertex(gstPoint(nearLeft, nearBottom, nearp),6);
    mesh->createVertex(gstPoint(nearRight, nearBottom, nearp),7);
    
    
    // Top
    mesh->createTriPoly(5, 1, 0, HF_TOP1);
    mesh->createTriPoly(5, 0, 4, HF_TOP2);
    
    // Left
    mesh->createTriPoly(1, 5, 2, HF_LEFT1);
    mesh->createTriPoly(5, 6, 2, HF_LEFT2);
    
    // Right
    mesh->createTriPoly(7, 4, 3, HF_RIGHT1);
    mesh->createTriPoly(3, 4, 0, HF_RIGHT2);
    
    // Bottom
    mesh->createTriPoly(6, 3, 2, HF_BOTTOM1);
    mesh->createTriPoly(6, 7, 3, HF_BOTTOM2);
    
    // Front
    mesh->createTriPoly(6, 5, 4, HF_FRONT1);
    mesh->createTriPoly(6, 4, 7, HF_FRONT2);
    
    // Back
    mesh->createTriPoly(3, 0, 2, HF_BACK1);
    mesh->createTriPoly(2, 0, 1, HF_BACK2);
    
    m_pMesh = mesh;
    setTriPolyMesh(m_pMesh);
}

void HapticFrustum::updateMesh(float nearLeft, float nearRight, float nearTop, float nearBottom,
        float farLeft, float farRight, float farTop, float farBottom, float nearp, float farp)
{
    assert(m_pMesh);
    // set begin modify on each of the faces
    for (gstTriPolyPtrHashMapConstIterator iter = m_pMesh->polygonsBegin();
         iter != m_pMesh->polygonsEnd();
         ++iter) {

        gstTriPoly *p = (*iter).second;
        p->beginModify();
    }

    // change vertex values
    gstVertex *v;

    v = m_pMesh->getVertex(0);
    v->init(farRight, farTop, farp);
    v = m_pMesh->getVertex(1);
    v->init(farLeft, farTop, farp);
    v = m_pMesh->getVertex(2);
    v->init(farLeft, farBottom, farp);
    v = m_pMesh->getVertex(3);
    v->init(farRight, farBottom, farp);

    v = m_pMesh->getVertex(4);
    v->init(nearRight, nearTop, nearp);
    v = m_pMesh->getVertex(5);
    v->init(nearLeft, nearTop, nearp);
    v = m_pMesh->getVertex(6);
    v->init(nearLeft, nearBottom, nearp);
    v = m_pMesh->getVertex(7);
    v->init(nearRight, nearBottom, nearp);

    // set end modify on each of the faces
    for (iter = m_pMesh->polygonsBegin();iter != m_pMesh->polygonsEnd() ;++iter) {
        gstTriPoly *p = (*iter).second;
        p->endModify();
    }
}
