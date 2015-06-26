//*****************************************************************************
//      Filename : HapticPlane.h
// ----------------------------------------------------------------------------
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef HAPTIC_PLANE_INCLUDE
#define HAPTIC_PLANE_INCLUDE

#include <gstShape.h>

// Infinite touchable plane derived from gstShape.
class  HapticPlane : public gstShape 
{
public:

	// Constructor.
	HapticPlane()
    {
        // infinite in x and y directions and small in z
        boundByBox(gstPoint(0,0,0), DBL_MAX, DBL_MAX, 2);
    }

    // Get type of this class.  No instance needed.
	static gstType getClassTypeId()
	{
		gstAssignUniqueId(HapticPlaneClassTypeId);
		return HapticPlaneClassTypeId;
	}

	// Get type of this instance.
	virtual gstType		getTypeId() const { return getClassTypeId(); }

	// Returns TRUE if this class is same or derived class of type.
	virtual gstBoolean isOfType(gstType type) const {return staticIsOfType(type);}

	// Returns TRUE if subclass is of type.
	static gstBoolean staticIsOfType(gstType type)
	{
		if (type == getClassTypeId()) return TRUE;
		else return (gstShape::staticIsOfType(type));
	}

    // Intersect line segment with shape
    virtual gstBoolean intersection(const gstPoint &startPt_WC,
        const gstPoint &endPt_WC,
        gstPoint &intersectionPt_WC,
        gstVector &intersectionNormal_WC,
        void **userData);

    // Dtermine new SCP for shape
    virtual gstBoolean collisionDetect(gstPHANToM *phantom);


private:

    static gstType			HapticPlaneClassTypeId;

};

#endif // HAPTIC_PLANE_INCLUDE
