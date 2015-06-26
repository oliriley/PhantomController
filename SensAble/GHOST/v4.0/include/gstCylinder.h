//=============================================================================
//   Filename : gstCylinder.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents the geometry of a cylinder.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_CYLINDER
#define GST_CYLINDER

#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstPHANToM.h>
#include <gstShape.h>

// Cylinder primitive.  This class represents the geometry of a cylinder.
// The default position and orientation is centered at the origin with height
// of 2.0 along y-axis and radius of 1.0.
class GHOST_DLL_IMPORT_EXPORT gstCylinder : public gstShape
{
    GHOST_INHERIT_HEADER(gstCylinder, gstShape);

public:

	// Constructor.
	gstCylinder();

	// Constructor.
	gstCylinder(const gstCylinder &);

	// Destructor.
	virtual ~gstCylinder() {}

	// Clone.
	virtual gstNode *Clone() const {return CloneCylinder();}
	gstCylinder *CloneCylinder() const {return (new gstCylinder(*this));}

	enum { NOT_TOUCHED, TOP_TOUCHED, SIDE_TOUCHED, BASE_TOUCHED };

	// Get height of object [millimeters].
	double getHeight() const {return height;}

	// Set height of object [millimeters].
	void setHeight(double newHeight) {
		height = newHeight;
		setBoundingRadius(this, sqrt(radius*radius+height*height*0.25));
		invalidateCumTransf();	
	}

	// Get radius of object [millimeters].
	double getRadius() const {return radius;}

	// Set radius of object [millimeters].
	void setRadius(double newRadius) {
		radius = newRadius;
		setBoundingRadius(this, sqrt(radius*radius+height*height*0.25));
		invalidateCumTransf();	
	}

gstInternal public:
	
	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	//  Returns TRUE if line segment defined by startPt_WC and endPt_WC
	//  intersects this shape object.  If so, intersectionPt_WC is set to
	//  point of intersection and intersectionNormal_WC is set to surface
	//  normal at intersection point.
	virtual gstBoolean	intersection(	const gstPoint &startPt_WC,
										const gstPoint &endPt_WC,
										gstPoint &intersectionPt_WC,
										gstVector &intersectionNormal_WC,
										void **);

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Returns TRUE if PHANToM is currently in contact with this object.
	// If so, the collision is added to the PHANToM's list through
	// gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
	virtual int			collisionDetect(gstPHANToM *PHANToM) ;

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Returns TRUE if pt is inside of this object.
	virtual int			checkIfPointIsInside_WC(const gstPoint &pt) ;

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Saves to file.
    void				printSelf(FILE *);

private:

	double			radius;
	double			height;
	double			a,b,h;
    gstVector		P1;     // current contact Point
    gstVector		N;      // current contact Normal
    gstVector		Nnew;   // propossed new contact normal
    gstPlane		T;      // current contact tangent Plane
    gstVector		D;        // current penetration dist
	int				been_outside; // outside flag

	// Private Methods
    double			S(const gstPoint &);          // surface function
    gstVector		grad_S(gstPoint &);    // surface gradient
    double			dist_to_T(gstPoint &);  // tangent plane surface function
    gstPoint		get_on_S(gstPoint &);  // find closed point on surface
    gstPoint		get_on_T(gstPoint &);  // find closest pt on tangent plane
};

#endif
