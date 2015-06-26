//=============================================================================
//   Filename : gstShape.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Base class for haptic geometry nodes.
//
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SHAPE
#define GST_SHAPE


#ifdef _WINDOWS
#define GST_DEFAULT_SURFACE_KS .8
//#define GST_DEFAULT_SURFACE_KS 0.4
#else
#define GST_DEFAULT_SURFACE_KS 1.0
//#define GST_DEFAULT_SURFACE_KS 0.6
#endif


#define GST_DEFAULT_SURFACE_KD 0.0
#define GST_DEFAULT_SURFACE_FD 0.2
#define GST_DEFAULT_SURFACE_FS 0.35

#define GST_SHAPE_MAX_PHANTOM_STATES 4

#include <gstDllExport.h>
#include <gstPlane.h>
#include <gstPHANToM.h>
#include <gstBoundedHapticObj.h>

// State Array struct.  Holds touched state for
//  up to four separate PHANToMs.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstShapeStateArrayStruct {
    gstPHANToM *phantom;
    int inContact;
} gstShapeStateArrayStruct;

// Graphics callback data structure.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstShapeGraphicsCBData {
    gstTransformMatrix  transform;
    gstTransformMatrix  cumulativeTransform;
} gstShapeGraphicsCBData;

// Base class for haptic geometry nodes.
class GHOST_DLL_IMPORT_EXPORT gstShape : public gstBoundedHapticObj
{
    GHOST_INHERIT_HEADER(gstShape, gstBoundedHapticObj);

public:
    static const gstEventType TOUCHED;
    static const gstEventType UNTOUCHED;

    // Destructor.
    virtual ~gstShape();

    // Clone.
    virtual gstNode *Clone() const {return CloneShape();}
    gstShape *CloneShape() const {return (new gstShape(*this));}

