//=============================================================================
//   Filename : gstPolyMesh.h
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

#ifndef GST_POLYMESH
#define GST_POLYMESH

#include <malloc.h>
#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstErrorHandler.h>
#include <gstShape.h>

#include <gstPolyStructs.h>

#include <gstBSP.h>

// Deprecated - use gstTriPolyMesh instead
class GHOST_DLL_IMPORT_EXPORT gstPolyMesh: public gstShape
{
    GHOST_INHERIT_HEADER(gstPolyMesh, gstShape);

protected:

    BinTree bspTree;
    int touchable;
    int optimize;
    int numvertices;
    Vertex *vertices;			// array of vertices
    int numedges;
    Edge *edges;				// array of edges
    int numpolygons;
    struct _Polygon *polygons; // array of polygons
    
    struct _Polygon *poly1, 
                    *poly2, 
                    *closestIntersectedNeighborPoly, 
                    *lastClosestNeighborPoly;
    
    struct _Polygon *lastEdgePoly, *lastCornerPoly;
    
    gstVector firstIntersectionNormal;
    
    gstPoint	*oldPosSave, *curPosSave;
    double alpha, beta, minAlphaBeta, lastMinAlphaBeta;
    int roundRobinIndex;
    double epsilon;
    gstBoolean betaIsAlongEdge2;
    double shading;
    
    gstBoolean checkForDuplicateVertices_;
    
public:
  
    // Constructor.
    gstPolyMesh();
    
    // Copy Constructor
    gstPolyMesh(const gstPolyMesh &);
    
    // Constructor.
    gstPolyMesh(int numVertices, 
        double vertices[][3], 
        int numTrianglePolygons, 
        int trianglePolygons[][3],
        gstBoolean checkForDuplicateVertices = TRUE);
    
    // Constructor.
    gstPolyMesh(
        int numVertices,			// number of vertices
        int v_dimension,			// dimension of vertices == 3
        double **vertices,			// vertices
        int numTrianglePolygons,	// number of faces
        int num_sides,				// number of sides of polys == 3
        int **trianglePolygons,		// faces
        gstBoolean checkForDuplicateVertices = TRUE);
    
    // Destructor.
    virtual ~gstPolyMesh();
    
    // Clone.
    virtual gstNode *Clone() const {return ClonePolyMesh();}
    gstPolyMesh *ClonePolyMesh() const {return (new gstPolyMesh(*this));}
    
    void Reinitialize(
        int numVertices,			// number of vertices
        int v_dimension,			// dimension of vertices == 3
        double **vertices,			// vertices
        int numTrianglePolygons,	// number of faces
        int num_sides,				// number of sides of polys == 3
        int **trianglePolygons,		// faces
        gstBoolean checkForDuplicateVertices = TRUE);

    enum touchStyle { DOUBLE_SIDED, FRONT_SIDE, BACK_SIDE };
    
    // Set triangle touchable sides. (FRONT_SIDED, BACK_SIDED, or DOUBLE_SIDED)
    void setTouchableSides(int _touchable);
    
    // Get triangle touchable sides. (FRONT_SIDED, BACK_SIDED, or DOUBLE_SIDED)
    int getTouchableSides() const;
    
    // Get number of vertices.
    int getNumVertices() const { return numvertices; }
    
    // Get number of edges.
    int getNumEdges() const { return numedges; }
    
    // Get number of polygons.
    int getNumPolygons() const { return numpolygons; }
    
    // Return pointer to polygon last touched by PHANToM.
    gstPolygon *getLastPolyTouched() {
        return poly1;
    }
    
    // Set haptic rendering optimization level.  Lower
    // levels of optimization emphasize accuracy over
    // performance while higher levels of optimization
    // emphasize performance over accuracy.  Currently,
    // only levels 0 (do not optimize) and 1 (optimize) 
    // are supported.  The default is 0.
    void	setOptimize(int newLevel) {
        if (newLevel >= 0 && newLevel <= 1) {
            optimize = newLevel;
        } else {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstPolyMesh::setOptimize",newLevel);
        }
    }
    
    int getOptimize() const {return optimize;}
    
    // Get current level of surface smoothing  
    // (0.0 == none, through 1.0 ==  maximum).
    // The default is 0.4.
    double getSmoothing() const { return shading; }
    
    // Set level of surface smoothing (0.0 == none, through
    // 1.0 ==  maximum) if shading is enabled.  The default
    // is 0.4.
    void setSmoothing(const double sh) { 
        shading = sh;
        if (shading > 1) shading = 1;
        if (shading < 0) shading = 0;
    }
    
    
    // Get vertex by index.
    Vertex *getVertex(int index) { return &vertices[index]; }
    gstPoint getVertexPoint(int index) const;
    
