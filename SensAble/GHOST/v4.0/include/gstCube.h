//=============================================================================
//   Filename : gstCube.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents the geometry of a box (i.e. different length sides)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_CUBE
#define GST_CUBE

#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstPHANToM.h>
#include <gstShape.h>

#define CUBE_FRONT 	1
#define CUBE_BACK 	2
#define CUBE_LEFT 	3
#define CUBE_RIGHT 	4
#define CUBE_TOP 	5
#define CUBE_BOTTOM 6

// Cube primitive.  This class represents the geometry of a cube.
// The default size orientation is centered at the origin with
// all side lengths of 2.0.
class GHOST_DLL_IMPORT_EXPORT gstCube : public gstShape
{
    GHOST_INHERIT_HEADER(gstCube, gstShape);

public:

	// Constructor.
	gstCube();

	// Constructor.
	gstCube(const gstCube &);

	// Destructor.
	virtual ~gstCube() {}

	// Clone.
	virtual gstNode *Clone() const {return CloneCube();}
	gstCube *CloneCube() const {return (new gstCube(*this));}

	// Get width of object [millimeters] along x axis.
	double				getWidth() const { return width; }

	// Set width of object [millimeters] along x axis.
	void				setWidth(double newWidth);
	
	// Get height of object [millimeters] along y axis.
	double				getHeight() const { return height; }

	// Set height of object [millimeters] along y axis.
	void				setHeight(double newHeight);

	// Get length of object [millimeters] along z axis.
	double				getLength() const { return length; }

	// Set length of object [millimeters] along z axis.
	void				setLength(double newLength);


gstInternal public:
	
	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	//  Returns TRUE if line segment defined by startPt_WC and endPt_WC
	//  intersects this shape object.  If so, intersectionPt_WC is set to
	//  point of intersection and intersectionNormal_WC is set to surface
	//  normal at intersection point.
	virtual gstBoolean	intersection(	const gstPoint &startPt_WC,
										const gstPoint &endPt_WC,
										gstPoint &intersectionPt_WC,
										gstVector &intersectionNormal_WC,
										void **);

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Returns TRUE if PHANToM is currently in contact with this object.
	// If so, the collision is added to the PHANToM's list through
	// gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
	virtual int			collisionDetect(gstPHANToM *PHANToM) ;

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Returns TRUE if pt is inside of this object.
	virtual int			checkIfPointIsInside_WC(const gstPoint &pt) ;

protected:
	gstBoolean	beenOutside;
	gstPoint	old_pos;
	double		length;	
	double		width; 
	double		height;	
};	

#endif
