//=============================================================================
//   Filename : gstObjectIntersectionInfo_SpatialObjectPtrSet.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Used by gstTriPolyMesh and gstSpatialPartition
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTOBJECTINTERSECTIONINFO_SPATIALOBJECTPTRSET_INCLUDED)
#define GSTOBJECTINTERSECTIONINFO_SPATIALOBJECTPTRSET_INCLUDED

#include <gstDllExport.h> 
#include <set>
#include <gstSpatialObject.h>
#include <gstObjectIntersectionInfo.h>

// Collection of intersections
class GHOST_DLL_IMPORT_EXPORT gstObjectIntersectionInfo_SpatialObjectPtrSet:
	public gstObjectIntersectionInfo
{

  public:

	set< gstSpatialObject *, less<gstSpatialObject *> > * getIntersectedObjects();

	void setIntersectedObjects(
		set< gstSpatialObject *, less<gstSpatialObject *> > *intObjs);

  private:

	set<gstSpatialObject *, less<gstSpatialObject *> > *d_intersectedObjs;
};


#endif  // GSTOBJECTINTERSECTIONINFO_SPATIALOBJECTPTRSET_INCLUDED
