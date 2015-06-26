//=============================================================================
//   Filename : gstTriPolyMeshBase.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for triangular polygon mesh, such as gstTriPolyMesh
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTTRIPOLYMESHBASE_INCLUDED)
#define GSTTRIPOLYMESHBASE_INCLUDED

#include <gstDllExport.h> 
#include <gstModifyBase.h>
#include <gstLineIntersectionInfoFirst_ParamTriPoly.h>
#include <gstSpatialObject.h>


//
// Base class for triangular polygons class.
//
class GHOST_DLL_IMPORT_EXPORT gstTriPolyMeshBase : public gstSpatialObject
{
    GHOST_INHERIT_HEADER(gstTriPolyMeshBase, gstSpatialObject);

  public:

	virtual ~gstTriPolyMeshBase();

	//
	// enumeration used to specify clockwise or counterclockwise specification of
	// polygon normal based on vertices.
    //
  	typedef enum VertexOrder_ {RV_CLOCKWISE, RV_COUNTERCLOCKWISE} VertexOrder;

	//
	// Set vertex order triangle polygons are specified by to determine their surface
	// normal.  This value can be RV_CLOCKWISE or RV_COUNTERCLOCKWISE.
	//
	gstTriPolyMeshBase::VertexOrder getVertexOrder();

	//
	// Signifies that modifiedTriPoly has been modified.
	//
	void modify(
		gstTriPoly *modifiedTriPoly);

	//
	// Set vertex order triangle polygons are specified in to determine their surface
	// normal.  This value can be RV_CLOCKWISE, RV_COUNTERCLOCKWISE.
	//
	void setVertexOrder(
		gstTriPolyMeshBase::VertexOrder newOrder);// clockwise or counterclockwise


	//
	// Specifies an object of derived type gstModifyBase that uses or
	// owns this object.  The owner object will have it's modified
	// method called when this object is modified.
	//
	void setOwner(gstModifyBase *owner) {
		d_owner = owner;
	}

	//
	// Signifies that this object may be modified after this call.
	//
	virtual gstBoolean beginModify(gstSpatialObject *){ return TRUE;}
	
	//
	// Signifies that modifications to the argument spatial object 
	// have ceased.  It is assumed that the spatial object is a part
	// of this triangular polygon mesh.
	//
	virtual gstBoolean endModify(gstSpatialObject *) { return TRUE; }

	//
	//
	// Signifies that modifications to this object and any gstTriPoly
	// objects making up the triangular mesh have ceased.
	//
	virtual void endModifications() {
		if (d_owner != NULL)
			d_owner->modified();
	}

	//
	// Returns the owner object of derived type gstModifyBase that uses 
	// or owns this object.
	//
	gstModifyBase *getOwner() const {
		return d_owner;
	}

	// Return the angle tolerance between faces.
	double getCreaseAngle() const {return d_creaseAngle;}

	// Set the angle tolerance between faces.
	void setCreaseAngle(double a) {d_creaseAngle = a;}

protected:

	// This class is intended as a base class only, the constructors
	// are protected so that instances can not be created.
	gstTriPolyMeshBase();

	//
	// Copy constructor
	//
	gstTriPolyMeshBase(
		const gstTriPolyMeshBase &mesh);

  private:

	VertexOrder d_vertexOrder;	// Order of vertices used to compute normal
	gstModifyBase *d_owner;	// Pointer to owner class of type gstModifyBase

	// crease angle tolerance between faces
	double d_creaseAngle;
};


#endif  // GSTTRIPOLYMESHBASE_INCLUDED
