//=============================================================================
//   Filename : gstBoundingCube.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: bounding object used by gstSpatialObject
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTBOUNDINGCUBE_INCLUDED)
#define GSTBOUNDINGCUBE_INCLUDED

#include <gstDllExport.h>
#include <gstObjectIntersectionInfo.h>
#include <gstLineSegment.h>
#include <gstRay.h>
#include <gstLineIntersectionInfoFirst_Param.h>
#include <gstLineIntersectionInfoFirstTwo_Param.h>
#include <gstBoundingSphere.h>
#include <gstBoundingBox.h>

//
//
// Represents a cube aligned with the x, y, and z axis 
// which contains a volume of space.
//
class GHOST_DLL_IMPORT_EXPORT gstBoundingCube
{
  public:

	enum returnValues {RV_LEFT, RV_MIDDLE, RV_RIGHT};

	//
	//
	// This constructor creates valid gstCube for minPt and maxPt
	// of a valid cube.  Otherwise the resulting gstBoundingCube
	// will be of undefined side length.
	//
	gstBoundingCube(
		const gstPoint &minPt,
		const gstPoint &maxPt);

	gstBoundingCube(
		const gstPoint &_center,
		const double _sideLength);

	virtual ~gstBoundingCube();

	//
	// Returns gstPoint coincident with the center of the back side
	// of this cube.
	//
	inline gstPoint backCenter() const;

	//
	// Returns gstPoint coincident with the center of the bottom side
	// of this cube.
	//
	inline gstPoint bottomCenter() const;

	//
	// Returns gstPoint coincident with the center of the front side
	// of this cube.
	//
	inline gstPoint frontCenter() const;

	//
	// Returns gstBoundingSphere centered at center of gstBoundingBox and with radius such
	// that gstBoundingBox is just enclosed within the sphere.
	//
	gstBoundingSphere getBoundingSphere() const;

	//
	// Returns the position of the center of gstBoundingBox
	//
	inline gstPoint getCenter() const;

	//
	// Returns new gstBoundingBox set to Left Lower Back quadrant of 'this' gstBoundingBox.
	//
	gstBoundingCube getLLBCube() const;

	//
	// Returns new gstBoundingBox set to Left Lower Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getLLFCube() const;

	//
	// Returns new gstBoundingBox set to Left Upper Back quadrant of 'this' gstBoundingBox.
	//
	gstBoundingCube getLUBCube() const;

	//
	// Returns new gstBoundingBox set to Left Upper Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getLUFCube() const;

	//
	// Returns maximum position (right upper front corner).
	//
	gstPoint getMaxPoint() const;

	//
	// Returns minimum position (left lower back corner).
	//
	gstPoint getMinPoint() const;

	//
	// Returns new gstBoundingBox set to Right Lower Back quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getRLBCube() const;

	//
	// Returns new gstBoundingBox set to Right Lower Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getRLFCube() const;

	//
	// Returns new gstBoundingBox set to Right Upper Back quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getRUBCube() const;

	//
	// Returns new gstBoundingBox set to Right Upper Front quadrant of 'this'
	// gstBoundingBox.
	//
	gstBoundingCube getRUFCube() const;

	//
	// Returns side length of cube.
	//
	inline double getSideLength() const;

	//
	// Tests if pt is inside of box.  If so, TRUE is returned.  Otherwise, FALSE is
	// returned.
	//
	gstBoolean inside(
		const gstPoint &pt) const;

	//
	// Intersects gstBoundingSphere with spatial object and returns
	// gstIntersection::intersectionType which can be; enclosed, enclosing,
	// overlapping, or none. 'enclosed' specifies that the sphere is enclosed within
	// the object, 'enclosing' specifies that the sphere encloses the spatial object,
	// 'overlapping' specifies that the sphere and the spatial object overlap
	// eachother, and 'none' specifies that the sphere and spatial object occupy
	// separate space.
	//
	virtual gstObjectIntersectionInfo::IntersectionType intersect(
		const gstBoundingSphere &sphere) const;

