//=============================================================================
//   Filename : gstTransformedBoundingBox.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz, Chris Tarr, Tim Gallagher	
//    Revised : Ted Bardasz
// ----------------------------------------------------------------------------
// Description: Maintains a bounding box in world coordinates for performance
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TRANSFORMED_BOUNDING_BOX
#define GST_TRANSFORMED_BOUNDING_BOX

#include <gstBoundingVolume.h>
#include <gstBoundingBox.h>

// A class that implements a bounding box bounding volume implementation for use
// by the PHANToM.  This implementation keeps a representation of the bounding volume
// in both local and world coordinates.  The world coordinate representation is kept
// up to date automatically and is used to optimize the checking of whether the PHANToM
// is within the focus of the owning haptic object.
class GHOST_DLL_IMPORT_EXPORT gstTransformedBoundingBox : public gstBoundingVolume {
public:
	// Constructors
	gstTransformedBoundingBox();
	
	// Constructor
	gstTransformedBoundingBox(const gstPoint &center, const double xlen, const double ylen, const double zlen);

	// Constructor
	gstTransformedBoundingBox(const gstTransformedBoundingBox *origBox);

	// Constructor
	gstTransformedBoundingBox(const gstTransformedBoundingBox &origBox);

	// Destructors
	~gstTransformedBoundingBox();

	// Cloning function
	virtual gstBoundingVolume *Clone() const {return (gstBoundingVolume *)CloneBoundBox();}
	// Cloning function
	gstTransformedBoundingBox *CloneBoundBox() const {return (new gstTransformedBoundingBox(*this));}

	// Accessor to set the center of the bounding box in local coordinates
	void setCenter(const gstPoint &center);

	// Accessor to set the x-axis length of the bounding box in local coordinates
	void setXlength(const double xlength);

	// Accessor to set the y-axis length of the bounding box in local coordinates
	void setYlength(const double ylength);

	// Accessor to set the z-axis length of the bounding box in local coordinates
	void setZlength(const double zlength);

	// Accessor to set the center of the bounding box in local coordinates
	gstPoint getCenter(int localOrWC = GST_LOCAL_FRAME) const;

	// Accessor to set the x-axis length of the bounding box in local coordinates
	double getXlength(int localOrWC = GST_LOCAL_FRAME) const;

	// Accessor to set the y-axis length of the bounding box in local coordinates
	double getYlength(int localOrWC = GST_LOCAL_FRAME) const;

	// Accessor to set the z-axis length of the bounding box in local coordinates
	double getZlength(int localOrWC = GST_LOCAL_FRAME) const;

	// Get the normal bounding box representation
	gstBoundingBox *getBox(int localOrWC) const;

	// GHOST_IGNORE_IN_DOC
	// Internal method
	// Get the local coordinate representation of this bounding box
	gstBoundingBox *getBoxLocal() const {
		return mBoxLocal;
	}

	// GHOST_IGNORE_IN_DOC
	// Internal method
	// Get the world coordinate representation of this bounding box
	gstBoundingBox *getBoxWC() const {
		return mBoxWC;
	}

	// GHOST_IGNORE_IN_DOC
	// Internal method
	// Set the local coordinate representation of this bounding box
	void setBoxLocal(gstBoundingBox *newBox){
		assert(newBox);

		if (mBoxLocal  && (mBoxLocal != newBox))
			delete mBoxLocal;
		mBoxLocal = newBox;
	}

	// GHOST_IGNORE_IN_DOC
	// Internal method
	// Set the world coordinate representation of this bounding box
	void setBoxWC(gstBoundingBox *newBox){
		assert(newBox);

		if (mBoxWC && (mBoxWC != newBox))
			delete mBoxWC;
		mBoxWC = newBox;
	}

	// This method will test whether the input line intersects the bounding box
	gstBoolean testLineIntWC(const gstPoint &startPt, const gstPoint &endPt);

	// This method will return the closest distance between the input point
	// and the bounding volume
	double getDistanceWC(const gstPoint &point);

	// This method will update the WC version of the bounding volume
	// based on the transform of the owner passed in
	gstBoolean update(gstTransform *owner);

	// Get the bounding volume of the correct type
	gstTransformedBoundingBox *asBox() {return this;}

protected:
	// Helper methods
	void convertPointsToLengths(const gstPoint &blb, const gstPoint &frt, gstPoint &center, double &length, double &width, double &height) const;
	void convertLengthsToPoints(const gstPoint &center, const double length, const double width, const double height, gstPoint &blb, gstPoint &frt) const;
	gstBoolean transformBoxToWorld(gstTransformMatrix &cumMat);

private:
	gstBoundingBox *mBoxLocal;
	gstBoundingBox *mBoxWC;
};


#endif
