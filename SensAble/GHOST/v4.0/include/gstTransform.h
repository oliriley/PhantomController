//=============================================================================
//   Filename : gstTransform.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Adds 3d transformations and callbacks to plain nodes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TRANSFORM
#define GST_TRANSFORM

#include <gstDllExport.h>
#include <gstTransformMatrix.h>

// Graphics callback data for gstTransform.  When
// prepareForGraphicsUpdate is called, graphics
// information is copied to this structure, which
// is then passed to the graphics callback in
// updateGraphics().  All subclasses of
// gstTransform that need to add callback data
// must define a new structure that has a
// superset of this structure's data and call
// its parents prepareForGraphicsUpdate and
// updateGraphics methods after their own are called.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstTransformCBData {
    gstTransformMatrix  transform;
    gstTransformMatrix  cumulativeTransform;
} gstTransformGraphicsCBData;

class gstTransform;
class gstSeparator;

typedef void (gstGraphicsCallback)
        (gstTransform *thisPtr, void *callbackData, void *userData);

typedef void (gstEventCallback)
        (gstTransform *thisPtr, void *callbackData, void *userData);

#include <gstEventStack.h>
#include <gstNode.h>


// Node class that adds 3D transformations and callbacks to nodes.
class GHOST_DLL_IMPORT_EXPORT gstTransform : public gstNode
{
    GHOST_INHERIT_HEADER(gstTransform, gstNode);

public:
    // Destructor.
    virtual ~gstTransform();

    // Clone.
    virtual gstNode *Clone() const {return CloneTransform();}
    gstTransform *CloneTransform() const {return (new gstTransform(*this));}

    // Casting.
    virtual gstTransform *AsTransform() {return this;}
    virtual gstSeparator *AsSeparator() {return 0;}

    friend class gstRotateManipulator;

    // Returns parent of node in graph.  
        // Returns NULL if the node has no parent or is the root of the scene graph.
    gstTransform *getParent() const { return parent; }


    //   Set graphics callback.  "callback" points to a user callback function that is
    //   called when gstScene::updateGraphics is called by the
    //   application and this instance of gstTransform or its
    //   subclasses have new graphics information.  Graphics
    //   information is passed as the second parameter
    //   "callbackData" and should be cast to the type  
    //   (CLASSNAMEgraphicsCBData *) for the correct class of this instance.
    void setGraphicsCallback(gstGraphicsCallback *callback, void *userdata);

    //   Set event callback.  "callback" points to a user callback function that is
    //   called when gstScene::updateEvents is called by the
    //   application and this instance of gstTransform or its
    //   subclasses have new event information.  Event
    //   information is passed as the second parameter
    //   "callbackData" and should be cast to type (gstEvent *).
    //   The fields of this structure are interpreted differently
    //   by each class and you should consult the GHOST Programming Guide
    //   for an list of nodes and their interpretation of these fields for
    //   various events.
    void setEventCallback(gstEventCallback *callback, void *userdata);

    // Get graphics callback user data.
    void *getGraphicsCBUserData() const {
        return graphicsCBUserData;
    }

    // Modify the touchability state of this node. When a node
    // is marked as untouchable, then its collision detection
    // should respect that and not introduce any forces.
    virtual void setTouchableByPHANToM(const gstBoolean bTouchable)
    {
        m_bTouchableByPHANToM = bTouchable;
    }

    // Returns the locally stored touchability state. This is not the same
    // as the cumulative isTouchableByPHANToM()
    gstBoolean getTouchableByPHANToM() const
    {
        return m_bTouchableByPHANToM;
    }

    // Marks the cumulative touchability state as invalid for this node
    virtual void invalidateCumTouchability()
    { 
        m_bCumTouchabilityValid = FALSE;
    }

    // Will evaluate the cumulative touchability state of the node
    // based on its own touchability as well as its parent nodes.
    gstBoolean isTouchableByPHANToM();

    //  Sets homogenous transformation matrix.
    //  Note: Any call to setTransformMatrix resets all scale
    //  rotate, and translate values set previously.  Conversly,
    //  any call to setRotation, setScale or setTranslation resets 
    //  the previous call to setTransform.  Setting a matrix 
    //  explicitly using this method or any of the array accessors
    //  to set a specific entry of the 4x4 matrix will cause 
    //  this transform matrix to become "User Defined".  A
    //  user defined matrix ceases to have the CSRTC' composite
    //  form described in the GHOST Programming Guide.  Instead,
    //  no composite form is assumed and some operations may run
    //  slower with the "User Defined" matrix since some 
    //  optimizations are not performed.
    void setTransformMatrix(const gstTransformMatrix &matrix);

