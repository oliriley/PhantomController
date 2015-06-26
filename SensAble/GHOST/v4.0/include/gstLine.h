//=============================================================================
//   Filename : gstLine.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: A geometric line, used by many other classes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTLINE_INCLUDED)
#define GSTLINE_INCLUDED

#include <gstLineBase.h>

//
// Implements an infinite line passing through space
//
class GHOST_DLL_IMPORT_EXPORT gstLine:
	public gstLineBase
{

  public:

	//
	// Constructor defines line passing through p1 and directed along v1.  The
	// parametric form is defined as f(t)=(1-t)P1+t(P1+V1).  Thus f(t)=p1 at t=0 and
	// f(t)=p1+v1 at t=1.
	//
	gstLine(
		const gstPoint &_p1,					// Line passes through _p1
		const gstVector &_v1);				// Line is directed along v1

	//
	// Constructor defines line as passing through points p1 and p2.  The parametric
	// representation is then f(t) = (1-t)P1+tP2.
	//
	gstLine(
		const gstPoint &_p1,					// Line passes through p1
		const gstPoint &_p2);					// Line passes through p2

	virtual ~gstLine();

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	inline virtual gstVector direction() const;

	//
	// Evaluates parametric form of line at t and returns point at f(t)
	//
	virtual gstPoint eval(
		const double t) const;

	//
	// Returns a point on the line.
	//
	virtual const gstPoint & getPointOnLine() const;

	//
	// Returns a point on the line.
	//
	virtual const gstPoint & pointOnLine() const;

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector unitDirection() const;

  private:

	//
	// f(t)=p1 when t=0
	//
	gstPoint p1;

	//
	// f(t)=p2 when t=1
	//
	gstPoint p2;
};


inline gstVector gstLine::direction() const
{
	return(p2 - p1);
}


#endif  // GSTLINE_INCLUDED
