//=============================================================================
//   Filename : gstBoundingSphere.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: base of gstTransformedBoundingSphere (a type of gstBoundingVolume)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTBOUNDINGSPHERE_INCLUDED)
#define GSTBOUNDINGSPHERE_INCLUDED

#include <gstBasic.h>
#include <gstRay.h>
#include <gstLineSegment.h>
#include <gstObjectIntersectionInfo.h>
#include <gstLineIntersectionInfoFirst_Param.h>
#include <float.h>


class gstTriPoly;

//
// Represents a bounding sphere.
//
class GHOST_DLL_IMPORT_EXPORT gstBoundingSphere
{
  public:

	gstBoundingSphere(
		const gstPoint &_center,
		const double _radius);

	virtual ~gstBoundingSphere();

	//
	// Returns the position of the center of gstBoundingSphere
	//
	inline gstPoint getCenter() const;

	//
	// Returns a gstBoundingSphere with DBL_MAX radius.
	//
	static gstBoundingSphere getLargestBoundingSphere();

	//
	// Returns the radius of gstBoundingSphere
	//
	inline double getRadius() const;

	//
	// Tests if pt is inside gstSimpleSphere.  If |sphereCenter-pt| <= sphereRadius,
	// then TRUE is returned. Otherwise, FALSE is returned.
	//
	gstBoolean inside(
		const gstPoint &pt) const;

	//
	// Sets the position of the center of gstBoundingSphere to _center.
	//
	void setCenter(
		const gstPoint &_center);

	//
	// Sets the radius of gstBoundingSphere to _radius.
	//
	void setRadius(
		const double _radius);

    //
	// This method will return TRUE if the line defined by the input points
	// intersects the sphere.
    //
	gstBoolean intersectP(const gstPoint &start_pt, const gstPoint &end_pt);

  private:

	gstPoint center;

	double radius;
};



inline gstPoint gstBoundingSphere::getCenter() const
{
	return(center);
}


inline double gstBoundingSphere::getRadius() const
{
	return(radius);
}


#endif  // GSTBOUNDINGSPHERE_INCLUDED
