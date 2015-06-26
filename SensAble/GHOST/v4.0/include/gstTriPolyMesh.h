//=============================================================================
//   Filename : gstTriPolyMesh.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Triangular polygen mesh, polgons may or may not share vertices
//              and edges.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTTRIPOLYMESH_INCLUDED)
#define GSTTRIPOLYMESH_INCLUDED


#include <gstDllExport.h> 

#include <list>
#include <hash_map>

#include <gstBinTree.h>
#include <gstTriPoly.h>
#include <gstObjectIntersectionInfo.h>
#include <gstLineIntersectionInfoFirst_ParamSpatObj.h>
#include <gstObjectIntersectionInfo_SpatialObjectPtrSet.h>

class gstSpatialPartition;

// In debug builds, FreeForm redefines the __STD_DEFAULT_ALLOCATOR.  Ghost explicelty
// exports it's templates using __default_alloc_template.  Therefore, we have to 
// explicitely specify the allocator type in debug builds because the the template
// generated types won't match those exported by ghost.  This is only an issue in this
// case becuase gstTriPolyMeshHaptic exposes a direct reference to one of these arrays.
// In Release,
// Even though the allocator types match, we can't explicitely expose it because
// MSDEV will treat the explicit definition as an error.
// C2908: explicit specialization; '__default_alloc_template<1,0>' has already been 
// specialized from the primary template. 
//
// AT BEST, I THINK THIS SHOULD BE A WARNING CLASS DIAGNOSTIC!!
//
#if defined( _DEBUG ) && defined( _BUILDING_FREEFORM )
    typedef hash_map<int, gstVertex *, hash<int>, equal_to<int>, class __default_alloc_template<1,0> > VertexMap;
#else   // Not debug or Not building freeform
    typedef hash_map<int, gstVertex *, hash<int>, equal_to<int> > VertexMap;
#endif  // _DEBUG && _BUILDING_FREEFORM

typedef VertexMap::iterator gstVertexPtrHashMapIterator;
typedef VertexMap::const_iterator gstVertexPtrHashMapConstIterator;


//
// Implements collection of triangular polygons that may or may not share vertices
// and edges.
//
class GHOST_DLL_IMPORT_EXPORT gstTriPolyMesh : public gstTriPolyMeshBase
{
    GHOST_INHERIT_HEADER(gstTriPolyMesh, gstTriPolyMeshBase);

    typedef void (* ModifyCallbackFunc)(gstTriPolyMesh *, void *);

  public:

    //
    // Creates an initially empty set of triangular polygons.
    //
    gstTriPolyMesh();

    // Copy Constructor
    gstTriPolyMesh(
        const gstTriPolyMesh &mesh);

    // Constructor. Backward compatible with old gstPolyMesh.
    gstTriPolyMesh(
        int numVertices,            // the number of vertices
        double vertices[][3],        // the vertices
        int numTrianglePolygons,    // the number of triangles
        int trianglePolygons[][3],    // the triangles
        gstBoolean useSpatialPartition = TRUE);

    // Constructor. Backward compatible with old gstPolyMesh.
    gstTriPolyMesh(
        int numVertices,            // the number of vertices
        int v_dimension,            // dimension of vertices == 3
        double **vertices,            // the vertices
        int numTrianglePolygons,    // the number of triangles
        int num_sides,                // number of sides of polys == 3
        int **trianglePolygons,        // the triangles
        gstBoolean useSpatialPartition = TRUE);

    // Constructor.
    gstTriPolyMesh(
        int numVertices,            // the number of vertices
        int v_dimension,            // dimension of vertices == 3
        double *vertices,            // the vertices
        int numTrianglePolygons,    // the number of triangles
        int num_sides,                // number of sides of polys == 3
        int *trianglePolygons,        // the triangles
        gstBoolean useSpatialPartition = TRUE);

    virtual ~gstTriPolyMesh();

    //
    // virtual method to copy this object and return base class
    // pointer to newly copied object.  Allows copy of object to be
    // made using base class pointer.
    inline virtual gstSpatialObject * clone() const;

    //
    // virtual method to copy this gstTriPolyMesh object and return 
    // gstTriPolyMesh pointer to newly cloned object.  
    //
    inline gstTriPolyMesh * cloneTriPolyMesh() const;

    //
    // Create new gstTriPoly by copying poly's vertices first and then 
    // creating poly with copied vertices.  A pointer to the newly 
    //    created polygon is returned.
    //
    gstTriPoly * copyPolygon(
        const gstTriPoly *poly);

    //
    // Creates new gstVertex with same position as vertToCopy if a vertex
    // at that position does not already exist.  Also copies the key if
    // the key is not already used by a vertex of this gstTriPolyMesh.  
    // A pointer to the already existing coincident vertex or the
    // newly created vertex is returned.
    //
    gstVertex * copyVertex(
        gstVertex *vertToCopy);

