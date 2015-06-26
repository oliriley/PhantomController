//=============================================================================
//   Filename : gstLineIntersectionInfo.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Encodes intersection data of a line, ray, or line segment with a box.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTLINEINTERSECTIONINFO_INCLUDED)
#define GSTLINEINTERSECTIONINFO_INCLUDED


#include <gstDllExport.h> 

class ostream;

//
// Stores status of intersection of line with some object.  This status
// may indicate no intersection, no intersection with line inside object,
// no intersection with line outside object, intersection into object,
// intersection into and out of object, intersection out of object,
// and intersection through object using the respective enumerations of
// gstLineIntersectionInfo::IntersectionType.
//
class GHOST_DLL_IMPORT_EXPORT gstLineIntersectionInfo
{
  public:

	typedef enum  resultValues
        {RV_NONE, RV_NONE_INSIDE, RV_NONE_OUTSIDE,
         RV_IN, RV_INOUT, RV_OUT, RV_THROUGH} IntersectionType;

	IntersectionType d_status;

	// Constructor
	gstLineIntersectionInfo() : d_status(RV_NONE) {}

	//
	// Returns the status of the intersection.
	//
	inline gstLineIntersectionInfo::IntersectionType getStatus() const;

	//
	// Sets status of the intersection to status.
	//
	inline void setStatus(
		gstLineIntersectionInfo::IntersectionType status);

	//
	// Returns the status of the intersection.
	//
	inline gstLineIntersectionInfo::IntersectionType status() const;
};



inline gstLineIntersectionInfo::IntersectionType
gstLineIntersectionInfo::getStatus() const
{
	return(d_status);
}


inline void gstLineIntersectionInfo::setStatus(
	gstLineIntersectionInfo::IntersectionType status)
{
	d_status = status;
}


inline gstLineIntersectionInfo::IntersectionType
gstLineIntersectionInfo::status() const
{
	return(d_status);
}

// insert operator for IntersectionType
GHOST_DLL_IMPORT_EXPORT ostream & operator <<
    (ostream& os, const gstLineIntersectionInfo::IntersectionType &type);


#endif  // GSTLINEINTERSECTIONINFO_INCLUDED
