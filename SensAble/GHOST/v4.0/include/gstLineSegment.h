//=============================================================================
//   Filename : gstLineSegment.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: A geometric line segment, used by many other classes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTLINESEGMENT_INCLUDED)
#define GSTLINESEGMENT_INCLUDED

#include <gstDllExport.h>
#include <gstLineBase.h>


//
// Implements an line segment in space defined by two endpoints, p1 and p2.
//
class GHOST_DLL_IMPORT_EXPORT gstLineSegment:
	public gstLineBase
{

  public:

	//
	// Constructor defines line segment starting at p1 directed along v1 and ending at
	// p1+v1.  The parametric form is defined as f(t)=(1-t)P1+t(P1+V1).  Thus f(t)=p1
	// at t=0 and f(t)=p1+v1 at t=1.
	//

	gstLineSegment(
		const gstPoint &p1,				// Line passes through p1
		const gstVector &v1);			// Line is directed along v1

	//
	// Constructor defines line segment from p1 to p2.  The parametric representation
	// is then f(t) = (1-t)P1+tP2.
	//

	gstLineSegment(
		const gstPoint &p1,				// Line passes through p1
		const gstPoint &p2);			// Line passes through p2


	gstLineSegment(
		const gstLineSegment *lineSeg);


	gstLineSegment(
		const gstLineSegment &lineSeg);


	virtual ~gstLineSegment();

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//

	inline virtual gstVector direction() const;

	//
	// Evaluates parametric form of line segment at t and returns point at f(t). p1 is
	// returned for values of t<0 and p2 is returned for values of t > 1.
	//

	inline gstPoint eval(
		const double t) const;

	//
	// Gives the parametric value t of the line segment for the projection of pt onto
	// the line segment.
	//

	double eval(
		const gstPoint &pt) const;

	//
	// Returns p2
	//
	inline const gstPoint & getEndPoint() const;

	//
	// Returns a point on the line.
	//
	inline virtual const gstPoint & getPointOnLine() const;

	//
	// Returns p1
	//
	inline const gstPoint & getStartPoint() const;

	//
	// Returns length of line segment
	//
	double length() const;

	//
	// Returns p1
	//
	inline gstPoint origin() const;

	//
	// Projects pt onto the line segment and returns the parametric
    // value of the projection.
	//
	double projectToParametric(
		const gstPoint &pt) const;

	//
	// Sets p2 = endPoint
	//
	void setEndPoint(
		const gstPoint &endPoint);

	//
	// Sets p1 = startPoint
	//
	void setStartPoint(
		const gstPoint &startPoint);

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector unitDirection() const;

  private:

	//
	// f(t)=p1 when t=0
	//
	gstPoint p1_;

	//
	// f(t)=p2 when t=1
	//
	gstPoint p2_;

	double d_length;

	gstVector d_unitDirection;

	void update();
};



inline gstVector gstLineSegment::direction() const
{
	return(d_length*d_unitDirection);
}


inline gstPoint gstLineSegment::eval(
	const double t) const
{
	return (p1_+(t*d_length)*d_unitDirection);
}


inline const gstPoint & gstLineSegment::getEndPoint() const
{
	return(p2_);
}


inline const gstPoint & gstLineSegment::getPointOnLine() const
{
	return(p1_);
}


inline const gstPoint & gstLineSegment::getStartPoint() const
{
	return(p1_);
}


inline gstPoint gstLineSegment::origin() const
{
	return(p1_);
}


#endif  // GSTLINESEGMENT_INCLUDED
