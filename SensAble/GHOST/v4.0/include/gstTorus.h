//=============================================================================
//   Filename : gstTorus.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents the geometry of torus.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TORUS
#define GST_TORUS

#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstPHANToM.h>
#include <gstShape.h>

// Torus primitive.  This represents the geometry of a torus.
// The default position and orientation is centered about the
// origin with the ring radius (Major Radius) set to 2/3 and the
// swept circle radius (Minor Radius) set to 1/3.  The y-axis
// passes through the hole of the Torus.
class GHOST_DLL_IMPORT_EXPORT gstTorus : public gstShape
{
    GHOST_INHERIT_HEADER(gstTorus, gstShape);

public:


	// Constructor.
	gstTorus();

	// Constructor.
	gstTorus(const gstTorus &);

	// Destructor.
	virtual ~gstTorus() {}

	// Clone.
	virtual gstNode *Clone() const {return CloneTorus();}
	gstTorus *CloneTorus() const {return (new gstTorus(*this));}

	// Get minor radius [millimeters].
	double getMinorRadius() const {return minorRadius;}

	// Set minor radius [millimeters].
	void setMinorRadius(double newRadius) {
		minorRadius = newRadius;
		setBoundingRadius(this, minorRadius+majorRadius);
		invalidateCumTransf();
	}

	// Get major radius [millimeters].
	double getMajorRadius() const {return majorRadius;}

	// Set major radius [millimeters].
	void setMajorRadius(double newRadius) {
		majorRadius = newRadius;
		setBoundingRadius(this, minorRadius+majorRadius);
		invalidateCumTransf();
	}

gstInternal public:

	// FOR_GHOST_EXTENSION
	//  Returns TRUE if line segment defined by startPt_WC and endPt_WC
	//  intersects this shape object.  If so, intersectionPt_WC is set to
	//  point of intersection and intersectionNormal_WC is set to surface
	//  normal at intersection point.
	virtual gstBoolean	intersection(	const gstPoint &startPt_WC,
										const gstPoint &endPt_WC,
										gstPoint &intersectionPt_WC,
										gstVector &intersectionNormal_WC,
										void **);

	// FOR_GHOST_EXTENSION
	// Used by system or for creating sub-classes only.
	// Returns TRUE if PHANToM is currently in contact with this object.
	// If so, the collision is added to the PHANToM's list through
	// gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
	virtual int collisionDetect(gstPHANToM *PHANToM) ;

	// FOR_GHOST_INTERNAL
	// Used by system or for creating sub-classes only.
	// Returns TRUE if pt is inside of this object.
	virtual int checkIfPointIsInside_WC(const gstPoint &pt) ;

	// FOR_GHOST_INTERNAL
	// Used by system or for creating sub-classes only.
	// Saves to file.
    void printSelf(FILE *);

private:

    double			majorRadius;
	double			minorRadius;
    gstPlane		T;    // current contact tangent Plane
    int				been_outside; // outside flag

    double			S(const gstPoint &); // surface function
    gstVector		grad_S(gstPoint &); // surface gradient
    double			dist_to_T(gstPoint &); // tangent plane surface function
    gstPoint		get_on_S(gstPoint &);  // find closed point on surface
    gstPoint		get_on_T(gstPoint &);  // find closest pt on tangent plane
};

#endif
