//=============================================================================
//   Filename : gstTriPolyBase.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for triangular polygon, such as gstTriPoly
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTTRIPOLYBASE_INCLUDED)
#define GSTTRIPOLYBASE_INCLUDED

#include <gstDllExport.h> 
#include <gstPlane.h>

//
// Base class for triangular polygons
//
class GHOST_DLL_IMPORT_EXPORT gstTriPolyBase:
	public gstPlane
{

  public:

	gstTriPolyBase();

	virtual ~gstTriPolyBase();

	//
	// GHOST_IGNORE_IN_DOC:
	// Get type of this class.  No instance needed.
	//
	static gstType getClassTypeId()
	{
		gstAssignUniqueId(gstTriPolyBaseClassTypeId);
		return gstTriPolyBaseClassTypeId;
	}

	//
	// GHOST_IGNORE_IN_DOC:
	// Get type Id of this instance.
	//
	inline virtual gstType getTypeId() const;

    //
	// GHOST_IGNORE_IN_DOC:
	// Returns TRUE if this class is same or derived class of type.
	//
	inline virtual gstBoolean isOfType(
		gstType type) const;

	//
	// GHOST_IGNORE_IN_DOC:
	// Returns TRUE if subclass is of type.
	//
	inline static gstBoolean staticIsOfType(
		gstType type);

  private:

	static gstType gstTriPolyBaseClassTypeId; // Type Id for class
};


inline gstType gstTriPolyBase::getTypeId() const
{
	return getClassTypeId();
}


inline gstBoolean gstTriPolyBase::isOfType(
	gstType type) const
{
	return staticIsOfType(type);
}


inline gstBoolean gstTriPolyBase::staticIsOfType(
	gstType type)
{
	if (type == getClassTypeId())
		return TRUE;
	else // Check if type is parent class.
		return (gstPlane::staticIsOfType(type));
}


#endif  // GSTTRIPOLYBASE_INCLUDED