	//
	// Intersects gstRay with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
	// specifies an intersection into the object, 'out' specifies an intersection
	// leaving the object, 'inOut' specifies intersections entering and leaving the
	// object, and 'none' specifies that the object is not intersected.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		const gstRay &ray,
		gstLineIntersectionInfoFirstTwo_Param &intersectionInfo) const;

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
		const gstLineSegment &lineSegment,
		gstLineIntersectionInfoFirstTwo_Param &intersectionInfo) const;

	//
	// Intersects gstRay with spatial object and returns
	// gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
	// specifies an intersection into the object, 'out' specifies an intersection
	// leaving the object, 'inOut' specifies intersections entering and leaving the
	// object, and 'none' specifies that the object is not intersected.
	//
	virtual gstLineIntersectionInfo::IntersectionType intersect(
		const gstRay &ray,
		gstLineIntersectionInfoFirst_Param &intersectionInfo) const;

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
		const gstLineSegment &lineSegment,
		gstLineIntersectionInfoFirst_Param &intersectionInfo) const;

	//
	// Returns gstPoint coincident with the center of the left side
	// of this cube.
	//
	inline gstPoint leftCenter() const;

	//
	// Returns gstPoint coincident with the left lower back corner of this gstBoundingBox.
	//
	inline gstPoint leftLowerBackCorner() const;

	//
	// Returns gstPoint coincident with the left lower front corner of this gstBoundingBox.
	//
	inline gstPoint leftLowerFrontCorner() const;

	//
	// Returns gstPoint coincident with the left upper back corner of this gstBoundingBox.
	//
	inline gstPoint leftUpperBackCorner() const;

	//
	// Returns gstPoint coincident with the left upper front corner of this gstBoundingBox.
	//
	inline gstPoint leftUpperFrontCorner() const;

	//
	// Returns gstPoint coincident with the center of the right side
	// of this cube.
	//
	inline gstPoint rightCenter() const;

	//
	// Returns gstPoint coincident with the right lower back corner of this gstBoundingBox.
	//
	inline gstPoint rightLowerBackCorner() const;

	//
	// Returns gstPoint coincident with the right lower front corner of this gstBoundingBox.
	//
	inline gstPoint rightLowerFrontCorner() const;

	//
	// Returns gstPoint coincident with the right upper back corner of this gstBoundingBox.
	//
	inline gstPoint rightUpperBackCorner() const;

    //
	// Returns gstPoint coincident with the right upper front corner of this gstBoundingBox.
	//
	inline gstPoint rightUpperFrontCorner() const;

	//
	// Sets the center of the cube at centerParam.
	//
	void setCenter(
		const gstPoint &centerParam);

	//
	// Sets the cube side length to sideLengthParam.
	//
	void setSideLength(
		const double sideLengthParam);

	//
	// Returns gstPoint coincident with the center of the top
	// of this cube.
	//
	inline gstPoint topCenter() const;

  private:

	gstPoint center;

	double sideLength;
};



inline gstPoint gstBoundingCube::backCenter() const
{
	return(gstPoint(center[0],center[1],center[2]-sideLength/2.0));
}


inline gstPoint gstBoundingCube::bottomCenter() const
{
	return(gstPoint(center[0],center[1]-sideLength/2.0,center[2]));
}


inline gstPoint gstBoundingCube::frontCenter() const
{
	return(gstPoint(center[0],center[1],center[2]+sideLength/2.0));
}


inline gstPoint gstBoundingCube::getCenter() const
{
	return (center);
}


inline double gstBoundingCube::getSideLength() const
{
	return(sideLength);
}


inline gstPoint gstBoundingCube::leftCenter() const
{
	return(gstPoint(center[0]-sideLength/2.0,center[1],center[2]));
}


inline gstPoint gstBoundingCube::leftLowerBackCorner() const
{
	return(gstPoint(center[0]-sideLength/2.0,
                    center[1]-sideLength/2.0,
                    center[2]-sideLength/2.0));
}


inline gstPoint gstBoundingCube::leftLowerFrontCorner() const
{
	return(gstPoint(center[0]-sideLength/2.0,
                    center[1]-sideLength/2.0,
                    center[2]+sideLength/2.0));
}


inline gstPoint gstBoundingCube::leftUpperBackCorner() const
{
	return(gstPoint(center[0]-sideLength/2.0,
                    center[1]+sideLength/2.0,
                    center[2]-sideLength/2.0));
}


inline gstPoint gstBoundingCube::leftUpperFrontCorner() const
{
	return(gstPoint(center[0]-sideLength/2.0,
                    center[1]+sideLength/2.0,
                    center[2]+sideLength/2.0));
}


inline gstPoint gstBoundingCube::rightCenter() const
{
	return(gstPoint(center[0]+sideLength/2.0,
                    center[1],
                    center[2]));
}


inline gstPoint gstBoundingCube::rightLowerBackCorner() const
{
	return(gstPoint(center[0]+sideLength/2.0,
                    center[1]-sideLength/2.0,
                    center[2]-sideLength/2.0));
}


inline gstPoint gstBoundingCube::rightLowerFrontCorner() const
{
	return(gstPoint(center[0]+sideLength/2.0,
                    center[1]-sideLength/2.0,
                    center[2]+sideLength/2.0));
}


inline gstPoint gstBoundingCube::rightUpperBackCorner() const
{
	return(gstPoint(center[0]+sideLength/2.0,
                    center[1]+sideLength/2.0,
                    center[2]-sideLength/2.0));
}


inline gstPoint gstBoundingCube::rightUpperFrontCorner() const
{
	return(gstPoint(center[0]+sideLength/2.0,
                    center[1]+sideLength/2.0,
                    center[2]+sideLength/2.0));
}


inline gstPoint gstBoundingCube::topCenter() const
{
	return(gstPoint(center[0],
                    center[1]+sideLength/2.0,
                    center[2]));
}


#endif  // GSTBOUNDINGCUBE_INCLUDED
