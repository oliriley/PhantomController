//=============================================================================
//   Filename : gstBoundaryCube.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Box/cube version of a gstBoundary
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BOUNDARY_CUBE
#define GST_BOUNDARY_CUBE

#include <gstDllExport.h>
#include <gstBoundary.h>

// Bounding cube boundary class.  This restricts the PHANToM inside a
// box-shaped volume.  The box is by default located at the origin
// with the width along the X-axis, height along the Y-axis, and depth 
// (length) along the Z-axis.
class GHOST_DLL_IMPORT_EXPORT gstBoundaryCube : public gstBoundary
{
    GHOST_INHERIT_HEADER(gstBoundaryCube, gstBoundary);

public:

	// Constructor.
	gstBoundaryCube();

	// Copy Constructor.
	gstBoundaryCube(const gstBoundaryCube &);

	// Destructor.
	virtual ~gstBoundaryCube() {}

	// Clone.
	virtual gstNode *Clone() const {return CloneBoundaryCube();}
	gstBoundaryCube *CloneBoundaryCube() const {return (new gstBoundaryCube(*this));}


	// FOR_GHOST_EXTENSION:
	// Checks to see if line segment intersects the box boundary.
	// TRUE is returned if the line segment defined by startPt_WC and endPt_WC
	// intersects the boundary.  If so, intersectionPt_WC is set to the
	// point of intersection and intersectionNormal_WC is set to surface
	// normal at intersection point.
	virtual gstBoolean  intersection (	const gstPoint &startPt_WC,
										const gstPoint &endPt_WC, 
										gstPoint &intersectionPt_WC,
										gstVector &intersectionNormal_WC,
										void **);

	// FOR_GHOST_INTERNAL:
	// Returns TRUE if point (specified in world coordinates) is inside
	// of boundary.102942720 Feb 
	virtual gstBoolean	checkIfPointIsInside_WC(const gstPoint &pt);

	// FOR_GHOST_EXTENSION:
	// Returns TRUE if a line drawn from the previous to the current
	// position of the PHANToM intersects the boundary.
	virtual gstBoolean	collisionDetect(gstPHANToM *PHANToM);
  
	// Set width (X-axis) of boundary [millimeters].
	void setWidth(double newWidth) {
		width = newWidth;
	}
	
	// Set height (Y-axis) of boundary [millimeters].
    void setHeight(double newHeight) {
		height = newHeight;
	}

	// Set depth/length (Z-axis) of boundary [millimeters].
	void setLength(double newLength) {
		length = newLength;
	}

    // Get height (Y-axis) of boundary [millimeters].
    double getHeight() {
        return height;
    }

    // Get depth/length (Z-axis) of boundary [millimeters].
    double getLength() {
        return length;
    }

    // Get width (X-axis) of boundary [millimeters].
    double getWidth() {
        return width;
    }
protected:

	double length, width, height;

};

#endif


