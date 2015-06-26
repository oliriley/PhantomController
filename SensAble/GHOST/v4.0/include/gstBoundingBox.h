//=============================================================================
//   Filename : gstBoundingBox.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: base of gstTransformedBoundingBox (a type of gstBoundingVolume)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include <gstDllExport.h>
#include <float.h>

#include <vector.h>

#include <gstBoundingSphere.h>
#include <gstObjectIntersectionInfo.h>
#include <gstLineIntersectionInfoFirst_Param.h>
#include <gstLineIntersectionInfoFirstTwo_Param.h>
#include <gstLineSegment.h>
#include <gstRay.h>

#ifndef GSTBOUNDINGBOX_INCLUDED
#define GSTBOUNDINGBOX_INCLUDED

class gstTriPoly;

//
//
// Represents a box aligned with the x, y, and z axis which 
// contains a volume of space.
//
class GHOST_DLL_IMPORT_EXPORT gstBoundingBox
{
  public:

	enum returnValues {RV_LEFT, RV_MIDDLE, RV_RIGHT};


	gstBoundingBox(
		const gstPoint &blb,					// Back Left Bottom corner
		const gstPoint &frt);					// Front Right Top corner


	gstBoundingBox();

	// Ctor which takes a collection of points and calculates the bounding box.
	gstBoundingBox(
		vector<gstPoint> &v);

	virtual ~gstBoundingBox();

	//
	// Returns the position of the center of gstBoundingBox
	//
	gstPoint center();

	//
	// Returns gstSimpleSphere centered at center of gstBoundingBox and with radius such
	// that gstBoundingBox is just enclosed within the sphere.
	//
	gstBoundingSphere getBoundingSphere();

	//
	// Returns a gstBoundingBox with DBL_MAX side lengths.
	//
	static gstBoundingBox getLargestBoundingBox();

	//
	// Returns new gstBoundingBox set to Left Lower Back quadrant of 'this' gstBoundingBox.
	//
	gstBoundingBox getLLBBox();

    //
	// Returns new gstBoundingBox set to Left Lower Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingBox getLLFBox();

	//
	// Returns new gstBoundingBox set to Left Upper Back quadrant of 'this' gstBoundingBox.
	//
	gstBoundingBox getLUBBox();

	//
	// Returns new gstBoundingBox set to Left Upper Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingBox getLUFBox();

	//
	// Returns new gstBoundingBox set to Right Lower Back quadrant of 'this'
	// gstBoundingBox.
    //
	gstBoundingBox getRLBBox();

	//
	// Returns new gstBoundingBox set to Right Lower Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingBox getRLFBox();

	//
	// Returns new gstBoundingBox set to Right Upper Back quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingBox getRUBBox();

	//
	// Returns new gstBoundingBox set to Right Upper Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingBox getRUFBox();

	//
	// Tests if pt is inside of box.  If so, TRUE is returned.  Otherwise, FALSE is
	// returned.
	//
	gstBoolean inside(
		gstPoint &pt);

	//
	// Intersects gstRay with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
	// specifies an intersection into the object, 'out' specifies an intersection
	// leaving the object, 'inOut' specifies intersections entering and leaving the
	// object, and 'none' specifies that the object is not intersected.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		gstRay &ray,
		gstLineIntersectionInfoFirstTwo_Param &intersectionInfo);

	//
	// Intersects gstLineSegment with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
	// none_outside. 'in' specifies an intersection into the object, 'out' specifies an
	// intersection leaving the object, 'inOut' specifies intersections entering and
	// leaving the object, 'none_insdie' specifies that the object is not intersected
	// and the line segment is within the object, and 'none_outside' specifies that
	// there is no intersection and the line segment is outside of the object.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		gstLineSegment &lineSegment,
		gstLineIntersectionInfoFirstTwo_Param &intersectionInfo);

	//
	// Intersects gstRay with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
	// specifies an intersection into the object, 'out' specifies an intersection
	// leaving the object, 'inOut' specifies intersections entering and leaving the
	// object, and 'none' specifies that the object is not intersected.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		gstRay &ray,
		gstLineIntersectionInfoFirst_Param &intersectionInfo);

	//
	// Intersects gstLineSegment with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
	// none_outside. 'in' specifies an intersection into the object, 'out' specifies an
	// intersection leaving the object, 'inOut' specifies intersections entering and
	// leaving the object, 'none_insdie' specifies that the object is not intersected
	// and the line segment is within the object, and 'none_outside' specifies that
	// there is no intersection and the line segment is outside of the object.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		gstLineSegment &lineSegment,
		gstLineIntersectionInfoFirst_Param &intersectionInfo);

	//
	// Returns gstPoint coincident with the left lower back corner of this gstBoundingBox.
	//
	gstPoint leftLowerBackCorner() const;

	//
	// Returns gstPoint coincident with the left lower front corner of this gstBoundingBox.
	//
	gstPoint leftLowerFrontCorner();

	//
	// Returns gstPoint coincident with the left upper back corner of this gstBoundingBox.
	//
	gstPoint leftUpperBackCorner();

	//
	// Returns gstPoint coincident with the left upper front corner of this gstBoundingBox.
	//
	gstPoint leftUpperFrontCorner();

	//
	// Returns gstPoint coincident with the right lower back corner of this gstBoundingBox.
	//
	gstPoint rightLowerBackCorner();

	//
	// Returns gstPoint coincident with the right lower front corner of this gstBoundingBox.
	//
	gstPoint rightLowerFrontCorner();

	//
	// Returns gstPoint coincident with the right upper back corner of this gstBoundingBox.
	//
	gstPoint rightUpperBackCorner();

	//
	// Returns gstPoint coincident with the right upper front corner of this gstBoundingBox.
	//
	gstPoint rightUpperFrontCorner() const;
	
	//
	// Sets maximum position (right upper front corner) to newMaxPt
	//
	void setMaxPt(gstPoint newMaxPt){
		maxPt = newMaxPt;
	}

	//
	// Sets minimum position (left lower back corner) to newMinPt
	//
	void setMinPt(gstPoint newMinPt){
		minPt = newMinPt;
	}

	//
	// Returns maximum position (right upper front corner).
	//
	gstPoint getMaxPt() const {
		return maxPt;
	}

	//
	// Returns minimum position (left lower back corner).
	//
	gstPoint getMinPt() const {
		return minPt;
	}

  private:

	gstPoint maxPt;

	gstPoint minPt;
};

#endif  // GSTBOUNDINGBOX_INCLUDED
