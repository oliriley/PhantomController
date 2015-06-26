//=============================================================================
//   Filename : gstVertex.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Represents a point in 3d space, with incident edge information
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTVERTEX_INCLUDED)
#define GSTVERTEX_INCLUDED



#include <gstDllExport.h>

#include <gstEdge.h>
#include <gstPoint.h>
#include <gstIncidentEdge.h>

#include <list>
#include <pair.h>
#include <hash_map.h>

typedef int gstVertexKey;

//
// Mesh element representing a position in space with incident
// gstEdges directed into and out of the vertex.
//
class GHOST_DLL_IMPORT_EXPORT gstVertex:
    public gstPoint
{

    friend class gstEdge;

  public:

    typedef list<gstIncidentEdge, __default_alloc_template<1,0> >::iterator incidentEdgeIterator;

    gstVertex(
        const gstPoint &_position,
        const gstVertexKey _key=NULL);

    virtual ~gstVertex();

    //
    // Returns unique key identifying this vertex.
    //
    inline gstVertexKey getKey() const;

    //
    // Returns the number of gstEdge elements incident upon this vertex.
    //
    inline int getNumIncidentEdges() const;

    //
    // Returns iterator positioned at the beginning of a list of incident gstEdges
    // upon this vertex.
    //
    inline gstIncidentEdgeListConstIterator incidentEdgesBegin() const;

    //
    // Returns iterator positioned at the ending of a list of incident gstEdges
    // upon this vertex.
    //
    inline gstIncidentEdgeListConstIterator incidentEdgesEnd() const;

    //
    // Returns TRUE if no edges are incident upon vertex.
    //
    inline gstBoolean isStranded() const;

  protected:

    //
    // Adds edge to list of incident edges along with direction of edge into or out of
    // vertex.
    //
    // NOTE: gstEdge::gstEdge should be the only place this method is called from.
    //
    void addIncidentEdge(
        gstEdge *edge);

    //
    // Removes edge edgeToRemove from this gstVertex.  If edgeToRemove was not incident
    // then FALSE is returned.  If edgeToRemove was the only incident edge then
    // gstVertex::STRANDED is returned.  Otherwise, TRUE is returned.
    //
    gstBoolean removeIncidentEdge(
        gstEdge *edgeToRemove);

  private:

    list<gstIncidentEdge, __default_alloc_template<1,0> > incidentEdges;

    gstVertexKey key;

    typedef enum returnValues_ {RV_STRANDED} ReturnValues;

    //
    // Not Implemented
    //
    gstVertex(
        const double x,
        const double y,
        const double z);

    //
    // Not Implemented
    //
    gstVertex();

    //
    // Not Implemented
    //
    gstVertex(
        const double *pos);

    void setKey(
        const gstVertexKey _key);
};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base< struct _Hashtable_node<struct pair<int const,class gstVertex *> > *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector< struct _Hashtable_node<struct pair<int const,class gstVertex *> > *, class __default_alloc_template<1,0>  >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT _Select1st<struct pair<int const, class gstVertex *> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT select1st<struct pair<int const, class gstVertex *> >;
GHOST_EXPIMP_TEMPLATE template struct GHOST_DLL_IMPORT_EXPORT equal_to<int>;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT hashtable<struct pair<int const ,class gstVertex *>,int,hash<int>,struct _Select1st<struct pair<int const ,class gstVertex *> >,struct equal_to<int>,class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT hash_map<int, gstVertex *, hash<int>, equal_to<int>, class __default_alloc_template<1,0>  >;
#endif


inline gstVertexKey gstVertex::getKey() const
{
    return(key);
}


inline int gstVertex::getNumIncidentEdges() const
{
    return(incidentEdges.size());
}


inline gstIncidentEdgeListConstIterator gstVertex::incidentEdgesBegin() const
{
    return incidentEdges.begin();
}


inline gstIncidentEdgeListConstIterator gstVertex::incidentEdgesEnd() const
{
    return incidentEdges.end();
}


inline gstBoolean gstVertex::isStranded() const
{
    return(getNumIncidentEdges() == 0);
}


#endif  // GSTVERTEX_INCLUDED
