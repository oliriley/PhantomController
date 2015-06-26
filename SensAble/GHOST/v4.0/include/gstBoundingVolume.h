//=============================================================================
//   Filename : gstBoundingVolume.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz, Chris Tarr, Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: base bounding volume used by gstBoundedHapticObj
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BOUNDING_VOLUME
#define GST_BOUNDING_VOLUME

#define GST_LOCAL_FRAME 0
#define GST_WC_FRAME 1

#include <gstDllExport.h>

#include <gstTransform.h>
#include <gstBoundingBox.h>
#include <gstBoundingSphere.h>

class gstBoundedHapticObj;
class gstTransformedBoundingSphere;
class gstTransformedBoundingBox;


// GHOST Class to inherit from to define bounding volumes
// An instance of this class is pointed to by the gstBoundedHapticObj.
class GHOST_DLL_IMPORT_EXPORT gstBoundingVolume {

public:

	// Destructor
	virtual ~gstBoundingVolume();

	// Cloning function
	virtual gstBoundingVolume *Clone() const = 0;

	// GHOST_IGNORE_IN_DOC 
	// Internal method
	// Accessor for getting the update state of this bounding volume
	gstBoolean getNeedsUpdate() {return mNeedsUpdate;}

	// GHOST_IGNORE_IN_DOC 
	// Internal method
	// Accessor for setting the update state of this bounding volume
	void setNeedsUpdate(gstBoolean t_or_f) {mNeedsUpdate = t_or_f;}

	// GHOST_IGNORE_IN_DOC 
	// Internal method
	// Accessor for setting the owner node of this bounding volume
	void setOwnerNode(gstBoundedHapticObj *owner);

	// GHOST_IGNORE_IN_DOC 
	// Internal method
	// Accessor for getting the owner node of this bounding volume
	gstBoundedHapticObj *getOwnerNode() const;

	// virtual function for setting the center of the bounding volume
	virtual void setCenter(const gstPoint &center) = 0;

    // virtual function for getting the center of the bounding volume
    virtual gstPoint getCenter(int localOrWC = GST_LOCAL_FRAME) const = 0;

	// GHOST_IGNORE_IN_DOC
	// Internal Method
	// This method will determine if the line segment specified by startPt and endPt
	// intersects the bounding volume.
	gstBoolean withinBoundingVolume(const gstPoint &startPt, const gstPoint &endPt) {
		return testLineIntWC(startPt, endPt);
	}

	// This method will return the smallest distance between the input point and 
	// the bounding volume in world coordinates;
	virtual double getDistanceWC(const gstPoint &point) = 0;

	// Predicate method as to whether a sphere intersects the volume
	virtual gstBoolean testLineIntWC(const gstPoint &startPt, const gstPoint &endPt) = 0;

	// Updates the position of the bounding volume object
	virtual gstBoolean update(gstTransform *owner) = 0;

	// Method to downcast a pointer of this type to a specific bounding volume class.
	virtual gstTransformedBoundingSphere *asSphere() {return 0;}
	virtual gstTransformedBoundingBox *asBox() {return 0;}

protected:
	// This class is intended as a base class only, the constructors
	// are protected so that instances can not be created.
	gstBoundingVolume();
	gstBoundingVolume(const gstBoundingVolume *origBounder);
	gstBoundingVolume(const gstBoundingVolume &origBounder);

private:
	gstBoolean mNeedsUpdate;
	gstBoundedHapticObj *mOwnerNode;
};



#endif
