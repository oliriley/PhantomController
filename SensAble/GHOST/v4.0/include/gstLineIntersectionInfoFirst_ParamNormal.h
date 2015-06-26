//=============================================================================
//   Filename : gstLineIntersectionInfoFirst_ParamNormal.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description:
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTLINEINTERSECTIONINFOFIRST_PARAMNORMAL_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRST_PARAMNORMAL_INCLUDED


#include <gstDllExport.h> 
#include <gstVector.h>
#include <gstLineIntersectionInfoFirst_Param.h>

//
// First intersection along line with object including parametric value (t1) of line where
// first intersection occurs and normal of surface at point of intersection.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirst_ParamNormal:
	public gstLineIntersectionInfoFirst_Param
{

  public:

    //
	// Returns value of intersection normal.
	//
	inline const gstVector & getNormal() const;

	//
	// Returns value of intersection normal.
	//
	const gstVector & normal() const;

	//
	// Sets value of intersection normal to normal.
	//
	inline void setNormal(
		const gstVector &normal);

  private:


	gstVector d_normal;					// Normal of surface at intersection
};



inline const gstVector & gstLineIntersectionInfoFirst_ParamNormal::getNormal() const
{
	return(d_normal);
}


inline void gstLineIntersectionInfoFirst_ParamNormal::setNormal(
	const gstVector &normal)
{
	d_normal = normal;
}


#endif  // GSTLINEINTERSECTIONINFOFIRST_PARAMNORMAL_INCLUDED
