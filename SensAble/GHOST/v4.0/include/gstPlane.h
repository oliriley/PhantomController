//=============================================================================
//   Filename : gstPlane.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Represents a geometric plane
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPLANE_INCLUDED)
#define GSTPLANE_INCLUDED

#include <gstDllExport.h>
#include <gstLineIntersectionInfo.h>
#include <stdio.h>
#include <gstTypedefs.h>
#include <gstVector.h>
#include <gstSpatialObject.h>

class gstLineIntersectionInfoFirst_Param;
class gstLineSegment;
class gstRay;

// Represents a geometric plane
class GHOST_DLL_IMPORT_EXPORT gstPlane : public gstSpatialObject
{
    GHOST_INHERIT_HEADER(gstPlane, gstSpatialObject);

  public:

    //
    // Constructor.
    //
    inline gstPlane(
        double a = 0.0,
        double b = 1.0,
        double c = 0.0,
        double d = 0.0);

    //
    // Constructor.
    //
    inline gstPlane(
        const gstPlane &plane);

    //
    // Constructor.
    //
    inline gstPlane(
        const gstPlane *plane);

    //
    // Constructor.
    //
    gstPlane(
        const gstPoint &,
        const gstPoint &,
        const gstPoint &);

    //
    // Constructor.
    //
    gstPlane(
        const gstVector &,
        double);

    gstPlane(
        const gstVector &,
        const gstPoint &);


    virtual ~gstPlane();

    //
    // comparison
    //
    bool operator==(const gstPlane& rhs) const;

    //
    // Returns A coefficient from plane equation.
    //
    inline double a() const;

    //
    // Returns B coefficient from plane equation.
    //
    inline double b() const;

    //
    // Returns C coefficient from plane equation.
    //
    inline double c() const;


    inline virtual gstSpatialObject * clone() const;

    inline gstPlane * clonePlane() const;

    //
    // Returns D coefficient from plane equation.
    //
    inline double d() const;

    //
    // Returns ax + by + cz + d.
    //
    inline double error(
        const gstPoint &pt) const;


    virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo) const;


    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo) const;


    virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_Ray_P(
        const gstRay &ray,
        gstLineIntersectionInfoFirst_Param &intersectionInfo) const;


    virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_P(
        const gstLineSegment &lineSegment,
        gstLineIntersectionInfoFirst_Param &intersectionInfo) const;

    //
    // Returns normal vector of plane.
    //
    inline virtual gstVector normal() const;

    //
    // Returns some point on the plane
    //
    inline gstPoint pointOnPlane() const;

    //
    // Returns vector perpendicular to point p.
    //
    inline gstVector perpVec(
        const gstPoint &pt) const;

    //
    // Given line passing through p1 and p2, intersects plane.
    // interection is set to point of intersection with line
    // and TRUE is returned.  If the line does not intersect, returns
    // FALSE.
    //
    inline gstPoint pointOfLineIntersection(
        const gstPoint &p1,
        const gstPoint &p2) const;

    //
    // Given line through p1 and p2, returns TRUE if line intersects plane
    // with point of intersection in intersection argument.  FALSE otherwise.
    //
    gstBoolean pointOfLineIntersection(
        const gstPoint &point1,
        const gstPoint &point2,
        gstPoint &intersection) const;

    //
    // Given endpoints of line segment, p1 and p2, returns TRUE if line segment intersects
    // plane with point of intersection in intersection argument.  The plane is considered
    // one-sided, in the direction of the normal; if the segment is in the same direction
    // as the normal, no intersection will be returned.  Returns FALSE if there is no
    // intersection.
    //
    gstBoolean pointOfSegmentIntersection(
        const gstPoint &point1,
        const gstPoint &point2,
        gstPoint &intersection) const;

    // Find intersection between a given gstPlane and this one.
    // The resulting intersection is returned as a gstRay, if one exists.
    // Tol is acceptable distance for parallel planes to be 
    // considered as coincident. Return value is:
    //   1 - unique intersection as a line, returned as a gstRay
    //   0 - planes are coincident (no gstRay returned)
    //  -1 - planes don't intersect (no gstRay returned)
    int intersectPlane(const gstPlane& interPlane,
                       gstRay& interLine,
                       double tol = GST_EPSILON) const;

    //
    // FOR_GHOST_INTERNAL:
    // Save to file.
    //
    inline void printSelf(
        FILE* outf) const;

    //
    // Print to stdout.
    //
    inline void printSelf2() const;

    //
    // Returns projection of point p onto plane.
    // 
    gstPoint project(
        const gstPoint &p,
        double offsetFactor = 1.00001) const;

    //
    // Returns projection of point p onto plane.
    //
    gstPoint projectPoint(
        const gstPoint &p) const;


    //
    // Returns projection of point p onto plane
    // along given vector.
    //
    bool projectPointAlongVector(
        const gstPoint& p, 
        const gstVector& v,
        gstPoint& resultPt) const;

    inline void setD(
        double _d);

    //
    // Set plane to be located at position indicated by normal and point.
    //
    inline void setPlane(
        const gstVector &newNormal,
        const gstPoint &newIntersection);

  protected:

    gstVector normalVector;

    double D;
};



