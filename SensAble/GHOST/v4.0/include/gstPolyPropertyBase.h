//=============================================================================
//   Filename : gstPolyPropertyBase.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for polgon properties
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================


#if !defined(GSTPOLYPROPERTYBASE_INCLUDED)
#define GSTPOLYPROPERTYBASE_INCLUDED


#include <gstDllExport.h>
#include <gstBasic.h>
#include <gstPoint2D.h>
#include <gstPoint.h>

//
// Base class for property associated with an entire polygon or
// one of its vertices (ie. normal vector).  Properties may be
// 1,2, or 3 dimensional quantities stored and returned as
// double, gstPoint2D, and gstPoint respectively.  The base class
// does not implement storage for any data, but does provide 
// virtual methods to access data.  All data accessors return FALSE
// in the base class.  Derived classes are to override the proper
// accessors to allow access to the data.
//
class GHOST_DLL_IMPORT_EXPORT gstPolyPropertyBase
{
  public:

	//
	// The destructor is kept protected since this is a reference counted 
	// object.
	//
	virtual ~gstPolyPropertyBase();

	//
	// Accessor to property data of type double.  This method
	// currently returns FALSE and has no effect on val.  If
	// a derived class stores double property data, then this
	// class is to return TRUE and place the double value into
	// val.
	//
	inline virtual gstBoolean getDoubleValue(
		double &val) const;

	//
	// Accessor to property data of type gstPoint2D.  This method
	// currently returns FALSE and has no effect on val.  If
	// a derived class stores gstPoint2D property data, then this
	// class is to return TRUE and place the gstPoint2D value into
	// val.
	//
	inline virtual gstBoolean getPoint2DValue(
		gstPoint2D &val) const;

	//
	// Accessor to property data of type gstPoint.  This method
	// currently returns FALSE and has no effect on val.  If
	// a derived class stores gstPoint property data, then this
	// class is to return TRUE and place the gstPoint value into
	// val.
	inline virtual gstBoolean getPoint3DValue(
		gstPoint &val) const;

	//
	// Pure virtual method to be implemented by derived classes.
	// This method should return a unique integer Id for the class.
	// This Id should be created as a static member of the derived
	// class using the createNewPropertyId method of this class.
	//
	inline virtual int Id()=0;

	//
	// Given three gstPolyPropertyBase derived objects, prop1, prop2, and 
	// prop3, that are assumed to store gstPoint data, the gstPoint 'result'
	// value is calculated by interpolating the 3 property values based on
	// the barycentric coordinate value barycentricCoord.  If successful, TRUE
	// is returned.  Otherwise, FALSE is returned.
	virtual gstBoolean interpolateBarycentric(
		gstPolyPropertyBase *prop1,
		gstPolyPropertyBase *prop2,
		gstPolyPropertyBase *prop3,
		const gstPoint &barycentricCoord,// point is within boundary of gstTriPoly
		gstPoint &result) const;

	//
	// Given three gstPolyPropertyBase derived objects, prop1, prop2, and 
	// prop3, that are assumed to store gstPoint2D data, the gstPoint2D 'result'
	// value is calculated by interpolating the 3 property values based on
	// the barycentric coordinate value barycentricCoord.  If successful, TRUE
	// is returned.  Otherwise, FALSE is returned.
	virtual gstBoolean interpolateBarycentric(
		gstPolyPropertyBase *prop1,
		gstPolyPropertyBase *prop2,
		gstPolyPropertyBase *prop3,
		const gstPoint &barycentricCoord,// point is within boundary of gstTriPoly
		gstPoint2D &result) const;

	//
	// Given three gstPolyPropertyBase derived objects, prop1, prop2, and 
	// prop3, that are assumed to store double data, the double 'result'
	// value is calculated by interpolating the 3 property values based on
	// the barycentric coordinate value barycentricCoord.  If successful, TRUE
	// is returned.  Otherwise, FALSE is returned.
	virtual gstBoolean interpolateBarycentric(
		gstPolyPropertyBase *prop1,
		gstPolyPropertyBase *prop2,
		gstPolyPropertyBase *prop3,
		const gstPoint &barycentricCoord,// point is within boundary of gstTriPoly
		double &result) const;

	//
	// Returns TRUE if the property stores double data.
	//
	inline virtual gstBoolean isDoubleProp() const;

	//
	// Returns TRUE if the property stores gstPoint2D data.
	//
	inline virtual gstBoolean isPoint2DProp() const;

	//
	// Returns TRUE if the property stores gstPoint data.
	//
	inline virtual gstBoolean isPoint3DProp() const;

	//
	// Denotes that some object is storing a pointer to this object
	// by incrementing the reference counter. Once the pointer is no 
	// longer stored by that object, unref() should be called.
	//
	inline void ref();

	//
	// Returns TRUE if the property stores double data and 
	// stores the value val.  Otherwise, FALSE is returned.
	//
	inline virtual gstBoolean setDoubleValue(
		const double &val);

	//
	// Returns TRUE if the property stores gstPoint2D data and 
	// stores the value val.  Otherwise, FALSE is returned.
	//
	inline virtual gstBoolean setPoint2DValue(
		const gstPoint2D &val);

	//
	// Returns TRUE if the property stores gstPoint data and 
	// stores the value val.  Otherwise, FALSE is returned.
	//
	inline virtual gstBoolean setPoint3DValue(
		const gstPoint &val);

	//
	// Denotes that an object previously storing a pointer to this object
	// is no longer doing so by decrementing the reference counter for 
	// this object.  If the reference counter is decremented to a value
	// less than or equal to zero then the storage for this object is deleted.
	//
	inline void unref();

  protected:

    //
	// This class is intended as a base class only, the constructor
	// is protected so that instances can not be created.
    //
	gstPolyPropertyBase() {}

	//
	// This method generates a unique integer Id that is mean't to be
	// used as a unique Id for each derived class of gstPolyPropertyBase.
	//
	static int createNewPropertyId();

  private:


	static int s_IdCounter;

	int d_referenceCounter;
};



inline gstBoolean gstPolyPropertyBase::getDoubleValue(
	double &val) const
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::getPoint2DValue(
	gstPoint2D &val) const
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::getPoint3DValue(
	gstPoint &val) const
{
	return(FALSE);
}


inline int gstPolyPropertyBase::Id()
{
	return(-1);
}


inline gstBoolean gstPolyPropertyBase::isDoubleProp() const
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::isPoint2DProp() const
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::isPoint3DProp() const
{
	return(FALSE);
}


inline void gstPolyPropertyBase::ref()
{
	d_referenceCounter++;
}


inline gstBoolean gstPolyPropertyBase::setDoubleValue(
	const double &val)
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::setPoint2DValue(
	const gstPoint2D &val)
{
	return(FALSE);
}


inline gstBoolean gstPolyPropertyBase::setPoint3DValue(
	const gstPoint &val)
{
	return(FALSE);
}


inline void gstPolyPropertyBase::unref()
{
	d_referenceCounter--;
	if (d_referenceCounter <= 0){
		delete this;
	}
}


#endif  // GSTPOLYPROPERTYBASE_INCLUDED
