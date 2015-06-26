//=============================================================================
//   Filename : gstLineIntersectionInfoFirst_ParamTriPoly.h
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

#if !defined(GSTLINEINTERSECTIONINFOFIRST_PARAMTRIPOLY_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRST_PARAMTRIPOLY_INCLUDED

#include <gstDllExport.h> 
#include <gstLineIntersectionInfoFirst_Param.h>

class GHOST_DLL_IMPORT_EXPORT gstTriPoly;

//
// First intersection along line with object including parametric value (t1) of line where
// first intersection occurs and pointer to gstTriPoly intersected by line.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirst_ParamTriPoly:
	public gstLineIntersectionInfoFirst_Param
{

  public:

	virtual ~gstLineIntersectionInfoFirst_ParamTriPoly();

	//
	// Returns pointer to intersected gstTriPoly.
	//
	inline gstTriPoly * getTriPoly() const;

    //
	// Sets pointer of intersected gstTriPoly to triPoly.
	//
	inline void setTriPoly(
		gstTriPoly *triPoly);

	//
	// Returns pointer to intersected gstTriPoly.
	//
	inline gstTriPoly * triPoly() const;

  private:

	gstTriPoly *d_triPoly;
};



inline gstTriPoly * gstLineIntersectionInfoFirst_ParamTriPoly::getTriPoly() const
{
	return(d_triPoly);
}


inline void gstLineIntersectionInfoFirst_ParamTriPoly::setTriPoly(
	gstTriPoly *triPoly)
{
	d_triPoly = triPoly;
}


inline gstTriPoly * gstLineIntersectionInfoFirst_ParamTriPoly::triPoly() const
{
	return(d_triPoly);
}


#endif  // GSTLINEINTERSECTIONINFOFIRST_PARAMTRIPOLY_INCLUDED