    // Get homogenous transformation matrix.
    void getTransformMatrix(gstTransformMatrix &matrixArg);

    // Get homogenous transformation matrix.
    gstTransformMatrix getTransformMatrix() {
        gstTransformMatrix M;
        getTransformMatrix(M);
        return M;
    }
  
    // Get cumulative transformation matrix.
    void getCumulativeTransformMatrix(gstTransformMatrix &matrixArg);

    // Get cumulative transformation matrix.
    gstTransformMatrix  getCumulativeTransformMatrix() {
        gstTransformMatrix cM;
        getCumulativeTransformMatrix(cM);
        return cM;
    }

    // Get scale orientation matrix.
    void getScaleOrientationMatrix(gstTransformMatrix &matrixArg);
    

    // Get scale orientation matrix.
    gstTransformMatrix  getScaleOrientationMatrix() {
        gstTransformMatrix cM;
        getScaleOrientationMatrix(cM);
        return cM;
    }

    // Get rotation matrix.
    void getRotationMatrix(gstTransformMatrix &matrixArg);

    // Get rotation matrix.
    gstTransformMatrix getRotationMatrix() {
        gstTransformMatrix cM;
        getRotationMatrix(cM);
        return cM;
    }

    // Get equivilant rotation of current rotation matrix (orientation) based 
    // on successive rotations around x,y,z axes.  Angles are in radians
    // and use right hand rule.
    gstPoint getRotationAngles() const {
        gstPoint axes;
        objTransf.getRotationAngles(axes);
        return axes;
    }

    // Get equivilant rotation of current rotation matrix (orientation) based 
    // on successive rotations around x,y,z axes.  Angles are in radians
    // and use right hand rule.
    void getRotationAngles(gstPoint &axes) const {
        objTransf.getRotationAngles(axes);
    }

    // Overwrite previous center position of node with new center position.
    // Not supported for gstShape classes.
    virtual void setCenter(const gstPoint &newCenter);    

    // Get x,y,z coordinates of center.  Not supported for gstShape classes.
    virtual void getCenter(gstPoint &centerArg) const;

    // Get x,y,z coordinates of center.  Not supported for gstShape classes.
    virtual gstPoint getCenter() {
        gstPoint centerArg;
        getCenter(centerArg);
        return centerArg;
    }

    // Get position in local coordinate reference frame.
    virtual void getPosition(gstPoint &pos) {
        if (objTransf.getUserDefined())
            pos.init(objTransf.get(3,0), objTransf.get(3,1), objTransf.get(3,2));
        else
            objTransf.getTranslation(pos);

    }

    // Get position in local coordinate reference frame.
    virtual gstPoint getPosition() {
        gstPoint pos;
        getPosition(pos);
        return pos;
    }

    // Get x,y,z translation in world coordinates.
    virtual void getPosition_WC(gstPoint &pos) {
        getTranslation_WC(pos);
    }
 
    // Get x,y,z translation in world coordinates.
    virtual gstPoint getPosition_WC() {
        gstPoint pos;
        getPosition_WC(pos);
        return pos;
    }

    // Overwrite previous translation with new translation.
    virtual void setPosition(const gstPoint &newPos) {
        setTranslation(newPos);
    }

    // Overwrite previous translation with new translation given as
    // a position in world reference frame coordinates.
    virtual void setPosition_WC(const gstPoint &newPos_WC) {
        if (parent != NULL)
            setTranslation(parent->fromWorld(newPos_WC));
        else
            setTranslation(newPos_WC);
    }

    // Overwrite previous translation with new translation.
    virtual void setPosition(double x,double y,double z) {
        setTranslation(gstPoint(x,y,z));
    }

    // Overwrite previous translation with new translation.
    virtual void setTranslation(double x, double y, double z) {
        setTranslation(gstPoint(x,y,z));
    }
  
    // Accumulate translation with previous translation of node.
    virtual void translate(double x, double y, double z) {
        translate(gstPoint(x,y,z));
    }

    // Accumulate translation with previous translation of node.
    virtual void translate (const gstPoint &translation);

    // Set translation.
    virtual void setTranslation(const gstPoint &translation);

    // Get translation in local coordinate reference frame.
    virtual void getTranslation(gstPoint &translationValue) const;

    // FOR_GHOST_INTERNAL
    // Get translation in world coordinate reference frame.
    virtual void getWorldTranslation(gstPoint &translationValue) {
        getTranslation_WC(translationValue);
    }

    // Get translation in world coordinate reference frame.
    virtual void getTranslation_WC(gstPoint &translationValue) const;

