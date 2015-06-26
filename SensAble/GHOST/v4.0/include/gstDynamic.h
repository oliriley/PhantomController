//=============================================================================
//   Filename : gstDynamic.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Base class for dynamic nodes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_DYNAMIC
#define GST_DYNAMIC

#include <gstDllExport.h>
#include <gstSeparator.h>

#define GST_FORCE_THRESHOLD 0.0
#define GST_VELOCITY_THRESHOLD 0.01

#define GST_DEFAULT_DAMPING 1.0
#define GST_DEFAULT_MASS 0.25

// Graphics callback data structure for gstDynamic.
// This replaces (i.e., is a superset of) gstTransformGraphicsCBData.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstDynamicCBData {
	gstTransformMatrix			transform;	// From gstTransformGraphicsCBData
	gstTransformMatrix			cumulativeTransform;
	gstVector					reactionForce;			// Newtons
	gstVector					reactionTorque;			// Newton mm
	gstVector					velocity;				// mm/s
	gstVector					acceleration;			// mm/s^2
	gstVector					angularVelocity;		// rad/s
	gstVector					angularAcceleration;	// rad/s^2
} gstDynamicGraphicsCBData;

// Base class for dynamic nodes.
class GHOST_DLL_IMPORT_EXPORT gstDynamic : public gstSeparator
{
    GHOST_INHERIT_HEADER(gstDynamic, gstSeparator);

public:
	// Destructor.
	virtual ~gstDynamic();

	// Set default mass [kilograms].
	static void	setDefaultMass(double newMass) {
		if (newMass < 0.0) {
			gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
				"gstDynamic::setDefaultMass",newMass);
			return;
		}
		defaultMass = newMass;
		return;
	}

	// Get default mass [kilograms].
	static double getDefaultMass() {
		return defaultMass;
	}

	// Set default damping coefficient [Kg/(1000.0*sec)].
    static void setDefaultDamping(double newDamping) {
		if (newDamping <= 0) {
			gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
				"gstDynamic::setDefaultDamping", newDamping);
			return;
		}
		defaultDamping = newDamping;
		return;
	}
	
	// Get default damping coefficient [Kg/(1000.0*sec)].
	static double getDefaultDamping() {
		return defaultDamping;
	}

	// FOR_GHOST_INTERNAL:
	// This is set automatically by gstScene at the appropriate time.
	// This is only for internal use.
	// Set length of time between calls to updateDynamics() [seconds].
	static void setDeltaT(double newDeltaT);

	// Get length of time between calls to updateDynamics() [seconds].
	static double getDeltaT()  {
		return deltaT;
	}	


	// Set mass [kilograms].
	void setMass(double newMass) {
		if (newMass < 0.0) {
			gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
				"gstDynamic::setMass",newMass);
			return;
		}
		mass = newMass;
		return;
	}

	// Get mass [kilograms].
	double getMass() const {
		return mass;
	}

	// Set damping coefficient [Kg/(1000.0*sec)].
	void setDamping(double newDamping) {
		if (fabs(newDamping) >= MAX_DAMPING) {
			gstErrorHandler(GST_OUT_OF_RANGE_ERROR,
				"gstDynamic::setDamping",newDamping);
			return;
		}
		damping = newDamping;
		return;
	}

	// Get damping coefficient [Kg/(1000.0*sec)].
	double getDamping() const {
		return damping;
	}

	// Set velocity [millimeters/second].
	void setVelocity(double x, double y, double z) {
		setVelocity(gstPoint(x,y,z));
	}

	// Set velocity [millimeters/second].
	void setVelocity(const gstVector newVel) {
		velocity = newVel;
	}

	// Get velocity [millimeters/second].
	const gstVector	&getVelocity() const {
		return velocity;
	}

	// Set acceleration [millimeters/(second squared)].
	void setAccel(const gstVector newAccel) {
		acceleration = newAccel;
	}

	// Get acceleration [millimeters/(second squared)].
	const gstVector	&getAccel() const {
		return acceleration;
	}

	// Set angular velocity [radians/second].
	void setAngularVelocity(const gstVector newVel) {
		angularVel = newVel;
	}

	// Get angular velocity [radians/second].
	const gstVector &getAngularVelocity() const {
		return angularVel;
	}

	// Set angular acceleration [radians/(second squared)].
	void setAngularAccel(const gstVector newAccel) {
		angularAccel = newAccel;
	}

	// Get angular acceleration in [radians/(second squared)].
	const gstVector	&getAngularAccel() const {
		return angularAccel;
	}

	// Get reaction force from PHANToM [Newtons].
	const gstVector	&getReactionForce() const {
		return reactionForce;
	}

	// Get reaction force in world reference frame 
	// from PHANToM [Newtons].
	virtual gstVector getReactionForce_WC() {
		return toWorld(reactionForce);
	}

	// Get reaction torque from PHANToM [Newton*millimeters].
	const gstVector	&getReactionTorque() const {
		return reactionTorque;
	}

	// Get reaction torque in world reference frame
	// from PHANToM [Newton*millimeters].
	virtual gstVector getReactionTorque_WC() {
		return toWorld(reactionTorque);
	}

	// FOR_GHOST_INTERNAL:
	// NOT CURRENTLY SUPPORTED.
	// Use acceleration as current acceleration until reset by another
	// call to useConstantAccel or a call to useCalculatedAccel.
	// A call to this function will interupt an unfinished call
	// to interpolatePosition.
	void useConstantAccel(const gstVector &accel) {
		usingConstantAccel = TRUE;
		acceleration = accel;
	}

	// FOR_GHOST_INTERNAL:
	// NOT CURRENTLY SUPPORTED.
	// Use acceleration calculated from reaction force to do
	// Euler integration step in updateDynamics.  A call to this
	// function will interupt an unfinished call to interpolatePosition.
	void useCalculatedAccel() {
		usingConstantAccel = FALSE;
	}

	// FOR_GHOST_EXTENSION:
	// Set homogenous transformation matrix for dynamic object
	void setTransformMatrixDynamic(const gstTransformMatrix &matrix);

	// FOR_GHOST_EXTENSION:
	// Overwrite previous position with new position for dynamic object
	void setPositionDynamic(const gstPoint &newPos);

	// FOR_GHOST_EXTENSION:
	// Accumulate new translation with previous translation of dynamic object
	void translateDynamic(const gstPoint &translation);

	// FOR_GHOST_EXTENSION:
	// Overwrite previous translation with new translation for dynamic object
	void setTranslationDynamic(const gstPoint &translation);    

	// FOR_GHOST_EXTENSION:
	// Accumulate rotation with previous rotation of dynamic object
	void rotateDynamic(const gstVector &axis, double rad);

	// FOR_GHOST_EXTENSION:
	// Overwrite previous rotation with new rotation for dynamic object
	void setRotationDynamic(const gstVector &axis, double rad);

	// FOR_GHOST_EXTENSION:
	// Accumulate scale with previous scale of dynamic object
	void scaleDynamic(double scale);

	// FOR_GHOST_EXTENSION:
	// Overwrite previous scale with new scale for dynamic object
	void setScaleDynamic(double newScale);


	// GHOST_IGNORE_IN_DOC
	// Make newChild a child of this node.
	virtual void addChild(gstTransform *newChild);

	// GHOST_IGNORE_IN_DOC:
	// Remove child.
	virtual void removeChild(gstTransform *childToRemove);

	// GHOST_IGNORE_IN_DOC:
	// Remove child.
	virtual gstTransform *removeChild(int	childIndex);


gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Reset dynamic state of gstDynamic.  Subclasses should overload to
	// reset their own dynamic state and call gstDynamic::resetState before
	// returning.
	virtual void resetDynamicState();

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
	virtual	void prepareToUpdateGraphics();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Returns TRUE if this dynamic object has moved this servoLoop.
	// Only the transformation methods ending in Dynamic affect this
	// value.
	gstBoolean dynamicMoveThisServoLoop() const {
		return changedThisServoLoop;
	}

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Called when object is put in scene graph.
	virtual void putInSceneGraph() {
		resetDynamicState();
		gstSeparator::putInSceneGraph();
	}

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Called when object is removed from scene graph.
	virtual void removeFromSceneGraph();

	
	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// newDynamicDep points to a gstDynamic parent descendant of this
	// node in the scene graph.  gstDynamic nodes cannot have another
	// gstDynamic as a parent.  Setting this to anything other than
	// NULL will result in an error.
	virtual void setDynamicDependent(gstTransform *newDynamicDep);


	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Add force and torque pair if force is above threshold
	// and node is in scene.  Force and torque calculated 
	// from this call are used in the next call to updateDynamics.
	void addForceTorque(const gstVector &force_WC, gstPoint &SCP_WC);

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// When subclassing, call gstDynamic::updateDynamics() at the
	// end of your updateDynamics() procedure.
	virtual void updateDynamics();

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Add object to list of gstDynamic objects needing dynamic 
	// update.  gstDynamic nodes in this list have their 
	// updateDynamics method called every servoLoop until removed 
	// by removeFromDynamicList.  Objects are usually only put on
	// this list if they have a sufficient force imposed on them
	// and stay on until their velocity falls below a threshold
	// value for a certain amount of time (1 second).
	void addToDynamicList();

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Remove object from list of dynamic objects needing dynamic
	// update.
	void removeFromDynamicList();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Static method to call all updateDynamics of gstDynamic
	// objects in dynamic update list and in the scene graph.
	static void staticUpdateDynamics() {
		gstDynamic *dynPtr = dynamicListHead;

		while (dynPtr != NULL) {
			dynPtr->updateDynamics();
			dynPtr = dynPtr->rightDynamic;
		}
	}

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Do cleanup to prepare for next servoLoop
	static void	nextServoLoop();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// If this is the first Dynamic transformation this servoLoop, 
    // then--before doing transform--save cumulative transform in
	// lastCumTransf.
	void updateLastObjTransf();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Get cumulative transformation from end of last servoLoop.
  	gstTransformMatrix &getLastCumulativeTransform();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Transforms point p, which is in the local coordinate
	// reference frame, to the point in the world coordinate
	// reference frame that it was in during the previous simulation cycle.
	gstPoint toWorldLast(const gstPoint &p);

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Transforms vector v, which is in the world coordinate
	// reference frame, to the vector in the local coordinate
	// reference frame that it was in during the previous simulation cycle.	
	gstVector fromWorldLast(const gstVector &v);

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Transforms point p, which is in the world coordinate
	// reference frame, to the point in the local coordinate
	// reference frame that it was in during the previous simulation cycle.
	gstPoint fromWorldLast(const gstPoint &p);

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Transforms vector v, which is in the local coordinate
	// reference frame, to the vector in the world coordinate
	// reference frame that it was in during the previous simulation cycle.
	gstVector toWorldLast(const gstVector &v);

    // DEPRECATED: Name changed for consistency
    void setTranslateDynamic (const gstPoint &translation) {
        setTranslationDynamic(translation);
    }

    // DEPRECATED: Name changed for consistency
    void setRotateDynamic(const gstVector &axis, double rad) {
        setRotationDynamic(axis, rad);
    }

