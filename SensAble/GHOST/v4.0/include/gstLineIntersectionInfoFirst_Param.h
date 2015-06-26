//=============================================================================
//   Filename : gstLineIntersectionInfoFirst_Param.h
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

#if !defined(GSTLINEINTERSECTIONINFOFIRST_PARAM_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRST_PARAM_INCLUDED

#include <gstDllExport.h> 
#include <gstLineIntersectionInfo.h>

//
// First intersection along line with object including parametric
// value (t1) of line where first intersection occurs.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirst_Param:
	public gstLineIntersectionInfo
{

  public:

	virtual ~gstLineIntersectionInfoFirst_Param();

	//
	// Returns value of T1.
	//
	inline double getT1() const;

	//
	// Sets value of T1 to t1.
	//
	inline void setT1(const double t1);

	//
	// Returns value of T1.
	//
	inline double t1() const;

  private:

	double d_t1; // Parametric value (t) of intersection on line
};



inline double gstLineIntersectionInfoFirst_Param::getT1() const
{
	return(d_t1);
}


inline void gstLineIntersectionInfoFirst_Param::setT1(const double t1)
{
	d_t1 = t1;
}


inline double gstLineIntersectionInfoFirst_Param::t1() const
{
	return(d_t1);
}


#endif  // GSTLINEINTERSECTIONINFOFIRST_PARAM_INCLUDED
