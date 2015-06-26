//=============================================================================
//   Filename : gstLineIntersectionInfoFirst_ParamSpatObj.h
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

#if !defined(GSTLINEINTERSECTIONINFOFIRST_PARAMSPATOBJ_INCLUDED)
#define GSTLINEINTERSECTIONINFOFIRST_PARAMSPATOBJ_INCLUDED

#include <gstDllExport.h> 
#include <gstLineIntersectionInfoFirst_Param.h>

class GHOST_DLL_IMPORT_EXPORT gstSpatialObject;

//
// First intersection along line with object including parametric value (t1) of line where
// first intersection occurs and pointer to gstSpatialObject intersected by line.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfoFirst_ParamSpatObj:
	public gstLineIntersectionInfoFirst_Param
{

  public:

	// Constructor
	gstLineIntersectionInfoFirst_ParamSpatObj() : d_spatialObj(0) {}


	virtual ~gstLineIntersectionInfoFirst_ParamSpatObj();

	//
	// Returns pointer to intersected spatial object.
	//
	inline gstSpatialObject * getSpatialObject() const;

	//
	// Sets pointer to intersected spatial object to spatialObjs.
	//
	inline void setSpatialObject(
		gstSpatialObject *spatialObj);

	//
	// Returns pointer to intersected spatial object.
	//
	inline gstSpatialObject * spatialObject() const;

  private:

	gstSpatialObject *d_spatialObj;
};


inline gstSpatialObject * 
gstLineIntersectionInfoFirst_ParamSpatObj::getSpatialObject() const
{
	return(d_spatialObj);
}


inline void gstLineIntersectionInfoFirst_ParamSpatObj::setSpatialObject(
	gstSpatialObject *spatialObj)
{
	d_spatialObj = spatialObj;
}


inline gstSpatialObject *
gstLineIntersectionInfoFirst_ParamSpatObj::spatialObject() const
{
	return(d_spatialObj);
}


#endif  // GSTLINEINTERSECTIONINFOFIRST_PARAMSPATOBJ_INCLUDED
