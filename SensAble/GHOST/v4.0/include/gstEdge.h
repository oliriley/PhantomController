//=============================================================================
//   Filename : gstEdge.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Used by TriPolyMesh family of classes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTEDGE_INCLUDED)
#define GSTEDGE_INCLUDED

#include <gstDllExport.h>

#include <list>
#include <gstLineSegment.h>

class gstVertex;
class gstTriPoly;

#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstTriPoly *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstTriPoly *, class __default_alloc_template<1,0> >;
#endif


typedef list<gstTriPoly *>::iterator gstTriPolyPtrListIterator;
typedef list<gstTriPoly *>::const_iterator gstTriPolyPtrListConstIterator;


//
// Mesh element representing an edge of one or more polygonal faces
// with two gstVertex elements at it's end points (v1 and v2). 
//
class GHOST_DLL_IMPORT_EXPORT gstEdge
{
    friend class gstTriPoly;

  public:

    gstEdge(
        gstVertex *v1new,
        gstVertex *v2new,
        gstTriPoly *p1=NULL,
        gstTriPoly *p2=NULL);

    virtual ~gstEdge();

    //
    // Returns a gstLineSegment with p1 and p2 of the line segment
    // coincident to v1 and v2 of this edge.
    //
    gstLineSegment getLineSegment() const;

    //
    // Returns number of polygons sharing this edge
    //
    inline int getNumIncidentPolys() const;

    //
    // Returns the first incident poly to this edge from an unordered list.
    // If there are no incident polygons to this edge, then NULL is returned.
    //
    inline gstTriPoly * getP1() const;

    //
    // Returns the second incident poly to this edge from an unordered list.
    // If there are less than 2 incident polygons to this edge, then NULL is 
    // returned.
    //
    inline gstTriPoly * getP2() const;

    //
    // Returns a pointer to gstVertex v1.
    //
    inline gstVertex * getV1() const;

    //
    // Returns a pointer to gstVertex v2.
    //
    inline gstVertex * getV2() const;

    //
    // Returns an iterator at the beginning position of a list of the
    // incident polygons to this edge.
    //
    inline gstTriPolyPtrListConstIterator incidentPolysBegin() const;

    //
    // Returns an iterator at the ending position of a list of the
    // incident polygons to this edge.
    //
    inline gstTriPolyPtrListConstIterator incidentPolysEnd() const;

    //
    // Returns TRUE if the number of incident polygons is 0.
    //
    inline gstBoolean isStranded() const;

    //
    // Projects pt onto edge and returns projection in projectedPt.  If projectedPt
    // lies to the right of v1 then gstEdge::LEFT is returned.  If projectedPt lies in
    // between v1 and v2 then gstEdge::BETWEEN is returned.  Otherwise, gstEdge::RIGHT
    // is returned.
    //
    int project(
        const gstPoint &pt,
        gstPoint *projectedPt=NULL) const;

    //
    // Returns a pointer to gstVertex v1.
    //
    inline gstVertex * v1() const;

    //
    // Returns a pointer to gstVertex v2.
    //
    inline gstVertex * v2() const;

  protected:

    //
    // Adds poly to the list of incident polygons for this edge.
    //
    void addIncidentPoly(
        gstTriPoly *poly);

    //
    // Removes polyToRemove from list of incident polys to edge.  If there are no more
    // incident polys left then gstEdge::STRANDED is returned.  If the poly wasn't
    // incident to this then FALSE is returned.  Otherwise, TRUE is returned.
    //
    int removeIncidentPoly(
        const gstTriPoly *polyToRemove);

  private:


    gstVertex * v1_;

    gstVertex * v2_;

    //
    // Only polys incident along entire edge.
    //
    list<gstTriPoly *> incidentPolys;

    typedef enum returnValues_ { RV_RIGHT, RV_MIDDLE, RV_LEFT, RV_STRANDED } ReturnValues;
};



inline int gstEdge::getNumIncidentPolys() const
{
    return(incidentPolys.size());
}


inline gstTriPoly * gstEdge::getP1() const
{
    if (incidentPolys.size() > 0)
        return(incidentPolys.front());
    else {
        return(NULL);
    
    }
}


inline gstTriPoly * gstEdge::getP2() const
{
    if (incidentPolys.size() > 1) {
        list<gstTriPoly *>::const_iterator iter = incidentPolys.begin();
        iter++;
        return(*iter);
    } else {
        return(NULL);
    }
}


inline gstVertex * gstEdge::getV1() const
{
    return(v1_);
}


inline gstVertex * gstEdge::getV2() const
{
    return(v2_);
}


inline gstTriPolyPtrListConstIterator gstEdge::incidentPolysBegin() const
{
    return incidentPolys.begin();
}


inline gstTriPolyPtrListConstIterator gstEdge::incidentPolysEnd() const
{
    return incidentPolys.end();
}


inline gstBoolean gstEdge::isStranded() const
{
    return(getNumIncidentPolys() == 0);
}


inline gstVertex * gstEdge::v1() const
{
    return(v1_);
}


inline gstVertex * gstEdge::v2() const
{
    return(v2_);
}


#endif  // GSTEDGE_INCLUDED
