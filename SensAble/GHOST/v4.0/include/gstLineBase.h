//=============================================================================
//   Filename : gstLineBase.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Base class for all types of lines passing through space 
//              (e.g. ray, line segment...)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTLINEBASE_INCLUDED)
#define GSTLINEBASE_INCLUDED


#include <gstDllExport.h>

#include <gstPoint.h>
#include <gstVector.h>

//
// Base class for all types of lines passing through space (ie. ray, line segment...)
//
class GHOST_DLL_IMPORT_EXPORT gstLineBase
{
  public:

	//
	// Constructor defines line as passing through points p1 and p2.  The parametric
	// representation is then f(t) = (1-t)P1+tP2.
	//
	gstLineBase(
		gstPoint &_p1,					// Line passes through p1
		gstPoint &_p2);					// Line passes through p2

	//
	// Constructor defines line passing through p1 and directed along v1.  The
	// parametric form is defined as f(t)=(1-t)P1+t(P1+V1).  Thus f(t)=p1 at t=0 and
	// f(t)=p1+v1 at t=1.
	//
	gstLineBase(
		gstPoint &_p1,					// Line passes through _p1
		gstVector &_v1);				// Line is directed along v1


	gstLineBase();


	virtual ~gstLineBase();

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector direction() const=0;

	//
	// Evaluates parametric form of line at t and returns point at f(t)
	//
	virtual gstPoint eval(
		const double t) const=0;

	//
	// Returns a point on the line.
	//
	virtual const gstPoint & getPointOnLine() const=0;

	//
	// Projects a point onto the line and returns the projected point on the line
	//
	virtual gstPoint project(
		const gstPoint &pt) const;

	//
	// gstVector directed along line in the direction of v1 or toward p2 depending on
	// constructor used.
	//
	virtual gstVector unitDirection() const;
};


#endif  // GSTLINEBASE_INCLUDED
