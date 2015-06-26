//=============================================================================
//   Filename : gstSeparator.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Hierarchical node class that allows grouping of nodes under
//              it into a sub-tree.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SEPARATOR
#define GST_SEPARATOR

#include <gstDllExport.h>
#include <gstTransform.h>

// Node class that allows grouping of nodes under it into a sub-tree.  
class GHOST_DLL_IMPORT_EXPORT gstSeparator : public gstTransform
{
    GHOST_INHERIT_HEADER(gstSeparator, gstTransform);

public:

    // Constructor.
    gstSeparator();

    // Constructor.
    gstSeparator(const gstSeparator *);

    // Constructor.
    gstSeparator(const gstSeparator &);

    // Destructor.
    virtual ~gstSeparator();

    // Clone.
    virtual gstNode *Clone() const {return CloneSeparator();}
    gstSeparator *CloneSeparator() const {return (new gstSeparator(*this));}

    // Casting.
    virtual gstSeparator *AsSeparator() {return this;}

    // GHOST_IGNORE_IN_DOC
    // Returns first node in subtree with nodeName = name,
    // otherwise returns NULL.
    virtual gstNode *getByName(const gstNodeName &name);

    // If flag is FALSE, then all gstShape descendents of this
    // node will become untouchable by virtue of parent/child
    // cumulative touchability. If the flag is TRUE, then it
    // is up to the child nodes to determine their touchability
    virtual void setTouchableByPHANToM(const gstBoolean bTouchable);

    // Mark the cumulative touchability invalid for this node and all
    // children beneath this separator
    virtual void invalidateCumTouchability();

    // Add child to separator.
    virtual void addChild(gstTransform *newChild);

    // If "childToRemove" exists, it is removed from the separator.  
    // Otherwise, if it is invalid, an error is sent to gstErrorHandler.
    virtual void removeChild(gstTransform *childToRemove);

    // If "childIndex" is valid, the appropriate
    // child is removed and a pointer to the child is 
    // returned.  If "childIndex" is invalid, an error is sent
    // to gstErrorHandler and NULL is returned. Note that "childIndex"
    // is valid from 0 to (number_of_children - 1).
    virtual gstTransform *removeChild(int childIndex);

    // Returns a pointer to the child indicated by childIndex.  
    // If childIndex is invalid, an error is sent to gstErrorHandler 
    // and NULL is returned.  Note that childIndex
    // is valid from 0 to (number_of_childern - 1).
    gstTransform *getChild(int childIndex);

    // Returns the number of children under the separator.
    int getNumChildren() const { return numChildren; }

    // Set homogenous transformation matrix for the separator.
    virtual void setTransformMatrix(const gstTransformMatrix &matrix);

    // Set center for the separator.
    virtual void setCenter(const gstPoint &newCenter);

    // Overwrite previous position of node with new position.
    virtual void setPosition(double x,double y,double z);

    // Overwrite previous position of node with new position.
    virtual void setPosition(const gstPoint &newPos);

    // Accumulate translation with previous translation for the separator. 
    virtual void translate(double x, double y, double z);

    // Accumulate translation with previous translation for the separator. 
    virtual void translate(const gstPoint &translation);

    // Overwrite previous translation with new translation.
    virtual void setTranslation(double x, double y, double z);

    // Overwrite previous translation with new translation.
    virtual void setTranslation(const gstPoint &translation);

    // Accumulate rotation with previous rotation for the separator.
    virtual void rotate(const gstVector &axis, double rad);

    // Overwrite previous rotation with new rotation.
    virtual void setRotation(const gstVector &axis, double rad);

    // Accumulate scale with previous scale for the separator.
    virtual void scale(double scale);

    // Overwrite previous scale with new scale.
    virtual void setScale(double newScale);

    // Get scale factors along scale orientation axis.
    // If the matrix has not been set explicity, then
    // the scale orientation axis' coincide with the
    // local reference frame axis'.
    virtual void getScaleFactor(gstPoint &newScale) const {
        objTransf.getScaleFactor(newScale);
    }

    // Get scale factors along scale orientation axis.
    // If the matrix has not been set explicity, then
    // the scale orientation axis' coincide with the
    // local reference frame axis'.
    virtual gstPoint getScaleFactor() const {
        gstPoint scaleFactorArg;
        getScaleFactor(scaleFactorArg);
        return scaleFactorArg;
    }
    
    virtual double getBoundingRadiusOfChildren();


gstInternal public:

    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // Called when object is added to scene graph.
    virtual void putInSceneGraph();

    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // Called when object is removed from scene graph.
    virtual void removeFromSceneGraph();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Sets a pointer to a dynamic parent of this node.
    // This allows the application to determine if an 
    // object is part of a dynamic system.
    virtual void setDynamicDependent(gstTransform *newDynamicDep);
    