    // Set default surface spring constant [Kg/(1000.0*sec^2)].
    // Range (0..1.0)
    static void setDefaultSurfaceKspring(double newKs) {
        if (newKs <= 0) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setDefaultSurfaceKs",newKs);
                return;
        }
        defaultSurfaceKs = newKs;
        return;
    }

    // Get default surface spring constant [Kg/(1000.0*sec^2)].
    static double getDefaultSurfaceKspring() {
        return defaultSurfaceKs;
    }

    // Set default surface damping coefficient [Kg/(1000.0*sec)].
    // Range (0..0.005).
    static void    setDefaultSurfaceKdamping(double newKd) {
        if (fabs(newKd) > MAX_SURFACE_DAMPING) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
            "gstShape::setDefaultSurfaceKd",newKd);
            return;
        }
        defaultSurfaceKd = newKd;
        return;
    }

    // Get default surface damping coefficient.
    static double getDefaultSurfaceKdamping() {
        return defaultSurfaceKd;
    }

    // Set default surface dynamic friction coefficient.
    // Range (0..1.0)
    static void    setDefaultSurfaceFdynamic(double newFd) {
        if (fabs(newFd) >= MAX_DYNAMIC_FRICTION) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
            "gstShape::setDefaultSurfaceFd",newFd);
            return;
        }
        defaultSurfaceFd = newFd;
        return;
    }

    // Get default surface dynamic friction coefficient.
    static double getDefaultSurfaceFdynamic() {
        return defaultSurfaceFd;
    }

    // Set default surface static friction coefficient.
    // Range (0..1.0)
    static void    setDefaultSurfaceFstatic(double newFs) {
        if (fabs(newFs) >= MAX_STATIC_FRICTION) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setDefaultSurfaceFs",
                newFs);
            return;
        }
        defaultSurfaceFs = newFs;
    }

    // Get default surface static friction coefficient.
    static double getDefaultSurfaceFstatic() {
        return defaultSurfaceFs;
    }

    // =============================================================


    // Set surface spring constant [Kg/(1000.0*sec^2)].
    // Range (0..1.0)
    void setSurfaceKspring(double newKs) {
        if (newKs <= 0) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setSurfaceSpringConstant",
                newKs);
            return;
        }
        surfaceKs = newKs;
        return;
    }

    // Get surface spring constant.
    double getSurfaceKspring() const {
        return surfaceKs;
    }

    // Set surface damping coefficient [Kg/(1000.0*sec)].
    // Range (0..0.005)
    void setSurfaceKdamping(double newKd) {
        if (fabs(newKd) > MAX_SURFACE_DAMPING) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setSurfaceDampingConstant",
                newKd);
            return;
        }
        surfaceKd = newKd;
        return;
    }

    // Get surface damping coefficient.
    double getSurfaceKdamping() const {
        return surfaceKd;
    }

    // Set surface dynamic friction coefficient.
    // Range (0..1.0)
    void setSurfaceFdynamic(double newFd) {
        if (fabs(newFd) >= MAX_DYNAMIC_FRICTION) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setSurfaceDynamicFriction",
                newFd);
            return;
        }
        surfaceFd = newFd;
        return;
    }

    // Get surface dynamic friction coefficient.
    double getSurfaceFdynamic() const {
        return surfaceFd;
    }

    // Set surface static friction coefficient.
    // Range (0..1.0)
    void setSurfaceFstatic(double newFs) {
        if (fabs(newFs) >= MAX_STATIC_FRICTION) {
            gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
                "gstShape::setSurfaceStaticFriction",
                newFs);
            return;
        }
        surfaceFs = newFs;
        return;
    }


    // Get surface static friction coefficient.
    double getSurfaceFstatic() const {
        return surfaceFs;
    }


    // Returns TRUE if object is in contact with any PHANToM in the scene.
    gstBoolean isInContact();
    
    // The node needs to reset its contacts when its touchability state
    // changes so that PHANToMs can no longer reference it.
    virtual void invalidateCumTouchability();

    // If flag is TRUE, shape is able to have contact with any gstPHANToMs in
    // the scene graph.  Otherwise, shape becomes transparent to gstPHANToMs
    // in the scene graph.
    virtual void setTouchableByPHANToM(gstBoolean bTouchable);

    // Resets all contact state pertaining to gstPHANToMs.  Thus, any
    // gstPHANToMs in contact with this shape will no longer be considered
    // in contact.
    void resetPHANToMContacts();

    // GHOST_IGNORE_IN_DOC
    // Transforms point p, which is in the local coordinate
    // reference frame, to the point in the world coordinate
    // reference frame.
    virtual gstPoint toWorld(const gstPoint &p);

    // GHOST_IGNORE_IN_DOC
    // Transforms point p, which is in the world coordinate
    // reference frame, to the point in the local coordinate
    // reference frame.
    virtual gstPoint fromWorld(const gstPoint &p);

    // GHOST_IGNORE_IN_DOC
    // Transforms point p, which is in the local coordinate
    // reference frame, to the point in the parent coordinate
    // reference frame.
    virtual gstPoint toParent(const gstPoint &p);

    // GHOST_IGNORE_IN_DOC
    // Transforms point p, which is in the parent coordinate
    // reference frame, to the point in the local coordinate
    // reference frame.
    virtual gstPoint fromParent(const gstPoint &p);

    // GHOST_IGNORE_IN_DOC
    // Transforms vector v, which is in the world coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame.
    virtual gstVector fromWorld(const gstVector &v) {
        return gstTransform::fromWorld(v);
    }

    // GHOST_IGNORE_IN_DOC
    // Transforms vector v, which is in the local coordinate
    // reference frame, to the vector in the world coordinate
    // reference frame.
    virtual gstVector toWorld(const gstVector &v) {
        return gstTransform::toWorld(v);
    }

    // GHOST_IGNORE_IN_DOC
    // Transforms vector v, which is in the parent coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame.
    virtual gstVector fromParent(const gstVector &v) {
        return gstTransform::fromParent(v);
    }

    // GHOST_IGNORE_IN_DOC
    // Transforms vector v, which is in the local coordinate
    // reference frame, to the vector in the parent coordinate
    // reference frame.
    virtual gstVector toParent(const gstVector &v) {
        return gstTransform::toParent(v);
    }

    // FOR_GHOST_EXTENSION
    // Transforms point p, which is in the local coordinate
    // reference frame - scale for this node, to the point 
    // in the world coordinate reference frame.
    virtual gstPoint toWorldNoLocalScale(const gstPoint &p);

    // FOR_GHOST_EXTENSION
    // Transforms point p, which is in the world coordinate
    // reference frame, to the point in the local coordinate
    // reference frame - scale for this node.
    virtual gstPoint fromWorldNoLocalScale(const gstPoint &p);

    // FOR_GHOST_EXTENSION
    // Transforms point p, which is in the local coordinate
    // reference frame - scale for this node, to the point 
    // in the parent coordinate reference frame.
    virtual gstPoint toParentNoLocalScale(const gstPoint &p);

    // FOR_GHOST_EXTENSION
    // Transforms point p, which is in the parent coordinate
    // reference frame, to the point in the local coordinate
    // reference frame - scale for this node.
    virtual gstPoint fromParentNoLocalScale(const gstPoint &p);


    // FOR_GHOST_EXTENSION
    // Transforms vector v, which is in the local coordinate
    // reference frame - scale for this node, to the vector 
    // in the world coordinate reference frame.
    virtual gstVector toWorldNoLocalScale(const gstVector &v) {
        return toWorld(v);
    }

    // FOR_GHOST_EXTENSION
    // Transforms vector v, which is in the world coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame - scale for this node.
    virtual gstVector fromWorldNoLocalScale(const gstVector &v) {
        return fromWorld(v);
    }

    // FOR_GHOST_EXTENSION
    // Transforms vector v, which is in the local coordinate
    // reference frame - scale for this node, to the vector 
    // in the parent coordinate reference frame.
    virtual gstVector toParentNoLocalScale(const gstVector &v) {
        return toParent(v);
    }

    // FOR_GHOST_EXTENSION
    // Transforms vector v, which is in the parent coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame - scale for this node.
    virtual gstVector fromParentNoLocalScale(const gstVector &v) {
        return fromParent(v);
    }