inline gstPlane::gstPlane(
    double a,
    double b,
    double c,
    double d):

    // Protected data initialization
    normalVector(a, b, c),
    D(d)
{
        D /= normalVector.norm();
        normalVector.normalize();
}


inline gstPlane::gstPlane(
    const gstPlane &plane):

    // Protected data initialization
    normalVector(plane.normalVector),
    D(plane.D)
{

}


inline gstPlane::gstPlane(
    const gstPlane *plane):

    // Protected data initialization
    normalVector(plane->normalVector),
    D(plane->D)
{

}


inline double gstPlane::a() const
{
    return normalVector.x();
}


inline double gstPlane::b() const
{
    return normalVector.y();
}


inline double gstPlane::c() const
{
    return normalVector.z();
}


inline gstSpatialObject * gstPlane::clone() const
{
    return(clonePlane());
}


inline gstPlane * gstPlane::clonePlane() const
{
    return(new gstPlane(*this));
}


inline double gstPlane::d() const
{
    return D;
}


inline double gstPlane::error(
    const gstPoint &pt) const
{
        return normalVector.x()*pt.x() +
            normalVector.y()*pt.y() +
            normalVector.z()*pt.z() +
            D;
}


inline gstVector gstPlane::normal() const
{
    return normalVector;
}

inline gstPoint gstPlane::pointOnPlane() const
{
    gstPoint pt = -normal() * D;
    return pt;
}

inline gstVector gstPlane::perpVec(
    const gstPoint &pt) const
{
        return error(pt)*normalVector;
}


inline gstPoint gstPlane::pointOfLineIntersection(
    const gstPoint &p1,
    const gstPoint &p2) const
{
    gstPoint intersection;
    pointOfLineIntersection(p1,p2,intersection);
    return intersection;
}


inline void gstPlane::printSelf(
    FILE* outf) const
{
    fprintf(outf,"%.19g %.19g %.19g %.19g",
        normalVector.x(), normalVector.y(), normalVector.z(), D);
}


inline void gstPlane::printSelf2() const
{
    printf("{ %g %g %g %g }",
        normalVector.x(), normalVector.y(), normalVector.z(), D);
}


inline void gstPlane::setD(
    double _d)
{
    D = _d;
}


inline void gstPlane::setPlane(
    const gstVector &newNormal,
    const gstPoint &newIntersection)
{
        normalVector = newNormal;
        D = -(normalVector.x()*newIntersection.x() +
            normalVector.y()*newIntersection.y() +
            normalVector.z()*newIntersection.z());
}


#endif  // GSTPLANE_INCLUDED
