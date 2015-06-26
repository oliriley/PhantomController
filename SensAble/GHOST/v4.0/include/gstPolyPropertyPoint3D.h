//=============================================================================
//   Filename : gstPolyPropertyPoint3D.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Property storing gstPoint data associated with an entire
//              polygon or one of its vertices (ie. normal vector).  
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPOLYPROPERTYPOINT3D_INCLUDED)
#define GSTPOLYPROPERTYPOINT3D_INCLUDED


#include <gstDllExport.h>
#include <gstPolyPropertyBase.h>

//
// Property storing gstPoint data associated with an entire polygon or
// one of its vertices (ie. normal vector).  
//
class GHOST_DLL_IMPORT_EXPORT gstPolyPropertyPoint3D:
	public gstPolyPropertyBase
{

  public:

	gstPolyPropertyPoint3D();


	gstPolyPropertyPoint3D(
		const gstPoint &val);

	//
	// Accessor to property data of type gstPoint.  Returns TRUE 
	// and places the gstPoint value into val.
	//
	inline virtual gstBoolean getPoint3DValue(
		gstPoint &val) const;

	//
	// Returns TRUE to identify that this property does
	// store values of type gstPoint.
	//
	inline virtual gstBoolean isPoint3DProp() const;

	//
	// Returns TRUE and stores val.
	//
	inline virtual gstBoolean setPoint3DValue(
		const gstPoint &val);

  protected:

	virtual ~gstPolyPropertyPoint3D();

  private:

	gstPoint d_data;
};



inline gstBoolean gstPolyPropertyPoint3D::getPoint3DValue(
	gstPoint &val) const
{
	val = d_data;
	return(TRUE);
}


inline gstBoolean gstPolyPropertyPoint3D::isPoint3DProp() const
{
	return(TRUE);
}


inline gstBoolean gstPolyPropertyPoint3D::setPoint3DValue(
	const gstPoint &val)
{
	d_data = val;
	return(TRUE);
}


#endif  // GSTPOLYPROPERTYPOINT3D_INCLUDED