gstInternal public:
    
    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Called when object is added to the scene graph.
    virtual void putInSceneGraph();

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Called when object is removed from the scene graph.
    virtual void removeFromSceneGraph();

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Takes the current surface contact point (SCP) and 
    // surface contact point normal (SCPnormal), transforms
    // them to the world coordinate system, and adds them 
    // to gstPHANToM's list of current collisions.
    gstBoolean addCollision(gstPHANToM *, gstPoint &SCP, gstVector &SCPnormal);


    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Returns TRUE if the line segment, defined by startPt_WC and endPt_WC
    // in the world coordinate system, intersects the shape object.
    // If TRUE, intersectionPt_WC is set to the point of intersection and 
    // intersectionNormal_WC is set to the surface normal at the
    // intersection point.
    virtual gstBoolean intersection(    const gstPoint &startPt_WC,
                                        const gstPoint &endPt_WC, 
                                        gstPoint &intersectionPt_WC,
                                        gstVector &intersectionNormal_WC,
                                        void **data);

    // FOR_GHOST_INTERNAL
    virtual void setEdgeData(void *data) {
        (void) data;
    }
  
    // FOR_GHOST_INTERNAL
    virtual void setCornerData(void *data) {
        (void) data;
    }
  

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // This lets the object know that the scene is
    // checking for a corner constraint now so that the object
        // knows to return a true surface normal. 
        // PolyMesh requires this method.
    virtual void cornerCheck() {}


    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Returns TRUE if the point (pt) is inside of the object.
    virtual gstBoolean checkIfPointIsInside_WC(const gstPoint &pt);

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Returns TRUE if the PHANToM is currently in contact with this object.
    // If so, the collision is added to the PHANToM's list through
    // gstPHANToM::getCollisionInfo() and gstPHANToM::collisionAdded().
    virtual gstBoolean collisionDetect(gstPHANToM *phantomNode);


    // FOR_GHOST_EXTENSION
    // Return integer representing the contact state for curPHANToM with
    // this shape node.  0 is assumed to be FALSE and represents no
    // contact between curPHANToM and this shape node.  Otherwise, the
    // value may indicate special information about the contact between
    // curPHANToM this shape node.
    virtual int    getStateForPHANToM(gstPHANToM *curPHANToM);

    // FOR_GHOST_EXTENSION
    // Update the contact state between this shape node and curPHANToM.  This
    // stores the state for later retrieval with getStateForPHANToM.
    virtual gstBoolean updateStateForPHANToM(gstPHANToM *curPHANToM, int inContact);

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // This function prepares data to be sent to a graphics callback.  When
    // gstScene::updateGraphics() is called by the application, gstScene stalls
    // the application process and--in the haptic process--calls this method for each 
    // node in the scene that has had graphics information changes since the last 
    // call to gstScene::updateGraphics().  When finished, the application process
    // continues by calling updateGraphics for all the same nodes.  updateGraphics()
    // actually calls the user graphics callback with the current graphics information
    // that was copied over in the calls to this method (prepareToUpdateGraphics()).  
    // The haptics process, therefore, is ONLY used to copy the current graphics 
    // information and the application process calls the callback functions.
    //
    // Each subclass of gstShape that passes additional data to this graphics callback 
    // must redefine this method and call <PARENTCLASS>::prepareToUpdateGraphics() 
    // before exiting.  In order to pass additional data to graphics callback, cbData 
    // must point to the new datatype that adds any additional fields.  These 
    // fields are then filled in by prepareToUpdateGraphics().
    virtual    void prepareToUpdateGraphics();    
    
    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Remove object from shapes scene list.  This should not 
    // be used by the application program.
    //gstBoolean removeFromShapesInSceneList(gstTransform *nodeToRemove);

    // Returns first shape in shapes scene list.
    static gstShape *getShapesInScene() {
        return shapesInSceneHead;
    }

    // Returns "next" shape in shapes scene list.
    gstShape *getNextShapeInScene() {
        return nextShapeInScene;
    }

    // GHOST_IGNORE_IN_DOC
    // Used by system or for creating sub-classes only.
    // Returns cumulative transformation matrix.
    virtual gstTransformMatrix    &getCumulativeTransform();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    static gstShape    *getNearShapesInSceneHead() {
        return nearShapesInSceneHead;
    }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    static void    clearNearShapesInScene() {
        nearShapesInSceneHead = NULL;
    }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    gstShape *getNextNearShapeInScene() {
        return nextNearShapeInScene;
    }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    static void     addNearShapeInScene(gstShape *nextNear) {
        nextNear->nextNearShapeInScene = nearShapesInSceneHead;
        nearShapesInSceneHead = nextNear;
    }

    // FOR_GHOST_INTERNAL
        // NOT CURRENTLY SUPPORTED.
    // Set x,y,z coordinates of center.
    virtual void setCenter(const gstPoint &) {
        gstErrorHandler(GST_MISC_ERROR,"setCenter not supported for gstShape class");
    }