protected:

	// This class is intended as a base class only, the constructors
	// are protected so that instances can not be created.
	gstDynamic();
	gstDynamic(const gstDynamic *);

	// Copy Constructor.
	gstDynamic(const gstDynamic &);

    int removeFromDynamicListCounter;	// # of servoLoops gstDynamic
										// has been candidate to remove
										// from dynamic update list.
	
	// These are the default values used to set above params in constructor
	static double		defaultDamping;
	static double		defaultMass;

	static gstDynamic	*dynamicListHead;
	gstDynamic			*leftDynamic, *rightDynamic;

	int					inDynamicList;
	gstBoolean			usingConstantAccel;

	static double		deltaT;		// FOR_GHOST_EXTENSION	
									// 1/updateRate
	
	double				damping;		// FOR_GHOST_EXTENSION	
									// damping constant
	double				mass;  // FOR_GHOST_EXTENSION	
							   // mass of dynamic object represented by
							   //  sub-tree below this node.
	gstVector			velocity;		// FOR_GHOST_EXTENSION:	
										// mm/s
	gstVector			acceleration;	// FOR_GHOST_EXTENSION:	
										// mm/s^2
	gstVector			angularVel;		// FOR_GHOST_EXTENSION:	
										// rad/s
	gstVector			angularAccel;	// FOR_GHOST_EXTENSION:	
										// rad/s^2
	gstVector			reactionForce;	// FOR_GHOST_EXTENSION:	
										// Newtons
	gstVector			reactionTorque;	// FOR_GHOST_EXTENSION:	
										// milli-Newtons/m
};



inline void gstDynamic::setTransformMatrixDynamic(const gstTransformMatrix &matrix) {
		updateLastObjTransf();
		invalidateCumTransf();
		gstTransform::setTransformMatrix(matrix);
		changedThisServoLoop = TRUE;
}


inline void gstDynamic::translateDynamic (const gstPoint &translation) {
		updateLastObjTransf();
		invalidateCumTransf();
		gstTransform::translate(translation);
		changedThisServoLoop = TRUE;
}

inline void	gstDynamic::setTranslationDynamic (const gstPoint &translation) {
		updateLastObjTransf();
		invalidateCumTransf();
		gstTransform::setTranslation(translation);
		changedThisServoLoop = TRUE;
}
	
inline void	gstDynamic::setPositionDynamic(const gstPoint &newPos) {
		setTranslationDynamic(newPos);
}

inline void	gstDynamic::rotateDynamic(const gstVector &axis, double rad) {
		updateLastObjTransf();
		invalidateCumTransf();
		gstTransform::rotate(axis, rad);
		changedThisServoLoop = TRUE;
}

inline void	gstDynamic::setRotationDynamic(const gstVector &axis, double rad) {
		updateLastObjTransf();
		invalidateCumTransf();
		gstTransform::setRotation(axis, rad);
		changedThisServoLoop = TRUE;
}
	
inline void	gstDynamic::scaleDynamic(double scale) {
		updateLastObjTransf();
		invalidateCumTransf();
		objTransf.scale(gstPoint(scale, scale,scale));
		changedThisServoLoop = TRUE;
}
	
inline void	gstDynamic::setScaleDynamic(double newScale) {
		updateLastObjTransf();
		invalidateCumTransf();
		objTransf.setScale(gstPoint(newScale,newScale,newScale));
		changedThisServoLoop = TRUE;
}	


#endif