    //
    // Creates new gstTriPoly with vertices v1, v2, and v3 and gives the
    // new polygon a key of polyKey.  A pointer to the newly created polygon
    // is returned if successful.  If v1, v2, and v3 are not unique
    // then NULL is returned.  If polyKey is not unique, then a new unique
    // key is generated and assigned to the new polygon.
    //
    gstTriPoly * createTriPoly(
        gstVertex *v1,
        gstVertex *v2,
        gstVertex *v3,
        gstPolyKey polyKey=INT_MAX);

    //
    // Creates new gstTriPoly with vertices v1, v2, and v3 and gives the
    // new polygon a key of polyKey.  A pointer to the newly created polygon
    // is returned if successful.  If v1, v2, and v3 not valid and unique
    // keys to polygons of this gstTriPolyMesh, then NULL is returned.  
    // If polyKey is not unique, then a new unique key is generated and 
    // assigned to the new polygon.
    //
    gstTriPoly * createTriPoly(
        const gstVertexKey v1key,
        const gstVertexKey v2key,
        const gstVertexKey v3key,
        const gstPolyKey polyKey=INT_MAX);

    //
    // Create new gstVertex to be used in gstTriPolys that follow.  New gstVertex is
    // positioned at vertPos and is indexed by vertkey.  If vertKey is not
    // unique, then a unique key and generated and used.  A pointer to the 
    // newly created gstVertex is returned if successful.  Otherwise, NULL
    // is returned.
    //
    // Note: Does not check for existing vertices of same position or key.
    //
    gstVertex * createVertex(
        const gstPoint vertPos,
        const gstVertexKey vertKey=INT_MAX);

    //
    // Signifies that this object may be modified after this call.
    //
    virtual gstBoolean beginModify(gstSpatialObject *so){ 
        if (spatialPartition)
            return spatialPartition->beginModify(so);
        else
            return TRUE;
    }

    //
    // Signifies that modifications to the argument spatial object 
    // have ceased.  It is assumed that the spatial object is a part
    // of this triangular polygon mesh.
    //
    virtual gstBoolean endModify(gstSpatialObject *so) {
        if (spatialPartition)
            return spatialPartition->endModify(so);
        else
            return TRUE;
    }

    //
    // Returns pointer to gstVertex coincident with vert if one exists.
    // Otherwise, NULL is returned.
    //
    gstVertex * findCoincidentVertex(
        const gstVertex *vert);

    //
    // Returns gstBoundingBox just enclosing this triangle.
    //
    virtual gstBoundingBox getBoundingBox();

    //
    // Returns gstBoundingSphere just enclosing this triangle.
    //
    virtual gstBoundingSphere getBoundingSphere();

    //
    // Returned vector of pointers to gstSpatialObjects that
    // are contained within this gstTriPolyMesh.
    //
    // Note: This operation takes proportional time and memory to
    // number of contained objects.
    //
    virtual gstBoolean getContainedObjects(
        gstSpatialObjectPtrVector &v);

    //
    // Returns TRUE if vertices that have no neighbor edges
    // are to be deleted.  Otherwise, FALSE is returned.
    gstBoolean getDeleteStrandedVertices() const;

    //
    // Returns number of polygons defined for gstTriPolyMesh.
    //
    inline int getNumPolygons() const;

    //
    // Returns number of vertices defined for gstTriPolyMesh.
    // This includes vertices that are stranded (have no neighbor
    // polygons) that have not been deleted.
    //
    inline int getNumVertices() const;

    //
    // Returns pointer to gstTriPolygon that has key equal to polyKey if
    // one exists.  Otherwise, NULL is returned.
    //
    gstTriPoly * getPolygon(
        const gstPolyKey polyKey) const;

    //
    // Returns pointer to gstSpatialPartition defined for this object if
    // one has been created.  Otherwise, NULL is returned.
    //
    gstSpatialPartition * getSpatialPartition();

    //
    // Returns pointer to gstVertex that has key equal to vertKey if
    // one exists.  Otherwise, NULL is returned.
    //
    gstVertex * getVertex(
        const gstVertexKey vertKey) const;

    //
    // Creates a new spatial partition for this object if one hasn't
    // already been defined and initializes it to reflect the mesh
    // defined by this gstTriPolyMesh.
    //
    void initSpatialPartition();

    //
    // Not implemented yet
    //
    virtual gstObjectIntersectionInfo::IntersectionType intersect_BC(
        const gstBoundingCube &cube);

    //
    // Intersects gstSimpleCube with spatial object and returns
    // gstIntersection::intersectionType which can be; enclosed, enclosing,
    // overlapping, or none. 'enclosed' specifies that the simple cube is enclosed
    // within the object, 'enclosing' specifies that the simple cube encloses the
    // spatial object, 'overlapping' specifies that the simple cube and the spatial
    // object overlap eachother, and 'none' specifies that the simple cube and spatial
    // object occupy separate space.
    //
    virtual gstObjectIntersectionInfo::IntersectionType intersect_BS_SOSet(
        const gstBoundingSphere &sphere,
        gstObjectIntersectionInfo_SpatialObjectPtrSet &intersectionInfo);


