//=============================================================================
//   Filename : gstPHANToMDynamic.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: For dual PHANToM use only
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_PHANTOM_DYNAMIC
#define GST_PHANTOM_DYNAMIC

#define MAX_COLLISIONS 10


#include <gstDllExport.h>
#include <gstDynamic.h>
#include <gstEffect.h>
#include <gstPHANToMInfo.h>

// Already declared in gstBasic.h
// class gstShape;

// Graphics callback data structure for gstPHANToM.
// This data structure replaces (i.e., is a superset of) 
// gstTransformDynamicCBData.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstPHANToMDynamicGraphicsCBData {
	gstTransformMatrix	transform;
	gstTransformMatrix	cumulativeTransform;
	gstVector	reactionForce;			// Newtons
	gstVector	velocity;				// mm/s
	gstVector	acceleration;			// mm/s^2
	gstVector	angularVelocity;		// rad/s
	gstVector	angularAcceleration;	// rad/s^2
	gstBoolean	stylusSwitch;			// on/off
	gstVector PHANToMForce;				// Newtons
	gstVector PHANToMTorque;			// milliNewton-meters
} gstPHANToMDynamicGraphicsCBData;


// PHANToM haptic interface dynamic class.  This node has a
// PHANToM device associated with it, much like the gstPHANToM
// node and therefore shares many methods in common with 
// gstPHANToM.  Note that the PHANToM encoders are reset by
// default when a gstPHANToMDynamic node is instantiated.  For
// more information on reseting the PHANToM refer to the
// gstPHANToM section of the GHOST Programming Guide. The position and
// orientation of this node are influenced or may correlate
// directly with the position and orientation of the associated
// PHANToM device specified by the configuration file passed to
// the constructor.  The descendent geometry nodes of this node
// have force interactions with other gstPHANToM nodes in scene.  
// For example, a gstCube under a gstTranslateManipulator is
// translated around to match the translation of the PHANToM
// device. As gstPHANToM nodes touch the gstCube, forces
// are sent to the gstPHANToM and the PHANToM associated with
// this node as if you are moving the block with one PHANToM
// and preventing it's movement with the gstPHANToM node.
class GHOST_DLL_IMPORT_EXPORT gstPHANToMDynamic : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstPHANToMDynamic, gstDynamic);

public:

	// Constructor.  Requires character string indicating
	// name of the PHANToM initialization file.  ResetEncoders
	// specifies if PHANToM encoders are to be reset to zero
	// when creating instance.  Encoders will be reset if TRUE,
	// otherwise encoders are not reset.  Default value is TRUE.
	gstPHANToMDynamic(char *configFile, int resetEncoders = TRUE);

	// Destructor.
	virtual ~gstPHANToMDynamic();

	// FOR_GHOST_EXTENSION:
	// Called when object is added to scene graph.
	virtual void		putInSceneGraph();

	// FOR_GHOST_EXTENSION:
	// Called when object is removed from scene graph.
	virtual void		removeFromSceneGraph();


	// Resets the encoders on the PHANToM device.
	void				resetEncoders();

	// Returns TRUE is instance had no errors during construction.
	gstBoolean			getValidConstruction() const {
		return validConstruction;
	}

	// Get reaction force in world reference frame 
	// from PHANToM [Newtons].
	virtual gstVector			getReactionForce_WC() {
		return parent->toWorld(reactionForce);
	}

	// Get reaction torque in world reference frame
	// from PHANToM [Newton*millimeters].
	virtual gstVector			getReactionTorque_WC() {
		return parent->toWorld(reactionTorque);
	}

	// Get average servo-loop (PHANToM update) rate [Hz].
	double				getAverageUpdateRate();

	// Get the time increment between the last two PHANToM
	// updates [seconds].
	double				getDeltaT();


	// Get position of PHANToM in world coordinates. 
        // The origin of this object's
	// local reference frame is actually the PHANToM
	// position.
	void			getPosition_WC(gstPoint &pt) {
		pt.init(0.0,0.0,0.0);
		pt = toWorld(pt);
	}

	// Get position of PHANToM in world coordinates. 
	// The origin of this object's
	// local reference frame is actually the PHANToM
	// position.
	virtual gstPoint	getPosition_WC() {
		gstPoint pos;
		getPosition_WC(pos);
		return pos;
	}


	// Get previous position of PHANToM in world coordinates (i.e.
	// position before previous call to gstPHANToM::update()).
	void				getLastPosition_WC(gstPoint &pt) {
		pt = lastPosition_WC;
	}

	// Returns whether there is a dev fault.
	gstBoolean			getStatus();
	
	// FOR_GHOST_INTERNAL:
	// NOT CURRENTLY SUPPORTED.
    // Provides PHANToM setup info.
    // Currently, this only indicates whether there is a gimbal.
	void				getInfo(gstPHANToMInfoStruct *info);
	
	// If flag is TRUE then forces will be turned on and sent to 
	//  PHANToM when simulating.  Otherwise, forces will not be
	//  turned on nor used at any time.
	void					setForceOutput(gstBoolean flag);

	//	Returns TRUE if forces are to be used during simulation.
	gstBoolean				getForceOutput() const { return _useForces; }


	// FOR_GHOST_INTERNAL:
	// Turn forces on for this PHANToM.
	virtual int				forcesOn();

	// FOR_GHOST_INTERNAL:
	// Turn forcess off for this PHANToM.
	virtual int				forcesOff();

	// Returns the max gain value before buzzing will occur.
	double				getMaxGain() const { return maxGain; }

	// Returns the max gain value before buzzing will occur.
	void				setMaxGain(double newMaxGain)  { 
		maxGain = newMaxGain; 
	}


	// Associate an special effect (i.e., from gstEffect class and sub-classes)
	// with the PHANToM.  If a previous effect
	// was in place, it is stopped before the new effect is added.
	void				setEffect(gstEffect *newEffect) {
		// Before we set a new effect, if we had an old effect then
		//  stop it first.
		stopEffect();
		effect = newEffect; }
	
	// Returns current effect currently associated with the PHANToM.
	gstEffect				*getEffect() {
		return effect;
	}
	
	// Start the effect (if any) associated with the PHANToM.
	gstBoolean			startEffect() { 
		if (effect != NULL) effect->start();
		return TRUE;}
	
	// Stop the effect (if any) associated with the PHANToM.
	gstBoolean			stopEffect() { 
		if (effect != NULL) effect->stop();
		return TRUE;}


gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Update the PHANToM.
	int						update();
			
	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Set PHANToM forces in local coordinate system.
	virtual int			setForce(const gstVector &force, const gstVector &torque);

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Set PHANToM forces in world coordinate sytem.
	virtual int			setForce_WC(const gstVector &force_WC, const gstVector &torque_WC);

	
	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Enable forces for all PHANToMs in scene graph.
	static	int			enableAllForces();


	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Disable forces for all PHANToMs in scene graph.
	static	int			disableAllForces();

	// FOR_GHOST_INTERNAL:
	// Used by system or for creating sub-classes only.
	// Update state (e.g. position, velocity) of all PHANToMs 
	// in the scene graph.
	static int			updateAll();



	// FOR_GHOST_EXTENSION:
	// Called every servo loop to update PHANToM state and dynamic state of node.
	virtual void		updateDynamics();

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
	virtual	void		prepareToUpdateGraphics();

protected:

	gstBoolean				validConstruction;
	gstPHANToMInfoStruct	info;
	gstTransformMatrix		phantomMatrix;	// FOR_GHOST_EXTENSION:
											// 4x4 homogenous matrix for PHANToM
											// position and orientation.  This
											// is updated automatically, every
											// servo loop.
									
	gstVector				phantomVelocity;// FOR_GHOST_EXTENSION:
											// Velocity of PHANToM in mm/sec.
									
	int						phantomId;
	gstPoint				lastPosition_WC;
	gstPoint				lastPos;
	gstEffect				*effect;
	gstBoolean				forcesAreOn;
	gstBoolean				stylusSwitch;
	gstVector				PHANToMForce;
	gstVector				PHANToMTorque;
	static gstPHANToMDynamic *gstPHANToMDynamicHead;
	gstPHANToMDynamic		*nextPHANToMDynamic;
	gstBoolean				_useForces;
	double					maxGain;

	// FOR_GHOST_INTERNAL:
	// This function resets collision object forces to zero to prepare for the next
	// servoLoop.  It also finds the first collision object that is dynamic, if one exists,
	// and transforms the lastSCP by the delta in movement that the dynamic obj underwent
	// the last servoLoop.  This tries to keep the SCP on the surface.  This breaks
	// if two collision objects move differently in one servoLoop.  
	// It is assumed that, for now, multiple simultaneous collisions with
	// moving objects are intersections of sub-objects of the same dynamic parent.
	void	prepareForNextLoop();

};

	
#endif


