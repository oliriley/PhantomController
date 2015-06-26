//=============================================================================
//   Filename : gstBSP.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Deprecated - use gstTriPolyMesh and gstSpatialPartion instead
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BSP
#define GST_BSP

#include <gstPolyStructs.h>

//
// Supporting Data Structures 
//

typedef struct Point3Struct {   /* 3d point */
        double x, y, z;
        } Point3;
typedef Point3 Vector3;


typedef struct _GeomObj{
     Point3   min, max;        /* extent of the primitive */
	 _GeomObj *next;
	 gstPolygon  *poly;
} GeomObj, *GeomObjPtr;


typedef struct _GeomObjList {
    /* Link list of primitives */
	GeomObjPtr objs;
    int  length;                /* Length of the link list */
} GeomObjList;


typedef struct {
       Point3     origin;       /* ray origin */
	   Point3	  endPoint;		/* this ray has finite length */
       Point3     direction;    /* unit vector, indicating ray direction */
} Ray;


typedef struct BinNode {
    Point3      min, max;      /* extent of node */
    GeomObjList members;       /* list of enclosed primitives */
    struct BinNode *child[2];  /* pointers to children nodes, if any */

    /* distance to the plane which subdivides the children */
    double  (*DistanceToDivisionPlane)(const Point3 &, const Ray &);

    /* children near/far ordering relative to a input point */
    void    (*GetChildren)(struct BinNode *, const Point3 &, 
				  struct BinNode **, struct BinNode **);

} BinNode, *BinNodePtr;


typedef struct {
    Point3     min, max;       /* extent of the entire bin tree */
    GeomObjList members;       /* list of all of the primitives */
    int         MaxDepth;      /* max allowed depth of the tree */
    int         MaxListLength; /* max primitive allowed in a leaf node */
    BinNodePtr  root;          /* root of the entire bin tree */
} BinTree;


//
// Data structure for a simple stack. This is necessary for implementing an 
// efficient linear BSP tree walking. A Stack size of 50 means it is possible
// to support a BSP tree of up to depth 49 and NO MORE!!! 
//
#define STACKSIZE  50

typedef struct {
    BinNodePtr node;
    double     min, max;
} StackElem;

typedef struct {
    int       stackPtr;
    StackElem stack[STACKSIZE];
} Stack, *StackPtr;


class gstPolyMesh;

gstBoolean RayTreeIntersect(Ray &ray, BinTree BSPTree, GeomObj **obj, double *distance,
							gstPolyMesh *);


void InitBinTree(BinTree *BSPTree, gstPolygon *polyArray, int numPolys, 
				 Vertex *vertices, int numVertices);

#endif
