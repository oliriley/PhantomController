//=============================================================================
//   Filename : gstRay.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: gstRay represents a ray or half line in 3D space.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTRAY_INCLUDED)
#define GSTRAY_INCLUDED

#include <gstDllExport.h>
#include <gstLineBase.h>

// gstRay represents a ray or half line in 3D space.  The implementation of the
// ray allows the set of common operations performed on a ray and can represent the
// ray in parametric form f(t) such that (t>=0).
//
// The internal representation is stored in parametric form f(t)=(1-t)P1+t(P1+V1).
//
// Some of the methods should be closely considered.  Should ray act as line for
// project and eval when t<0 or t>1?


//
// Implements a ray originating at p1 and directed along the vector v1.
// This ray is defined parametrically such that f(0.0)=p1 and f(1.0)=p1+v1
//
class GHOST_DLL_IMPORT_EXPORT gstRay:
	public gstLineBase
{

  public:

	//
	// Constructor defines ray starting at p1 directed along v1. The parametric form is
	// defined as f(t)=(1-t)P1+t(P1+V1).  Thus f(t)=p1 at t=0 and f(t)=p1+v1 at t=1.
	//
	gstRay(
		const gstPoint &p1,				// Line passes through p1
		const gstVector &v1);			// Line is directed along v1

	//
	// Constructor defines ray starting at p1 and passing through p2 at t=1.  The
	// parametric representation is then f(t) = (1-t)P1+tP2.
	//
	gstRay(
		const gstPoint &p1,				// Line passes through p1
		const gstPoint &p2);			// Line passes through p2

	gstRay();

	virtual ~gstRay();

        //
        // comparison
        //
        bool operator==(const gstRay& rhs) const;

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector direction() const;

	//
	// Evaluates parametric form of line segment at t and returns point at f(t). p1 is
	// returned for values of t<0 and p2 is returned for values of t > 1.
	//
	gstPoint eval(
		const double t) const;

	//
	// Gives the parametric value t of the line segment for the projection of pt onto
	// the line segment.
	//
	double eval(
		const gstPoint &pt) const;

	//
	// Returns a point on the line.
	//
	virtual const gstPoint & getPointOnLine() const;

	//
	// Returns p1.
	//
	gstPoint origin() const;

	//
	// Returns a point on the line.
	//
	virtual const gstPoint & pointOnLine() const;

	//
	// Projects pt onto the line segment and returns the parametric value of the
	// projection.
	//
	double projectToParametric(
		const gstPoint &pt) const;

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector unitDirection() const;

  private:

	gstPoint d_p1;						// f(t)=p1 when t=0

	gstVector d_v1;						// f(t)=p1+v1 when t=1
};


#endif  // GSTRAY_INCLUDED
