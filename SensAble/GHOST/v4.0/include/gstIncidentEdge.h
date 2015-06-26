//=============================================================================
//   Filename : gstIncidentEdge.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Used by gstTriPoly
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTINCIDENTEDGE_INCLUDED)
#define GSTINCIDENTEDGE_INCLUDED

#include <gstDllExport.h>
#include <list.h>
#include <vector.h>

class gstEdge;
class gstVertex;

// Type of edge used by gstTriPoly
class GHOST_DLL_IMPORT_EXPORT gstIncidentEdge
{
  public:

    typedef enum ReturnValues_
        {RV_IN, RV_OUT, RV_CLOCKWISE, RV_COUNTERCLOCKWISE} ReturnValues;


    gstIncidentEdge(
        gstEdge *_edge=NULL,
        const int _direction=gstIncidentEdge::RV_IN);


    inline int getDirection() const;


    inline gstEdge * getEdge() const;

    //
    // Returns pointer to gstVertex on opposite side of incident edge.
    //
    gstVertex * getOppositeVertex() const;

    //
    // Equality test operator.
    //
    inline gstBoolean operator ==(
        const gstIncidentEdge &e) const;

    //
    // Less than operator.
    //
    inline gstBoolean operator < ( const gstIncidentEdge &e) const;
    
    void setDirection(
        const int _direction);

    void setEdge(
        gstEdge *_edge);

  private:


    int direction;

    gstEdge * edge;
};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstIncidentEdge, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstIncidentEdge, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstIncidentEdge, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstIncidentEdge, __default_alloc_template<1,0> >;
#endif


typedef list<gstIncidentEdge, __default_alloc_template<1,0> >::iterator gstIncidentEdgeListIterator;
typedef list<gstIncidentEdge, __default_alloc_template<1,0> >::const_iterator gstIncidentEdgeListConstIterator;


inline int gstIncidentEdge::getDirection() const
{
    return(direction);
}

inline gstEdge * gstIncidentEdge::getEdge() const
{
    return(edge);
}

inline gstBoolean gstIncidentEdge::operator ==(
    const gstIncidentEdge &e) const
{
    return((direction == e.direction) && (edge == e.edge));
}

inline gstBoolean gstIncidentEdge::operator <(
    const gstIncidentEdge &e) const
{
    return((edge < e.edge));
}

#endif  // GSTINCIDENTEDGE_INCLUDED