protected:

    // This class is intended as a base class only, the constructors
    // are protected so that instances can not be created.
    gstShape();
    gstShape(const gstShape *);
    gstShape(const gstShape &);

    double                surfaceKs;    // FOR_GHOST_INTERNAL:
                                    // surface spring constant.
    double                surfaceKd;  // FOR_GHOST_INTERNAL:
                                    // surface damper constant.
    double                surfaceFd;    // FOR_GHOST_INTERNAL:    
                                    // surface dynamic friction constant.
    double                surfaceFs;    // FOR_GHOST_INTERNAL:
                                    // surface static friction constant.
    gstShapeStateArrayStruct    *stateArray;    // FOR_GHOST_EXTENSION:
                                                // Contact state array.

    gstBoolean            _resetPHANToMContacts;    // FOR_GHOST_EXTENSION:
                                                // TRUE if next collisionDetect
                                                // should return FALSE to let
                                                // objects fall through.
                                                // CollisionDetect should
                                                // reset value to FALSE.
    
    // These are the static default values used to set above params 
    //  in constructor.
    static double        defaultSurfaceKs;
    static double        defaultSurfaceKd;
    static double        defaultSurfaceFd;
    static double        defaultSurfaceFs;

    static gstShape        *shapesInSceneHead;
    static gstShape        *nearShapesInSceneHead;

    gstShape            *nextShapeInScene;
    gstShape            *nextNearShapeInScene;
};
    
#endif