    // Add new rotation specified by vector/axis angle approach
    // to existing rotation.  Angle is in radians.
    // this version right multiplies: newRot = currentRot * givenRot
    void rotateRM(const gstVector &, double radians);

    // Add new rotation specified by vector/axis angle approach
    // to existing rotation.  Angle is in radians.
    // this version left multiplies: newRot = givenRot * currentRot
    void rotateLM(const gstVector &axis, double radians);        

    // Overwrite previous rotation of node using vector/angle method [radians].
    virtual void setRotation(const gstVector &axis, double rad);

    // FOR_GHOST_INTERNAL
    // Not currently supported.
    virtual void getRotation(gstVector &axisArg, double *radArg) const;

    // Accumulate uniform scale with previous scale of node.
    virtual void scale(double scale);    

      // Accumualte scale with previous scale of node.
    virtual void scale(double x, double y, double z) {
        scale(gstPoint(x,y,z));
    }

    // Accumulate scale with previous scale of node.
    virtual void scale(const gstPoint &newScale);

    // Overwrite previous scale of node with new scale.
    virtual void setScale(double x, double y, double z);

    // Overwrite previous scale of node with new uniform scale.
    virtual void setScale(double newScale);

    // Overwrite previous scale of node with new scale.
    virtual void setScale(const gstPoint &newScale);

    // Get x,y,z scale  factors along scale orientation axis.
    virtual void getScaleFactor(gstPoint &scaleFactorArg) const ;

    // Get x,y,z scale factors along scale orientation axis.
    virtual gstPoint getScaleFactor() const {
        gstPoint scaleFactorArg;
        getScaleFactor(scaleFactorArg);
        return scaleFactorArg;
    }

    // Transform point "p", which is in the local coordinate
    // reference frame, to the point in the world coordinate
    // reference frame.
    virtual gstPoint toWorld(const gstPoint &p);

    // Transform vector "v", which is in the world coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame.
    virtual gstVector fromWorld(const gstVector &v);

    // Transform point "p", which is in the world coordinate
    // reference frame, to the point in the local coordinate
    // reference frame.
    virtual gstPoint fromWorld(const gstPoint &p);

    // Transform vector "v", which is in the local coordinate
    // reference frame, to the vector in the world coordinate
    // reference frame.
    virtual gstVector toWorld(const gstVector &v);

    // Transform point "p", which is in the local coordinate
    // reference frame, to the point in the parent coordinate
    // reference frame.
    virtual gstPoint toParent(const gstPoint &p){return objTransf.fromLocal(p);}

    // Transform vector "v", which is in the parent coordinate
    // reference frame, to the vector in the local coordinate
    // reference frame.
    virtual gstVector fromParent(const gstVector &v){return objTransf.toLocal(v);}

    // Transform point "p", which is in the parent coordinate
    // reference frame, to the point in the local coordinate
    // reference frame.
    virtual gstPoint fromParent(const gstPoint &p){return objTransf.toLocal(p);}

    // Transform vector "v", which is in the local coordinate
    // reference frame, to the vector in the parent coordinate
    // reference frame.
    virtual gstVector toParent(const gstVector &v){return objTransf.fromLocal(v);}

    // FOR_GHOST_EXTENSION
    //  Get cumulative transformation matrix.
    //  NOTE: result is a non constant reference.  
    virtual gstTransformMatrix &getCumulativeTransform();
    
    
gstInternal public:

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Called when object is removed from scene graph.
    virtual void removeFromSceneGraph();

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Called when object is put in scene graph.
    virtual void putInSceneGraph() {
        gstNode::putInSceneGraph();
        addToGraphicsQueue();

        // Make sure the cumulative state for this node is invalidated
        invalidateCumTransf();
        invalidateCumTouchability();
    }

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Invalidate cumulative transformation matrix.
    // When this object or any object above it in the scene changes its local
    // transformMatrix, this node's cumulative transform matrix becomes not valid
    // until it is recomputed based on the new data.  This function invalidates
    // the cumulative transform matrix for this node and its inverse.
    virtual void invalidateCumTransf();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Invalidate cumulative transformation matrix and make it untouched.
    // When this object or any object above it in the scene changes its local
    // transformMatrix,  this node's cumulative transform matrix becomes not valid
    // until it is recomputed based on the new data.  This function invalidates
    // the cumulative transform matrix for this node and its inverse.
    virtual void invalidateCumTransfAndMakeUntouched();

    
    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Set parent.
    void setParent(gstTransform *newParent);

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Set newDynamicDep as gstDynamic descendent parent of this instance.
    // Indicates whether a node is part of a dynamic system.
    virtual void setDynamicDependent(gstTransform *newDynamicDep) {
        dynamicDependent = newDynamicDep;
    }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Make object untouched.  This is called when an object is 
    // transformed using the normal--i.e. non
    // dynamic--transformations.  It should turn the
    // forces off for all geometry objects under and
    // including this node until the PHANToM leaves
    // the object (if the PHANToM is inside the object).
    virtual void makeUntouched() {}