    // FOR_GHOST_EXTENSION:
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
    virtual void prepareToUpdateGraphics();

    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // When this object or any object above it in the scene changes its local
    // transformMatrix, then this node's cumulative transform matrix is not valid
    // until it is recomputed based on the new data.  This function invalidates
    // the cumulative transform matrix for this node and its inverse.
    virtual void invalidateCumTransf();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // When this object or any object above it in the scene changes its local
    // transformMatrix, then this node's cumulative transform matrix is not valid
    // until it is recomputed based on the new data.  This function invalidates
    // the cumulative transform matrix for this node and its inverse.  This function
    // also makes the object untouched if the object is not dynamic.
    virtual void invalidateCumTransfAndMakeUntouched();
    
    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Gets the number of children of the separator.  
    int getNumChildrenDEBUG();

    // DEPRECATED: Name changed for consistency
    virtual void setTranslate(double x, double y, double z) {
        setTranslation(x, y, z);
    }

    // DEPRECATED: Name changed for consistency
    virtual void setTranslate (const gstPoint &translation) {
        setTranslation(translation);
    }

    // DEPRECATED: Name changed for consistency
    virtual void setRotate(const gstVector &axis, double rad) {
        setRotation(axis, rad);
    }

protected:

    gstNodeList * children;
    int           numChildren;

private:

    // FOR_GHOST_INTERNAL:
    // Non-uniform scaling is not yet supported in GHOST. These are
        // kept private to prevent applications from attempting to perform non-uniform
        // scaling on objects.
    virtual void setScale(const gstPoint &newScale) {
        gstErrorHandler(GST_INVALID_SCALE, "gstSeparator::setScale",newScale);
    }

    // FOR_GHOST_INTERNAL:
    // Non-uniform scaling is not yet supported in GHOST. These are
    // kept private to prevent applications from attempting to perform non-uniform
    // scaling on objects.
    virtual void setScale(double x, double y, double z) {
        if (x || y || z) {}     // To remove compiler warning
        gstErrorHandler(GST_INVALID_SCALE, "gstSeparator::setScale");
    }

    // FOR_GHOST_INTERNAL:
    // Non-uniform scaling is not yet supported in GHOST. These are
    // kept private to prevent applications from attempting to perform non-uniform
    // scaling on objects.
    virtual void scale(double x, double y, double z) {
        if (x || y || z) {}     // To remove compiler warning
        gstErrorHandler(GST_INVALID_SCALE, "gstSeparator::setScale");
    }

    // FOR_GHOST_INTERNAL:
    // Non-uniform scaling is not yet supported in GHOST. These are
    // kept private to prevent applications from attempting to perform non-uniform
    // scaling on objects.
    virtual void scale(const gstPoint &newScale) {
        gstErrorHandler(GST_INVALID_SCALE, "gstSeparator::setScale", newScale);
    }
};


inline void gstSeparator::setTransformMatrix(const gstTransformMatrix &matrix) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::setTransformMatrix(matrix);
}

inline void gstSeparator::setCenter(const gstPoint &newCenter) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::setCenter(newCenter);
}

inline void gstSeparator::translate(const gstPoint &translation) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::translate(translation);
}

inline void gstSeparator::translate(double x, double y, double z) 
{
    translate(gstPoint(x, y, z));
}

inline void gstSeparator::setTranslation(const gstPoint &translation) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::setTranslation(translation);
}

inline void gstSeparator::setTranslation(double x, double y, double z) 
{
    setTranslation(gstPoint(x, y, z));
}

inline void gstSeparator::setPosition(double x, double y, double z) 
{
    setTranslation(gstPoint(x, y, z));
}

inline void gstSeparator::setPosition(const gstPoint &newPos) 
{
    setTranslation(newPos);
}

inline void gstSeparator::rotate(const gstVector &axis, double rad) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::rotate(axis, rad);
}

inline void gstSeparator::setRotation(const gstVector &axis, double rad) 
{
    invalidateCumTransfAndMakeUntouched();
    gstTransform::setRotation(axis, rad);
}

inline void gstSeparator::scale(double scale) 
{
    changedThisServoLoop = FALSE;
    addToGraphicsQueue();
    invalidateCumTransfAndMakeUntouched();
    objTransf.scale(gstPoint(scale, scale, scale));
}

inline void gstSeparator::setScale(double newScale) 
{
    changedThisServoLoop = FALSE;
    addToGraphicsQueue();
    invalidateCumTransfAndMakeUntouched();
    objTransf.setScale(gstPoint(newScale, newScale, newScale));
}

#endif