    virtual gstLineIntersectionInfo::IntersectionType intersect_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_LS_PSO(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_LS_PSO(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_PSO(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // Returns an iterater pointing to the begining of the polygons 
    // defined for this gstTriPolyMesh.
    //
    inline gstTriPolyPtrHashMapConstIterator polygonsBegin() const;

    //
    // Returns an iterater pointing to the end of the polygons 
    // defined for this gstTriPolyMesh.
    //
    inline gstTriPolyPtrHashMapConstIterator polygonsEnd() const;

    //
    // Removes gstTriPoly defined for this mesh with key equal to polyKey and
    // returns TRUE if one exists that meets this criteria.  Otherwise, FALSE is 
    // returned.
    //
    gstBoolean removePolygon(
        const gstPolyKey polyKey);

    //
    // Removes gstTriPoly polyToRemove if it is contained by this mesh and returns TRUE.
    // Otherwise, FALSE is returned.
    //
    gstBoolean removePolygon(
        gstTriPoly **polyToRemove);

    //
    // Removes gstVertex defined for this mesh with key equal to vertKey and
    // returns TRUE if one exists that meets this criteria.  Otherwise, FALSE is 
    // returned.
    //
    gstBoolean removeVertex(
        const gstVertexKey vertKey);

    //
    // If deleteStrandedVertices is TRUE, vertices that have no neighbor edges
    // are to be deleted.  Otherwise, they are left alone.
    //
    inline void setDeleteStrandedVertices(
        const gstBoolean deleteStrandedVertices);

    //
    // sets spatial partition defined for this object to s.
    //
    void setSpatialPartition(
        gstSpatialPartition *s);

    //
    // If u is TRUE the spatial partition defined for this mesh is
    // used to speed up calculations if one exists.  Otherwise, all
    // polygons are examined for calculations requiring it.
    //
    void setUseSpatialPartition(
        gstBoolean u);

    //
    // Returns TRUE if the spatial partition defined for this mesh is
    // used to speed up calculations if one has been created.  Otherwise
    // FALSE is returned meaning all polygons are examined for 
    // calculations requiring it.
    //
    gstBoolean useSpatialPartition() const;

    //
    // Returns begining iterator for vertices.
    //
    inline gstVertexPtrHashMapConstIterator verticesBegin() const;

    //
    // Returns ending iterator for vertices.
    //
    inline gstVertexPtrHashMapConstIterator verticesEnd() const;

  private:

    // Pointer to spatial partition of polys
    gstSpatialPartition *spatialPartition;    

    // Vertices generated for this mesh
    VertexMap verts;    

    // Triangular polygons generated for this mesh
    TriPolyMap polys;    

    // If TRUE then vertices stranded by removing a polygon are deleted.
    gstBoolean d_deleteStrandedVertices;

    // Used to generate unique keys for gstTriPolys when not specified by user
    gstPolyKey d_uniquePolyKey;            

    // Used to generate unique keys for gstVertex when not specified by user
    gstVertexKey d_uniqueVertexKey;        

    // TRUE if spatial partition is to be used
    gstBoolean d_useSpatialPartition;    

    //
    // Calls copyPolygon on all gstTriPolys of polysToCopy
    //
    void copyPolygons(
        TriPolyMap &newPolys,
        const TriPolyMap &polysToCopy);

    //
    // Calls copyVertex on all gstVertices of vertsToCopy
    //
    void copyVertices(
        VertexMap &newVerts,
        const VertexMap &vertsToCopy);

    //
    // returns a gstPolyKey that is not currently used by any polygons held by this
    // object.
    //
    gstPolyKey getUniquePolyKey();

    //
    // returns a gstPolyKey that is not currently used by any polygons held by this
    // object.
    //
    gstVertexKey getUniqueVertexKey();
};



inline gstSpatialObject * gstTriPolyMesh::clone() const
{
    return(cloneTriPolyMesh());
}


inline gstTriPolyMesh * gstTriPolyMesh::cloneTriPolyMesh() const
{
    return(new gstTriPolyMesh(*this));
}


inline int gstTriPolyMesh::getNumPolygons() const
{
    return(polys.size());
}


inline int gstTriPolyMesh::getNumVertices() const
{
    return(verts.size());
}


inline gstTriPolyPtrHashMapConstIterator gstTriPolyMesh::polygonsBegin() const
{
    return(polys.begin());
}


inline gstTriPolyPtrHashMapConstIterator gstTriPolyMesh::polygonsEnd() const
{
    return(polys.end());
}


inline void gstTriPolyMesh::setDeleteStrandedVertices(
    const gstBoolean deleteStrandedVertices)
{
    d_deleteStrandedVertices = deleteStrandedVertices;
}


inline gstVertexPtrHashMapConstIterator gstTriPolyMesh::verticesBegin() const
{
    return(verts.begin());
}


inline gstVertexPtrHashMapConstIterator gstTriPolyMesh::verticesEnd() const
{
    return(verts.end());
}


#endif  // GSTTRIPOLYMESH_INCLUDED
