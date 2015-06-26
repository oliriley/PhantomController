//=============================================================================
//   Filename : gstTransformedBoundingSphere.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz, Chris Tarr, Tim Gallagher	
//    Revised : Ted Bardasz
// ----------------------------------------------------------------------------
// Description: Maintains bounding sphere in world coordinates for performance
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TRANSFORMED_BOUNDING_SPHERE
#define GST_TRANSFORMED_BOUNDING_SPHERE

#include <gstBoundingVolume.h>
#include <gstBoundingSphere.h>

// A class that implements a bounding sphere bounding volume implementation for use
// by the PHANToM.  This implementation keeps a representation of the bounding volume
// in both local and world coordinates.  The world coordinate representation is kept
// up to date automatically and is used to optimize the checking of whether the PHANToM
// is within the focus of the owning haptic object.
class GHOST_DLL_IMPORT_EXPORT gstTransformedBoundingSphere : public gstBoundingVolume {
public:
	// Constructors
	gstTransformedBoundingSphere();
	
	// Constructor
	gstTransformedBoundingSphere(const gstPoint &center, double radius);

	// Constructor
	gstTransformedBoundingSphere(const gstTransformedBoundingSphere *origSphere);

	// Constructor
	gstTransformedBoundingSphere(const gstTransformedBoundingSphere &origSphere);

	// Destructors
	~gstTransformedBoundingSphere();

	// Cloning function
	virtual gstBoundingVolume *Clone() const {return (gstBoundingVolume *)CloneBoundSphere();}

	// Cloning function
	gstTransformedBoundingSphere *CloneBoundSphere() const {return (new gstTransformedBoundingSphere(*this));}

	// Accessor for setting the center of the bounding volume in local coordinates
	void setCenter(const gstPoint &center);

	// Accessor for setting the radius of the bounding volume in local coordinates
	void setRadius(const double radius);

	// Accessor for getting the center of the bounding volume.
	gstPoint getCenter(int localOrWC = GST_LOCAL_FRAME) const ;

	// Accessor for setting the radius of the bounding volume.
	double getRadius(int localOrWC = GST_LOCAL_FRAME) const ;

	// Get the normal bounding sphere representation of the bounding volume
	gstBoundingSphere *getSphere(int localOrWC) const;

	// Get the local coordinate representation of the bounding volume
	gstBoundingSphere *getSphereLocal() const {
		return mSphereLocal;
	}

	// Get the world coordinate representation of the bounding volume
	gstBoundingSphere *getSphereWC() const {
		return mSphereWC;
	}

	// Set the local coordinate representation of the bounding volume
	void setSphereLocal(gstBoundingSphere *newSphere){
		assert(newSphere);

		if (mSphereLocal && (newSphere != mSphereLocal))
			delete mSphereLocal;

		mSphereLocal = newSphere;
	}

	// Set the world coordinate representation of the bounding volume
	void setSphereWC(gstBoundingSphere *newSphere){
		assert(newSphere);

		if (mSphereWC && (newSphere != mSphereWC))
			delete mSphereWC;

		mSphereWC = newSphere;
	}

	// A method to determine if the line segment specified by startPt and endPt
	// intersects the bounding volume
	gstBoolean testLineIntWC(const gstPoint &startPt, const gstPoint &endPt);

	// A method to get to closest distance from point to the bounding sphere.
	double getDistanceWC(const gstPoint &point);

	// A method to update the world coordinate representation of the bounding sphere
	// based on the local representation and translations, rotations and scales from the
	// world coordinate system
	gstBoolean update(gstTransform *owner);

	// Get the bounding volume of the correct type
	virtual gstTransformedBoundingSphere *asSphere() {return this;}

private:
	gstBoundingSphere *mSphereLocal;
	gstBoundingSphere *mSphereWC;
};

#endif
