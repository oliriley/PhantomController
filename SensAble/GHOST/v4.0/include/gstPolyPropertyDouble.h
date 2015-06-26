//=============================================================================
//   Filename : gstPolyPropertyDouble.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Property storing double data associated with an entire
//              polygon or one of its vertices (ie. normal vector).  
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPOLYPROPERTYDOUBLE_INCLUDED)
#define GSTPOLYPROPERTYDOUBLE_INCLUDED

#include <gstDllExport.h>
#include <gstPolyPropertyBase.h>

//
// Property storing double data associated with an entire polygon or
// one of its vertices (ie. normal vector).  
//
class GHOST_DLL_IMPORT_EXPORT gstPolyPropertyDouble:
	public gstPolyPropertyBase
{

  public:


	gstPolyPropertyDouble();


	gstPolyPropertyDouble(
		const double &val);

	//
	//
	// Accessor to property data of type double.  Returns TRUE 
	// and places the double value into val.
	//
	inline virtual gstBoolean getDoubleValue(
		double &val) const;

	//
	//
	// Returns TRUE to identify that this property does
	// store values of type double.
	//
	inline virtual gstBoolean isDoubleProp() const;

	//
	//
	// Returns TRUE and stores val.
	//
	inline virtual gstBoolean setDoubleValue(
		const double &val);

  protected:


	virtual ~gstPolyPropertyDouble();

  private:


	double d_data;
};



inline gstBoolean gstPolyPropertyDouble::getDoubleValue(
	double &val) const
{
	val = d_data;
	return(TRUE);
}


inline gstBoolean gstPolyPropertyDouble::isDoubleProp() const
{
	return(TRUE);
}


inline gstBoolean gstPolyPropertyDouble::setDoubleValue(
	const double &val)
{
	d_data = val;
	return(TRUE);
}


#endif  // GSTPOLYPROPERTYDOUBLE_INCLUDED
