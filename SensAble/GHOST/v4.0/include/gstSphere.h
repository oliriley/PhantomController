//=============================================================================
//   Filename : gstSphere.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents the geometry of sphere.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SPHERE
#define GST_SPHERE

#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstPHANToM.h>
#include <gstShape.h>

// Sphere primitive.  This class represents the geometry of a sphere.
class GHOST_DLL_IMPORT_EXPORT gstSphere : public gstShape
{
    GHOST_INHERIT_HEADER(gstSphere, gstShape);

public:

	// Constructor.
	gstSphere();

	// Constructor.
	gstSphere(const gstSphere &);

	// Destructor.
	virtual ~gstSphere() {}

	// Clone.
	virtual gstNode *Clone() const {return CloneSphere();}
	gstSphere *CloneSphere() const {return (new gstSphere(*this));}

	// Get radius of object [millimeters].
	double getRadius() const {return radius;}

	// Set radius of object [millimeters].
	void setRadius(double newRadius) {
		radius = newRadius;
		setBoundingRadius(this, radius);
		invalidateCumTransf();
	}

gstInternal public:

	// FOR_GHOST_EXTENSION
	// Used by system or for creating sub-classes only.
	// Returns TRUE if line segment defined by startPt_WC and endPt_WC
	// intersects this shape object.  If so, intersectionPt_WC is set to
	// point of intersection and intersectionNormal_WC is set to surface
	// normal at intersection point.
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
	virtual int	collisionDetect(gstPHANToM *PHANToM) ;

	// FOR_GHOST_INTERNAL
	// Used by system or for creating sub-classes only.
	// Returns TRUE if pt is inside of this object.
	virtual int	checkIfPointIsInside_WC(const gstPoint &pt) ;

	// FOR_GHOST_INTERNAL
	// Used by system or for creating sub-classes only.
	// Saves to file.
    void printSelf(FILE *);

private:

	double			radius;
    gstPlane		T;    // current contact tangent Plane
    int				been_outside; // outside flag

    double			S(const gstPoint &); // surface function
    gstVector		grad_S(gstPoint &); // surface gradient
    double			dist_to_T(gstPoint &); // tangent plane surface function
    gstPoint		get_on_S(gstPoint &);  // find closed point on surface
    gstPoint		get_on_T(gstPoint &);  // find closest pt on tangent plane
};

#endif
