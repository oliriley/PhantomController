//=============================================================================
//   Filename : gstBoundary.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: base shape class, for keeping PHANToM inside a shape
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BOUNDARY
#define GST_BOUNDARY

#include <gstDllExport.h>
#include <gstShape.h>

// Base boundary class. Keep the PHANTOM inside this shape.
class GHOST_DLL_IMPORT_EXPORT gstBoundary : public gstShape
{
    GHOST_INHERIT_HEADER(gstBoundary, gstShape);

public:

	// Destructor.
	virtual ~gstBoundary();

	// Clone.
	virtual gstNode *Clone() const {return CloneBoundary();}

	// Clone as gstBoundary.
	gstBoundary *CloneBoundary() const {return (new gstBoundary(*this));}

gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Checks to see if line segment intersects the boundary.
	// TRUE is returned if the line segment defined by startPt_WC and endPt_WC
	// intersects the boundary.  If so, intersectionPt_WC is set to the
	// point of intersection and intersectionNormal_WC is set to surface
	// normal at intersection point.
	virtual gstBoolean  intersection (	const gstPoint &startPt_WC,
										const gstPoint &endPt_WC, 
										gstPoint &intersectionPt_WC,
										gstVector &intersectionNormal_WC,
										void **) { 
		(void) startPt_WC;
		(void) endPt_WC;
		(void) intersectionPt_WC;
		(void) intersectionNormal_WC;
		return FALSE;
	}

	// FOR_GHOST_INTERNAL:
	// Returns TRUE if point (specified in world coordinates) is inside
	// of boundary. 
	virtual gstBoolean	checkIfPointIsInside_WC(const gstPoint &pt) {
		(void) pt;
		return FALSE;
	}

	// FOR_GHOST_EXTENSION:
	// Returns TRUE if a line drawn from the previous to the current
	// position of the PHANToM intersects the boundary.
	virtual gstBoolean	collisionDetect(gstPHANToM *PHANToM) {
		(void) PHANToM;
		return FALSE;
	}


	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Puts boundary object in scene graph.  Note that 
	// gstShapes::putInSceneGraph and removeFromSceneGraph must be skipped
	// so that this object is not put in the shape list.  This is a special
	// shape type which is only felt by an identified gstPHANToM object
	// and should not be considered a regular geometry object.
	virtual void		putInSceneGraph() {gstTransform::putInSceneGraph();}

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Remove boundary object from scene graph.
	virtual void		removeFromSceneGraph() {gstTransform::removeFromSceneGraph();}

protected:

    // constructors are protected, use class as parent only
	// Constructor.
	gstBoundary();

	// Constructor.
	gstBoundary(const gstBoundary *);

	// Copy Constructor.
	gstBoundary(const gstBoundary &);
};
#endif
