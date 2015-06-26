//=============================================================================
//   Filename : gstPolyStructs.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr, Loren Shih
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Deprecated - use gstTriPolyMesh instead
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_POLYMESHHELPER
#define GST_POLYMESHHELPER

#include <malloc.h>
#include <gstErrorHandler.h>
#include <gstShape.h>

// Tri-mesh object list structure
typedef struct _PolygonList {
    int polygonIndex;
    struct _PolygonList *next;
} PolygonList;

// Tri-mesh vertex structure
typedef struct _Vertex {
    gstPoint point;
    PolygonList *polygonList;
} Vertex;

// Tri-mesh edge structure
typedef struct _Edge {
    Vertex *vertex1;
    Vertex *vertex2;
    struct _Polygon *face1;
    struct _Polygon *face2;
} Edge;

// Tri-mesh structure
typedef struct _Polygon {
    Edge *edge1;
    Edge *edge2;
    Edge *edge3;
    gstVector normal;
    double d;
    int index;
    int vertices[3]; 
    gstVector verticesN[3];
} gstPolygon;

#endif
