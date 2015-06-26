//=============================================================================
//   Filename : gstTriPoly.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Triangular polygon, used by gstTriPolyMesh
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTTRIPOLY_INCLUDED)
#define GSTTRIPOLY_INCLUDED

#include <gstDllExport.h> 

#include <gstVertex.h>
#include <gstPoint2D.h>
#include <gstTriPolyBase.h>
#include <gstTriPolyMeshBase.h>
#include <gstPolyPropertyContainer.h>
#include <gstLineIntersectionInfoFirstTwo_ParamEdge.h>

#include <pair.h>
#include <vector.h>
#include <hash_map>


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstTriPoly *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstTriPoly *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstIncidentEdge, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstIncidentEdge, class __default_alloc_template<1,0> >;
#endif

typedef int gstPolyKey;
class gstLinePolyIntersectionData;

//
// Triangular Poly Class.  Every polygon is defined by its 3 vertices
// in the order specified by the parent gstTriPolyMeshBase class's
// getVertexOrder method.  The parent gstTriPolyMeshBase is specified 
// through the constructor.
class GHOST_DLL_IMPORT_EXPORT gstTriPoly : public gstTriPolyBase
{
    GHOST_INHERIT_HEADER(gstTriPoly, gstTriPolyBase);

  public:

    //
    // Creates Triangle defined by the 3 vertices v1, v2, and v3.  v1, v2, and v3
    // are assumed not to be all colinear and should all be distinct objects.  The
    // triangle will have undefined behavior otherwise.
    //
    gstTriPoly(
        gstVertex *_v1,
        gstVertex *_v2,
        gstVertex *_v3,
        gstTriPolyMeshBase *mesh=NULL,
        const gstPolyKey _key=NULL);


    virtual ~gstTriPoly();

    //
    // virtual method to copy this object and return base class
    // pointer to newly copied object.  Allows copy of object to be
    // made using base class pointer.
    //
    inline virtual gstSpatialObject * clone() const;

    //
    // virtual method to copy this gstTriPoly object and return 
    // gstTriPoly pointer to newly cloned object.  
    //
    inline gstTriPoly * cloneTriPoly() const;

    //
    // Given a 3D point coplaner with this triangle, the corresponding barycentric
    // coordinate is returned.  The returned barycentric coordinate is only valid if pt
    // is coplaner and within the boundary of the triangle.  Otherwise, the result is
    // undefined.
    //
    gstPoint convertToBarycentricCoord(
        const gstPoint &pt) const;

    //
    // Convert UV coordinate to 3D point.
    //
    gstPoint convertToCartesianCoord(
        const gstPoint2D &uvCoord) const;

    //
    // Converts 3D point coplaner with triangle to UV coordinates of triangle.  U is
    // along v1->v2 vector and V is along v1->v3 vector.  Undefined results for points
    // not coplaner with the triangle.
    //
    gstPoint2D convertToUVCoord(
        const gstPoint &pt) const;

    //
    // returns pointer to gstPolyPropertyContainer class storing polygonal
    // property information (ie. vertex normals...) pertaining to this 
    // gstTriPoly.
    gstPolyPropertyContainer &getPropertyContainer() {
        return d_properties;
    }

    //
    // Returns normal vector of Triangle.
    //
    inline virtual gstVector interpolateNormal(const gstPoint &barycentricCoord);
    
    //
    // Returns pointer to edge1 (v1->v2).
    //
    inline gstEdge * e1() const;

    //
    // Returns pointer to edge2 (v2->v3).
    //
    inline gstEdge * e2() const;

    //
    // Returns pointer to edge3 (v3->v1).
    //
    inline gstEdge * e3() const;

    //
    // Returns gstBoundingBox just enclosing this triangle.
    //
    virtual gstBoundingBox getBoundingBox();

    //
    // Returns pointer to edge1 (v1->v2).
    //
    inline gstEdge * getE1() const;

    //
    // Returns pointer to edge2 (v2->v3).
    //
    inline gstEdge * getE2() const;

    //
    // Returns pointer to edge3 (v3->v1).
    //
    inline gstEdge * getE3() const;

    //
    // Returns unique id of this object.  This id is created at object
    // construction.
    //
    inline gstPolyKey getKey() const;

    //
    // Returns pointer to gstTriPoly cooresponding to correct index.  Index '0' refers
    // to this poly.  Indices '1', '2', and '3' refer to the tri poly on the opposite
    // side of edge1, edge2, or edge3 respectively.  If no poly exists for the given
    // index NULL is returned.
    //
    gstTriPoly * getPoly(
        unsigned int index);

    //
    // Returns pointer to parent gstTriPolyMesh object. 
    //
    inline gstTriPolyMeshBase * getTriPolyMesh() const;

    //
    // Returns pointer to v1.
    //
    inline gstVertex * getV1() const;

    //
    // Returns pointer to v2.
    //
    inline gstVertex * getV2() const;

    //
    // Returns pointer to v3.
    //
    inline gstVertex * getV3() const;

    //
    // Returns TRUE if pt is coplaner to this triangle and is within boundary of
    // triangle. If pt is coplaner and outside the boundary of this triangle FALSE is
    // returned.  The behavior of this method is undefined if pt is not coplaner with
    // this triangle.
    //
    gstBoolean inside(
        const gstPoint &pt) const;

    //
    // Intersects gstTriPoly with gstBoundingCube object and returns
    // gstObjectObjectIntersection::intersectionType which can be; RV_OVERLAPPING or
    // RV_NONE. 'RV_OVERLAPPING' specifies that the cube encloses or overlaps the
    // triangle, and 'RV_NONE' specifies that the cube and triangle occupy separate
    // space. This method does not distinguish that the cube completely encloses the
    // triangle.
    //
    virtual gstObjectIntersectionInfo::IntersectionType intersect_BC(
        const gstBoundingCube &cube);