    // Get edge by index.
    Edge *getEdge(int index) { return &edges[index]; }
    
    // Get polygon by index.
    struct _Polygon *getPolygon(int index) { return &polygons[index]; }
    void getPolygonIndices(int index, int &a, int &b, int &c) const;
    
    // FOR_GHOST_EXTENSION:
    // If line passing through oldPos and curPos intersects
    //  polygon then return TRUE and set intersection to 
    //  point of intersection.  Otherwise, return FALSE.
    int isIntersectingPolygon(	struct _Polygon *polygon, 
								const gstPoint &oldPos, 
                                const gstPoint &curPos,
                                gstPoint *intersection);
    
    // FOR_GHOST_INTERNAL:
    // Use this after isIntersectingPolygon from above has been
    // called if oldPos and curPos are unchanged.
    // If line passing through oldPosSave and curPosSave intersects
    // polygon then return TRUE and set intersection to 
    // point of intersection.  Otherwise, return FALSE.
    int isIntersectingPolygon(	struct _Polygon *polygon, 
								gstPoint *intersection);
    
    
    int polySide(struct _Polygon *polygon, gstPoint pt);
    
    gstBoolean DidCheckForDupVert() const {return checkForDuplicateVertices_;}
    
    gstInternal public:
    
    // GHOST_IGNORE_IN_DOC
    // Used by system or for creating sub-classes only.
    // Return cumulative transformation matrix.
    virtual gstTransformMatrix	&getCumulativeTransform();
    
    
    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Return interpolated normal of closest point
    // on surface of mesh to SCP.
    gstVector interpolate(gstPoint SCP);
    
    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // Returns TRUE if line segment defined by startPt_WC and endPt_WC
    // intersects this shape object.  If so, intersectionPt_WC is set to
    // point of intersection and intersectionNormal_WC is set to surface
    // normal at intersection point.
    virtual gstBoolean intersection(	const gstPoint &startPt_WC,
        const gstPoint &endPt_WC,
        gstPoint &intersectionPt_WC,
        gstVector &intersectionNormal_WC,
        void **data);
    
    // FOR_GHOST_INTERNAL:
    virtual void		setEdgeData(void *data) {
        lastEdgePoly = (struct _Polygon *) data;
        //printf("Last Edge normal :");
        //lastEdgePoly->normal.printSelf();
    }
    
    // FOR_GHOST_INTERNAL:
    virtual void		setCornerData(void *data) {
        lastCornerPoly = (struct _Polygon *) data;
        //printf("Last Corner normal :");
        //lastCornerPoly->normal.printSelf();
        
    }
    
    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // This is kinda a hack to let the object know that scene is
    // checking for a corner constraint now.
    virtual void		cornerCheck() {
        firstIntersectionNormal.init(0.0,0.0,0.0);
    }
    
    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // Returns TRUE if PHANToM is currently in contact with this object.
    // If so, the collision is added to the PHANToM's list through
    // gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
    virtual int collisionDetect(gstPHANToM *PHANToM);	
    
    
private:
        
    // All construction grunt work;
    void Initialize(
        int numVertices,			// number of vertices
        double **vertices,			// vertices
        int numTrianglePolygons,	// number of faces
        int **trianglePolygons,		// faces
        gstBoolean checkForDuplicateVertices = TRUE);
    
    // All Destruction grunt work;
    void Clean();		
    
    // FOR_GHOST_INTERNAL:
    gstVector getNormal(	struct _Polygon *polygon, 
        gstPoint PPolyIntersect);
    
    // FOR_GHOST_INTERNAL:
    Edge *getEdgeCrossed(	struct _Polygon *polygon, 
        gstPoint PPolyIntersect,
        gstPoint PPlaneProject, 
        gstPoint *PEdgeCrossing);
    
    // FOR_GHOST_INTERNAL:
    void	buildTree();
    
    // FOR_GHOST_INTERNAL:
    Edge *setEdge(struct _Polygon *p, int v1, int v2, int *e);	
    
    // FOR_GHOST_INTERNAL:
    void addPolygon(PolygonList **list, int index)
    {
        PolygonList *newEntry = (PolygonList *) malloc(sizeof(PolygonList));
        newEntry->polygonIndex = index;
        
        newEntry->next = *list;
        *list = newEntry;
    }
    
    // FOR_GHOST_INTERNAL:
    int pointInPolygon(gstPoint intersection, struct _Polygon *polygon);
    
    // FOR_GHOST_INTERNAL:
    gstVector calc_Vnormal(Vertex *vertex, gstVector *normal);
    
    // FOR_GHOST_INTERNAL:
    void gstPolyMeshError();
    
    // FOR_GHOST_INTERNAL:
    gstVector bound_concave(gstVector norm, gstPoint SCP, gstPolygon *poly);
        
};

#endif
