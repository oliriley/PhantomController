//=============================================================================
//   Filename : gstLineIntersectionInfoFirstTwo_ParamEdge.h
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

#if !defined(GSTLINEINTERSECTIONINFOFIRSTTWO_PARAMEDGE_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRSTTWO_PARAMEDGE_INCLUDED

#include <gstDllExport.h> 
#include <gstLineIntersectionInfoFirstTwo_Param.h>

class gstEdge;

//
// First two intersections along line coplanar with polygonal face including parametric values
// (t1 and t2) of line where first two intersections occur and includes pointers to the polygon's
// edges (e1 and e2) for the respective two intersections.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirstTwo_ParamEdge:
	public gstLineIntersectionInfoFirstTwo_Param
{

  public:

	//
	// Returns value of E1.
	//
	inline gstEdge * e1() const;

	//
	// Returns value of E2.
	//
	inline gstEdge * e2() const;

	//
	// Returns value of E1.
	//
	inline gstEdge * getE1() const;

	//
	// Returns value of E2.
	//
	inline gstEdge * getE2() const;

	//
	// Sets value of E1 to e1.
	//
	inline void setE1(
		gstEdge *e1);

	//
	// Sets value of E2 to e2.
	//
	inline void setE2(
		gstEdge *e2);

  private:

	gstEdge *d_e1;	// Edge crossed at 1st intersection
    gstEdge *d_e2;	// Edge crossed at 2nd intersection
};



inline gstEdge * gstLineIntersectionInfoFirstTwo_ParamEdge::e1() const
{
	return(d_e1);
}


inline gstEdge * gstLineIntersectionInfoFirstTwo_ParamEdge::e2() const
{
	return(d_e2);
}


inline gstEdge * gstLineIntersectionInfoFirstTwo_ParamEdge::getE1() const
{
	return(d_e1);
}


inline gstEdge * gstLineIntersectionInfoFirstTwo_ParamEdge::getE2() const
{
	return(d_e2);
}


inline void gstLineIntersectionInfoFirstTwo_ParamEdge::setE1(
	gstEdge *e1)
{
	d_e1 = e1;
}


inline void gstLineIntersectionInfoFirstTwo_ParamEdge::setE2(
	gstEdge *e2)
{
	d_e2 = e2;
}


#endif  // GSTLINEINTERSECTIONINFOFIRSTTWO_PARAMEDGE_INCLUDED