    //
    // Intersects gstTriPoly with gstBoundingCube object and returns
    // gstObjectObjectIntersection::intersectionType which can be; RV_OVERLAPPING or
    // RV_NONE. 'RV_OVERLAPPING' specifies that the cube encloses or overlaps the
    // triangle, and 'RV_NONE' specifies that the cube and triangle occupy separate
    // space. This method does not distinguish that the cube completely encloses the
    // triangle.
    //
    virtual gstObjectIntersectionInfo::IntersectionType intersect_BS(
        const gstBoundingSphere &sphere);

    //
    // lineSeg is a line segment co-planer to this gstTriPoly face.  The intersection
    // of the line segment may have 0, 1, or 2 intersections along its length with the
    // vertices or edges of the gstTriPoly.  TRUE is returned if there are > 0
    // intersections.  Otherwise FALSE is returned.  Information about the
    // intersections is stored in intersectionData.
    //
    gstBoolean intersectCoplanar_LS_PE(
        const gstLineSegment &lineSeg,
        gstLineIntersectionInfoFirstTwo_ParamEdge &intersectionData);

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
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Called when 'this' gstTriPoly object has been modified.
    //
    void modify();

    //
    // Called when 'this' gstTriPoly object is going to be modified.
    //
    gstBoolean beginModify();

    //
    // Called when 'this' gstTriPoly object is done being modified.
    //
    gstBoolean endModify();

  private:

    gstPolyKey key; // unique Id for triPoly instance

    gstVertex *v1;    // Vertex 1

    gstVertex *v2;    // Vertex 1

    gstVertex *v3;    // Vertex 1

    gstPolyPropertyContainer d_properties;    // polygon properties container(ie. texture coords)

    vector<gstIncidentEdge> edges;    // Vector of edges 1,2, and 3

    gstTriPolyMeshBase *d_triMesh;    // Pointer to parent gstTriPolyMeshBase object

    void recalcPlaneEquation();    // Recalculates normal vector of plane
};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<struct _Hashtable_node<struct pair<int const,class gstTriPoly *> > *, class __default_alloc_template<1,0>  >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<struct _Hashtable_node<struct pair<int const,class gstTriPoly *> > *, class __default_alloc_template<1,0>  >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Select1st<struct pair<int const, class gstTriPoly *> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT select1st<struct pair<int const, class gstTriPoly *> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT equal_to<int>;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT hashtable<struct pair<int const ,class gstTriPoly *>,int,hash<int>,struct _Select1st<struct pair<int const ,class gstTriPoly *> >,struct equal_to<int>,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT hash_map<int, gstTriPoly *, hash<int>, equal_to<int>, class __default_alloc_template<1,0>  >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstTriPoly *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstTriPoly *, class __default_alloc_template<1,0> >;
#endif

// equality operator struct define for ints
// for use by template code
struct eqint
{
    bool operator()(const int i1, const int i2) const
    {
        return i1==i2;
    }
};

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
    typedef vector<gstTriPoly *, class __default_alloc_template<1,0> > gstTriPolyPtrVector;
    typedef hash_map<int, gstTriPoly *, hash<int>, equal_to<int>, class __default_alloc_template<1,0> > TriPolyMap;
    typedef list<gstTriPoly *, class __default_alloc_template<1,0> > gstTriPolyPtrList;
#else   // Not debug or Not building freeform
    typedef vector<gstTriPoly*> gstTriPolyPtrVector;
    typedef hash_map<int, gstTriPoly *, hash<int>, equal_to<int> > TriPolyMap;
    typedef list<gstTriPoly *> gstTriPolyPtrList;
#endif  // _DEBUG && _BUILDING_FREEFORM

typedef gstTriPolyPtrList::iterator gstTriPolyPtrListIterator;
typedef gstTriPolyPtrList::const_iterator gstTriPolyPtrListConstIterator;

typedef TriPolyMap::iterator gstTriPolyPtrHashMapIterator;
typedef TriPolyMap::const_iterator gstTriPolyPtrHashMapConstIterator;

inline gstSpatialObject * gstTriPoly::clone() const
{
    return(cloneTriPoly());
}


inline gstTriPoly * gstTriPoly::cloneTriPoly() const
{
    return(new gstTriPoly(*this));
}


inline gstEdge * gstTriPoly::e1() const
{
    return(edges[0].getEdge());
}


inline gstEdge * gstTriPoly::e2() const
{
    return(edges[1].getEdge());
}


inline gstEdge * gstTriPoly::e3() const
{
    return(edges[2].getEdge());
}


inline gstEdge * gstTriPoly::getE1() const
{
    return(edges[0].getEdge());
}


inline gstEdge * gstTriPoly::getE2() const
{
    return(edges[1].getEdge());
}


inline gstEdge * gstTriPoly::getE3() const
{
    return(edges[2].getEdge());
}


inline gstPolyKey gstTriPoly::getKey() const
{
    return(key);
}


inline gstTriPolyMeshBase * gstTriPoly::getTriPolyMesh() const
{
    return(d_triMesh);
}



inline gstVertex * gstTriPoly::getV1() const
{
    return(v1);
}


inline gstVertex * gstTriPoly::getV2() const
{
    return(v2);
}


inline gstVertex * gstTriPoly::getV3() const
{
    return(v3);
}



#endif  // GSTTRIPOLY_INCLUDED
