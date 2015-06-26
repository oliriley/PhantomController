//=============================================================================
//   Filename : gstSpatialObject.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Base class for 3D object in space, for gstTriPolyMesh, etc.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTSPATIALOBJECT_INCLUDED)
#define GSTSPATIALOBJECT_INCLUDED

#include <gstDllExport.h>

#include <gstObjectIntersectionInfo.h>
#include <gstLineIntersectionInfoFirst_Param.h>
#include <gstLineIntersectionInfoFirst_ParamSpatObj.h>
#include <gstLine.h>
#include <gstRay.h>
#include <gstLineSegment.h>
#include <gstBoundingBox.h>
#include <gstBoundingCube.h>

#include <vector.h>
#include <set.h>

class gstObjectIntersectionInfo_SpatialObjectPtrSet;
class gstSpatialObject;

#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT less< gstSpatialObject *>;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Rb_tree_base<class gstSpatialObject *,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Rb_tree_base<class gstSpatialObject *,class __default_alloc_template<0,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Rb_tree<class gstSpatialObject *,class gstSpatialObject *,struct identity<class gstSpatialObject *>,struct less<class gstSpatialObject *>,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Rb_tree<class gstSpatialObject *,class gstSpatialObject *,struct identity<class gstSpatialObject *>,struct less<class gstSpatialObject *>,class __default_alloc_template<0,0> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Rb_tree_base<class gstSpatialObject *,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Rb_tree_base<class gstSpatialObject *,class __default_alloc_template<0,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Rb_tree<class gstSpatialObject *,class gstSpatialObject *,struct _Identity<class gstSpatialObject *>,struct less<class gstSpatialObject *>,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Rb_tree<class gstSpatialObject *,class gstSpatialObject *,struct _Identity<class gstSpatialObject *>,struct less<class gstSpatialObject *>,class __default_alloc_template<0,0> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT rb_tree<gstSpatialObject *, gstSpatialObject *, identity<gstSpatialObject *>, less< gstSpatialObject *>,class __default_alloc_template<0,0>  >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT set<gstSpatialObject *, less< gstSpatialObject *>, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstSpatialObject *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstSpatialObject *, class __default_alloc_template<1,0> >;
#endif

// In debug builds, FreeForm redefines the __STD_DEFAULT_ALLOCATOR.  Ghost explicelty
// exports it's templates using __default_alloc_template.  Therefore, we have to 
// explicitely specify the allocator type in debug builds because the the template
// generated types won't match those exported by ghost.  This is only an issue in this
// case becuase gstTriPolyMeshHaptic exposes a direct reference to one of these arrays.
// In Release
// Even though the allocator types match, we can't explicitely expose it because
// MSDEV will treat the explicit definition as an error.
// C2908: explicit specialization; '__default_alloc_template<1,0>' has already been 
// specialized from the primary template. 
//
// AT BEST, I THINK THIS SHOULD BE A WARNING CLASS DIAGNOSTIC!!
//
#if defined( _DEBUG ) && defined( _BUILDING_FREEFORM )
    typedef vector<gstSpatialObject *, class __default_alloc_template<1,0> > gstSpatialObjectPtrVector;
#else
    typedef vector<gstSpatialObject*> gstSpatialObjectPtrVector;
#endif  // _DEBUG && _BUILDING_FREEFORM

//
// Base class intended for all GHOST objects of spatial extent.
// This class allows access to functionality common to such classes. 
class GHOST_DLL_IMPORT_EXPORT gstSpatialObject
{
  public:

    virtual ~gstSpatialObject();

    // Return a copy of this object as a gstSpatialObject.
    inline virtual gstSpatialObject * clone() const;

    // Return a copy of this object as a gstSpatialObject.
    inline gstSpatialObject * cloneSpatialObject() const;

    // Return the bounding box of the object.
    virtual gstBoundingBox getBoundingBox();

    // return the bounding sphere of the object.
    virtual gstBoundingSphere getBoundingSphere();

    //
    // GHOST_IGNORE_IN_DOC:
    // Get type of this class.  No instance needed.
    //
    static gstType getClassTypeId()
    {
        gstAssignUniqueId(gstSpatialObjectClassTypeId);
        return gstSpatialObjectClassTypeId;
    }

    // Gather the collection of objects contained within this object.
    // By default this returns FALSE indicating that there is no collection.
    virtual gstBoolean getContainedObjects(
        gstSpatialObjectPtrVector &);

    //
    // GHOST_IGNORE_IN_DOC:
    // Get type of this instance.
    //
    inline virtual gstType getTypeId() const;

    //
    // Intersects gstSimpleCube with spatial object and returns
    // gstIntersection::intersectionType which can be; enclosed, enclosing,
    // overlapping, or none. 'enclosed' specifies that the simple cube is enclosed
    // within the object, 'enclosing' specifies that the simple cube encloses the
    // spatial object, 'overlapping' specifies that the simple cube and the spatial
    // object overlap eachother, and 'none' specifies that the simple cube and spatial
    // object occupy separate space.
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
        gstObjectIntersectionInfo_SpatialObjectPtrSet &intInfo);

    //
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersect_LS_P(
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
    virtual gstLineIntersectionInfo::IntersectionType intersect_LS_PSO(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersect_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersect_Ray_PSO(
        const gstRay &ray,
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
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_Ray_PSO(
        const gstRay &ray,
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
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_Ray_PSO(
        const gstRay &ray,
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
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo);

    //
    // Intersects gstRay with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, or none.  'in'
    // specifies an intersection into the object, 'out' specifies an intersection
    // leaving the object, 'inOut' specifies intersections entering and leaving the
    // object, and 'none' specifies that the object is not intersected.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_Ray_PSO(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // GHOST_IGNORE_IN_DOC:
    // Returns TRUE this class is same or derived class of type.
    //
    inline virtual gstBoolean isOfType(
        gstType type) const;

    //
    // GHOST_IGNORE_IN_DOC:
    // Returns TRUE if subclass is of type.
    //
    inline static gstBoolean staticIsOfType(
        gstType type);

protected:
    // This class is intended as a base class only, the constructor
    // is protected so that instances can not be created.
    gstSpatialObject();

  private:


    static gstType gstSpatialObjectClassTypeId;
};

inline gstSpatialObject * gstSpatialObject::clone() const
{
    return(cloneSpatialObject());
}

inline gstSpatialObject * gstSpatialObject::cloneSpatialObject() const
{
    return(new gstSpatialObject(*this));
}

inline gstType gstSpatialObject::getTypeId() const
{
    return getClassTypeId();
}

inline gstBoolean gstSpatialObject::isOfType(
    gstType type) const
{
    return staticIsOfType(type);
}

inline gstBoolean gstSpatialObject::staticIsOfType(
    gstType type)
{
    if (type == getClassTypeId())
        return TRUE;
    else
        return FALSE;
}


#endif  // GSTSPATIALOBJECT_INCLUDED
