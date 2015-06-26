//=============================================================================
//   Filename : gstLineIntersectionInfoFirstTwo_Param.h
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

#if !defined(GSTLINEINTERSECTIONINFOFIRSTTWO_PARAM_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRSTTWO_PARAM_INCLUDED

#include <gstDllExport.h> 
#include <gstLineIntersectionInfoFirst_Param.h>

//
// First two intersections along line with object including parametric values (t1 and t2) of line where
// first two intersections occur.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirstTwo_Param:
	public gstLineIntersectionInfoFirst_Param
{

  public:

	//
	// Returns value of T2.
	//
	inline double getT2() const;

	//
	// Sets value of T2 to t2.
	//
	inline void setT2(
		const double t2);

    //
	// Returns value of T2.
	//
	inline double t2() const;

  private:

	double d_t2;    // Parametric value of second intersection on line
};



inline double gstLineIntersectionInfoFirstTwo_Param::getT2() const
{
	return(d_t2);
}


inline void gstLineIntersectionInfoFirstTwo_Param::setT2(
	const double t2)
{
	d_t2 = t2;
}


inline double gstLineIntersectionInfoFirstTwo_Param::t2() const
{
	return(d_t2);
}


#endif  // GSTLINEINTERSECTIONINFOFIRSTTWO_PARAM_INCLUDED
