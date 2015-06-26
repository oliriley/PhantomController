//=============================================================================
//   Filename : gstTriPolyMeshHaptic.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Triangle mesh scene graph node, uses a gstTriPolyMesh as the
//              triangle data.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTTRIPOLYMESHHAPTIC_INCLUDED)
#define GSTTRIPOLYMESHHAPTIC_INCLUDED

#include <gstDllExport.h> 

#include <gstShape.h>
#include <gstPHANToM.h>
#include <gstModifyBase.h>
#include <gstTriPolyMesh.h>
#include <gstLineIntersectionInfoFirst_ParamNormal.h>
#include <gstLineIntersectionInfoFirst_ParamSpatObj.h>

#include <vector.h>

//
// Triangle polygon scene graph node.  Represents a triangular polygon mesh that is
// haptically palpable.
//
class GHOST_DLL_IMPORT_EXPORT gstTriPolyMeshHaptic :
    public gstShape, public gstModifyBase
{
    GHOST_INHERIT_HEADER(gstTriPolyMeshHaptic, gstShape);

  public:

    typedef enum resultValues { RV_NONE, RV_FACE, RV_EDGE, RV_CORNER} ReturnValues;

    typedef enum TouchableFrom_ { RV_FRONT, RV_BACK, RV_FRONT_AND_BACK} TouchableFrom;

    //
    // Defined new gstTriPolysH object with empty polygon mesh
    //
    gstTriPolyMeshHaptic();


    gstTriPolyMeshHaptic(
        const gstTriPolyMeshHaptic &mesh);


    gstTriPolyMeshHaptic(
        gstTriPolyMesh *triMesh);


    virtual ~gstTriPolyMeshHaptic();

    //
    // virtual method to copy this object and return base class
    // pointer to newly copied object.  Allows copy of object to be
    // made using base class pointer.
    //
    inline virtual gstNode * Clone() const;

    //
    // virtual method to copy this gstTriPolyMeshHaptic object and return 
    // gstTriPolyMeshHaptic pointer to newly cloned object.  
    inline gstTriPolyMeshHaptic * CloneTriPolyMeshHaptic() const;

    //
    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Returns TRUE if the PHANToM is currently in contact with this object.
    // If so, the collision is added to the PHANToM's list through
    // gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
    //
    virtual gstBoolean collisionDetect(
        gstPHANToM *phantomNode);

    //
    // Returns TRUE if normal interpolation smoothing is to be used.
    // 
    // Note: Normal polygon property must be specified at vertices for this
    // to take effect.
    // 
    inline gstBoolean getSmoothing() const { return d_smoothing; }

    //
    // If smoothing is TRUE normal interpolation smoothing is to be used.
    // 
    // Note: Normal polygon property must be specified at vertices for this
    // to take effect.
    // 
    inline void setSmoothing(gstBoolean smoothing) { d_smoothing = smoothing; }

    //
    // Returns RV_FRONT, RV_BACK, or RV_FRONT_AND_BACK to denote which
    // sides of the triangles will be touchable by the PHANToM.
    // 
    inline gstTriPolyMeshHaptic::TouchableFrom getTouchableFrom() const;

    //
    // Returns a vector of pointers to gstTriPoly.  This vector
    // will contain 0, 1, 2, or 3 values if there was no, face,
    // edge, or corner contact respectively.  For edge and corner 
    // contact the 2nd and 3rd values indicate the polys defining 
    // the edge and corner respectively.
    // 
    inline const gstTriPolyPtrVector & getTouchedPolys() const;

    //
    // Returns a pointer to the gstTriPolyMesh storing the polygonal
    // mesh for this object.  If none has been set yet then NULL is
    // returned.
    //
    inline gstTriPolyMeshBase * getTriPolyMesh() const;

    virtual gstBoolean intersect_LS_PN(
        gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamNormal &intersectionInfo);


    virtual gstBoolean intersect_LS_PN_WC(
        gstLineSegment &lineSegment_WC,
        gstLineIntersectionInfoFirst_ParamNormal &intersectionInfo_WC);

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
    // Intersects gstLineSegment with spatial object and returns
    // gstIntersection::intersectionType which can be; in, out, inOut, none_inside, or
    // none_outside. 'in' specifies an intersection into the object, 'out' specifies an
    // intersection leaving the object, 'inOut' specifies intersections entering and
    // leaving the object, 'none_insdie' specifies that the object is not intersected
    // and the line segment is within the object, and 'none_outside' specifies that
    // there is no intersection and the line segment is outside of the object.
    //
    virtual gstLineIntersectionInfo::IntersectionType intersectCacheFirstIn_LS_PSO(
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
    virtual gstLineIntersectionInfo::IntersectionType intersectCacheFirstInOut_LS_PSO(
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
    virtual gstLineIntersectionInfo::IntersectionType intersectCacheFirstOut_LS_PSO(
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
    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_PSO(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

    //
    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Returns TRUE if the line segment, defined by startPt_WC and endPt_WC
    // in the world coordinate system, intersects the shape object.
    // If TRUE, intersectionPt_WC is set to the point of intersection and
    // intersectionNormal_WC is set to the surface normal at the
    // intersection point.
    //
    virtual gstBoolean intersection(
        const gstPoint &startPt_WC,
        const gstPoint &endPt_WC,
        gstPoint &intersectionPt_WC,
        gstVector &intersectionNormal_WC,
        void **data);


    virtual void scale(const gstPoint &_scale);
    
    virtual void scale(double s)
    {
        scale(gstPoint(s,s,s));
    }
    
    virtual void scale(double x, double y, double z)
    {
        scale(gstPoint(x,y,z));
    }
    
    
    virtual void setCenter(
        const gstPoint &_center);
    
    
    virtual void setScale(
        const gstPoint &_scale);
    
    virtual void setScale(double scale)
    {
        setScale(gstPoint(scale,scale,scale));
    }
    
    virtual void setScale(double x, double y, double z)
    {
        setScale(gstPoint(x,y,z));
    }

    //
    // Sets touchable sides of triangles to value of touchableFrom.  
    // The possible values are RV_FRONT, RV_BACK, or RV_FRONT_AND_BACK.
    // 
    inline void setTouchableFrom(
        TouchableFrom touchableFrom);

    //
    // Sets newTriMesh as the gstTriPolyMesh storing the polygonal
    // mesh for this object.
    //
    inline void setTriPolyMesh(
        gstTriPolyMeshBase *newTriMesh);

    //
    // Signifies that modifications to this object have occured and
    // any calculated state must be updated.
    //
    virtual void modified() {
        updateBoundingShape();
    }

    //
    // Updates boundingObject based on current state of polygonal mesh.
    //
    void updateBoundingShape();

  private:

    // gstTriPolyMeshBase object storing polygonal mesh
    gstTriPolyMeshBase *d_triMesh;    

    // TRUE if normals are to be interpolated based on vertex normals
    gstBoolean d_smoothing;

    static const double LINE_BACKUP_FACTOR;

    // Indicates if front and/or back of triangle is touchable
    TouchableFrom d_touchableFrom;    

    // Stores polygons contacted currently
    gstTriPolyPtrVector d_touchedPolys;    
};


inline gstNode * gstTriPolyMeshHaptic::Clone() const
{
    return(CloneTriPolyMeshHaptic());
}


inline gstTriPolyMeshHaptic *
gstTriPolyMeshHaptic::CloneTriPolyMeshHaptic() const
{
    return(new gstTriPolyMeshHaptic(*this));
}


inline gstTriPolyMeshHaptic::TouchableFrom
gstTriPolyMeshHaptic::getTouchableFrom() const
{
    return(d_touchableFrom);
}


inline const gstTriPolyPtrVector &
gstTriPolyMeshHaptic::getTouchedPolys() const
{
    return(d_touchedPolys);
}


inline gstTriPolyMeshBase *
gstTriPolyMeshHaptic::getTriPolyMesh() const
{
    return(d_triMesh);
}

inline void gstTriPolyMeshHaptic::setTouchableFrom(
    TouchableFrom touchableFrom)
{
    d_touchableFrom = touchableFrom;
}


inline void gstTriPolyMeshHaptic::setTriPolyMesh(
    gstTriPolyMeshBase *newTriMesh)
{
    if (newTriMesh == NULL)
        return;

    d_triMesh = newTriMesh;
    d_triMesh->setOwner(this);
}


#endif  // GSTTRIPOLYMESHHAPTIC_INCLUDED