    // Get dynamic dependent.  A node should only have one ancestor of type
    // gstDynamic, this method returns that node.
    gstTransform *getDynamicDependent() const {
        return dynamicDependent; }
  
    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Returns TRUE if object is in graphics queue.
    gstBoolean getInGraphicsQueue() { return inGraphicsQueue; }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Set up data structures to update graphics for all transforms
    // in the scene that have changed since the last call to 
    // staticPrepareToUpdateGraphics().
    static void    staticPrepareToUpdateGraphics();

    // FOR_GHOST_EXTENSION
    // Used by system or for creating sub-classes only.
    // Set up data structures to update graphics.
    virtual void prepareToUpdateGraphics();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Update graphics for all transforms in the scene that have
    // changed since the last call to staticUpdateGraphics().
    static void    staticUpdateGraphics();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Update graphics.
    void updateGraphics();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Set up data structures to update events for all transforms
    // that have changed since the last call to staticPreparetoUpdateEvents().
    static void    staticPrepareToUpdateEvents();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Set up data structures to update events.
    virtual void prepareToUpdateEvents();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Update events for all transforms in the scene graph.
    static void    staticUpdateEvents();

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Update events.
    void updateEvents();
  
    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Debug method.  Not for use by applications. 
    gstTransformMatrix    getObjectTransformMatrixDEBUG() {
        return objTransf;
    }

    // FOR_GHOST_INTERNAL
    // Used by system or for creating sub-classes only.
    // Debug method.  Not for use by applications. 
    gstTransformMatrix    getCumTransformMatrixDEBUG() {
        return cumTransf;
    }

    // DEPRECATED: Name changed for consistency
    virtual void setRotate(const gstVector &axis, double rad) {
        setRotation(axis, rad);
    }

    // DEPRECATED: Name changed for consistency
    virtual void rotate(const gstVector &axis, double rad) {
        rotateLM(axis, rad);
    }

    // DEPRECATED: Name changed for consistency
    virtual void setTranslate(const gstPoint &translation) {
        setTranslation(translation);
    }

    // DEPRECATED: Name changed for consistency
    virtual void setTranslate(double x, double y, double z) {
        setTranslation(gstPoint(x,y,z));
    }


protected:

    // This class is intended as a base class only, the constructors
    // are protected so that instances can not be created.
    gstTransform();

    // Constructor.
    gstTransform(const gstTransform *);

    // Constructor.
    gstTransform(const gstTransform &);

    gstTransform        *parent;
    gstTransform        *dynamicDependent;

    // transform matrices
    gstTransformMatrix      objTransf, lastObjTransf;
    gstVector               scaleFactor;
    gstTransformMatrix      cumTransf, lastCumTransf;
    int                     cumTransformValid;

    // Event and Graphics Queues data
    gstBoolean            inGraphicsQueue;
    gstBoolean            inEventQueue;
    gstBoolean            changedThisServoLoop;

    static gstTransform    *graphicsQueueHead;
    static gstTransform    *waitingForGraphicsCallbackHead;
    static gstTransform    *eventsQueueHead;
    static gstTransform    *eventsWaitingToBeFiredHead;

    gstTransform        *nextNodeInGraphicsQueue;
    gstTransform        *nextWaitingForGraphicsCallback;
    gstTransform        *nextEventInQueue;
    gstTransform        *nextEventWaitingToBeFired;

    gstGraphicsCallback *graphicsCallback;
    void *graphicsCBUserData;

    // This is used to store graphics data for callback.
    // Memory is allocated in this class's constructor.
    void *graphicsCBData;

    gstEventCallback *eventCallback;
    void *eventCBUserData;

    // Useful methods.
    gstBoolean addEvent(const gstEvent &newEvent);
    void addToGraphicsQueue();


private:

    // NOTE: Other subclasses should not directly touch this.
    gstEventStack events, eventsWaitingToBeFired;

    gstBoolean m_bTouchableByPHANToM;
    gstBoolean m_bCumTouchableByPHANToM;
    gstBoolean m_bCumTouchabilityValid;

};
    
#endif
