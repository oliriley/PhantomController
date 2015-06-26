//=============================================================================
//   Filename : gstPolyPropertyPoint2D.h
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

#if !defined(GSTPOLYPROPERTYPOINT2D_INCLUDED)
#define GSTPOLYPROPERTYPOINT2D_INCLUDED

#include <gstDllExport.h>
#include <gstPolyPropertyBase.h>


//
// Property storing gstPoint2D data associated with an entire polygon or
// one of its vertices (ie. normal vector).  
//
class GHOST_DLL_IMPORT_EXPORT gstPolyPropertyPoint2D:
	public gstPolyPropertyBase
{

  public:

	gstPolyPropertyPoint2D();

	gstPolyPropertyPoint2D(
		const gstPoint2D &val);

	//
	// Accessor to property data of type gstPoint2D.  Returns TRUE 
	// and places the gstPoint2D value into val.
	//
	inline virtual gstBoolean getPoint2DValue(
		gstPoint2D &val) const;

	//
	// Returns TRUE to identify that this property does
	// store values of type gstPoint2D.
	//
	inline virtual gstBoolean isPoint2DProp() const;

	//
	// Returns TRUE and stores val.
	//
	inline virtual gstBoolean setPoint2DValue(
		const gstPoint2D &val);

  protected:

	virtual ~gstPolyPropertyPoint2D();

  private:


	gstPoint2D d_data;
};



inline gstBoolean gstPolyPropertyPoint2D::getPoint2DValue(
	gstPoint2D &val) const
{
	val = d_data;
	return(TRUE);
}


inline gstBoolean gstPolyPropertyPoint2D::isPoint2DProp() const
{
	return(TRUE);
}


inline gstBoolean gstPolyPropertyPoint2D::setPoint2DValue(
	const gstPoint2D &val)
{
	d_data = val;
	return(TRUE);
}


#endif  // GSTPOLYPROPERTYPOINT2D_INCLUDED
