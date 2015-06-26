//=============================================================================
//   Filename : gstBoundedHapticObj.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz, Chris Tarr, Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for nodes which have a gstBoundingVolume
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BOUNDED_HAPTIC_OBJ
#define GST_BOUNDED_HAPTIC_OBJ

#include <gstDllExport.h>
#include <gstBoundingVolume.h>

// Base class for haptic objects that utilize a 
// bounding volume.
class GHOST_DLL_IMPORT_EXPORT gstBoundedHapticObj : public gstTransform
{
    GHOST_INHERIT_HEADER(gstBoundedHapticObj, gstTransform);

public:

	virtual ~gstBoundedHapticObj();

	// Downcast function.
	gstBoundedHapticObj *AsBoundedHapticObj() {return this;}

	// Does the bounding volume need updating
	gstBoolean getNeedsUpdate();

	// Set the state of the bounding volume w.r.t. updating
	void setNeedsUpdate(gstBoolean t_or_f);

	// Assign a gstBoundingVolume instance to the haptic object
	virtual void setBoundingVolume(gstBoundingVolume *boundingObj);

	// Get the gstBoundingVolume instance currently associated with the haptic object
	virtual gstBoundingVolume *getBoundingVolume() const;

	// Utility method to bound the haptic object by a sphere
	gstBoolean boundBySphere(const gstPoint &center, const double radius);

	// Utility method to bound the haptic object by a box
	gstBoolean boundByBox(const gstPoint &center,
        const double xlen, const double ylen, const double zlen);

	// Returns TRUE or FALSE depending on if the sphere intersects the 
	// bounding volume
	virtual gstBoolean withinBoundingVolume(const gstPoint &start, const gstPoint &end);

	// This method will return the closest distance from the input point to the 
	// bounding volume
	virtual double getDistanceFromBoundingVolumeWC(const gstPoint &point);

	// Invalidate the cumulative transform for the object
	// This will trigger the recalculation of the bounding sphere in world coordinates
	virtual void invalidateCumTransf();

protected:
	// This class is intended as a base class only, the constructors
	// are protected so that instances can not be created.
	gstBoundedHapticObj();

	// Copy Ctor.
	gstBoundedHapticObj(const gstBoundedHapticObj &);
	gstBoundedHapticObj(const gstBoundedHapticObj *);

private:
	bool mNeedsUpdate;
	gstBoundingVolume *mBoundingVolume;
};

// Helper function for setting the center of the bounding volume
// instance associated to the haptic object
GHOST_DLL_IMPORT_EXPORT
void setBoundingCenter(gstBoundedHapticObj *bounded, const gstPoint &radius);

// Helper function for setting the radius of the bounding sphere
// instance associated with the haptic object
GHOST_DLL_IMPORT_EXPORT
void setBoundingRadius(gstBoundedHapticObj *bounded, double radius);

// Helper function for getting the radius of the bounding sphere
// instance associated with the haptic object
GHOST_DLL_IMPORT_EXPORT
double getBoundingRadius(gstBoundedHapticObj *bounded);

// Helper function for setting the x-axis length of the
// bounding box instance associated with the haptic object
GHOST_DLL_IMPORT_EXPORT
void setBoundingDimensions(gstBoundedHapticObj *bounded,
                           double xlen, double ylen, double zlen);

#endif
