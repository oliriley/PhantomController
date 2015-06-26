//=============================================================================
//   Filename : gstSpatialPartition.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for spatial partion, such as gstBinTree
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_SPATIALPARTITION_H_
#define _GST_SPATIALPARTITION_H_

#include <gstDllExport.h> 
#include <gstSpatialObject.h>

// Class to act as a base class for all types of spatial partitions
// which will get imposed on GHOST objects.
class GHOST_DLL_IMPORT_EXPORT gstSpatialPartition : public gstSpatialObject
{
	public:
		gstSpatialPartition() {}
		virtual ~gstSpatialPartition() {}

		// Triggers the building of the partition.
		// This does not happen by default at construction time.
		virtual void init() {} 

		// Notification method indicating that a particular gstSpatialObject
		// inside of the partition is going to be modified/moved.
		// Should return value indicating success or failure in
		// finding the specified gstSpatialObject inside of the partition.
		// Base class version does nothing.
        virtual gstBoolean beginModify(gstSpatialObject *) { return FALSE; }

		// Notification method indicating that a particular gstSpatialObject
		// inside of the partition is finished being modified/moved.
		// Should return value indicating success or failure in
		// finding the specified gstSpatialObject inside of the partition.
		// Base class version does nothing.
        virtual gstBoolean endModify(gstSpatialObject *) { return FALSE; }

	private:
};


#endif // _GST_SPATIALPARTITION_H_
