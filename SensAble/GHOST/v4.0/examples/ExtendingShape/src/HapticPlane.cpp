//*****************************************************************************
//      Filename : HapticPlane.cpp
// ----------------------------------------------------------------------------
// Infinite touchable plane derived from gstShape.
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include "HapticPlane.h"

gstType HapticPlane::HapticPlaneClassTypeId;

// Intersect line segment with shape
gstBoolean HapticPlane::intersection(const gstPoint &startPt_WC,
    const gstPoint &endPt_WC,
    gstPoint &intersectionPt_WC,
    gstVector &intersectionNormal_WC,
    void **userData)
{
    gstPoint startPt = fromWorld(startPt_WC);
    gstPoint endPt = fromWorld(endPt_WC);

    // if startPt.z() * endPt.z() > 0 then they have same sign and are on same side of plane
    // also throw out case where both are in plane (don't need it)
    if ((startPt.z() * endPt.z() > 0.0) || (startPt.z() == endPt.z()))      
      return false;
      
    // points are on opposite sides of plane find intersection
    double t = -startPt.z()/(endPt.z() - startPt.z());
    gstPoint intersectionPt;
    intersectionPt[0] = startPt.x() + t * (endPt.x() - startPt.x());
    intersectionPt[1] = startPt.y() + t * (endPt.y() - startPt.y());
    intersectionPt[2] = 0.0;
    
    // pick normal based on which side of plane P1 is
    gstVector intersectionNormal;
    if (startPt.z() > 0.0)
        intersectionNormal = gstVector(0.0,0.0,1.0);
    else 
        intersectionNormal = gstVector(0.0,0.0,-1.0);

    // Convert back to world coords
    intersectionPt_WC = toWorld(intersectionPt);
    intersectionNormal_WC = toWorld(intersectionNormal);

    return true;
}

// Dtermine new SCP for shape
gstBoolean HapticPlane::collisionDetect(gstPHANToM *phantom)
{
    if(!isTouchableByPHANToM() || _resetPHANToMContacts) {
      _resetPHANToMContacts = FALSE;
      (void) updateStateForPHANToM(phantom, FALSE);
      return FALSE;
    }

    //Convert phantom data to local coordinate system
    gstPoint lastSCP, phantomPos, phantomPos_WC;
    phantom->getLastSCP_WC(lastSCP);
    lastSCP = fromWorld(lastSCP);
    phantom->getPosition_WC(phantomPos_WC);
    phantomPos = fromWorld(phantomPos_WC);

    // if on same side of support plane or both on support plane 
    // there is no collision so return false
    if ((lastSCP.z() * phantomPos.z() > 0) || (lastSCP.z() == phantomPos.z())) {
        (void) updateStateForPHANToM(phantom, false);
        return false;
    }
    
    // are on opposite sides of support plane, find SCP
    // which is projection of phantomPos onto plane
    gstPoint SCP = gstPoint(phantomPos.x(), phantomPos.y(), 0.0);

    gstVector SCPNormal;
    // pick normal based on which side of plane lastSCP is on
    if (lastSCP.z() > 0.0)
        SCPNormal = gstVector(0.0,0.0,1.0);
    else 
        SCPNormal = gstVector(0.0,0.0,-1.0);

    (void) updateStateForPHANToM(phantom, true);
    addCollision(phantom, SCP, SCPNormal);

    return true;
}
