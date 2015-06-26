//=============================================================================
//   Filename : gstEffect.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Base class for effects, for generating forces to be added 
//              to the PHANToM throughout the scene
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_EFFECT
#define GST_EFFECT

#include <gstBasic.h>
#include <gstVector.h>

// Base class for PHANToM special effects.
// Unlike other GHOST force phenomena, these effects
// are geometry-independent, meaning that forces are
// not generated based on any specific geometric object.

class GHOST_DLL_IMPORT_EXPORT gstEffect {

public:

	// Destructor.
	virtual ~gstEffect() {}

	// Returns TRUE if manip is currently active.
	gstBoolean			isActive() const { return active; }

	// FOR_GHOST_EXTENSION:
	// Start the effect.  WARNING:  When re-starting an effect,
    // make sure to reset any state, such as past PHANToM position.
    // Otherwise, the next call to calcEffectForce could 
	// generate unexpectedly large forces.
	virtual gstBoolean start() { 
		active = TRUE; time = 0.0; return TRUE;}

	// FOR_GHOST_EXTENSION:
	// Stop the effect.
	virtual void stop() { 
		active = FALSE;}

	// FOR_GHOST_EXTENSION:
	// Caculate the force.  Force is returned in parent 
	// reference frame of phantom. When subclassing, the first 
	// parameter should be cast to gstPHANToM to retrieve 
	// any information about the state of the PHANToM that 
	// is needed to calculate the forces.  deltaT should 
	// be used to update the time.  Also, if the effect is
	// not active, the zero vector should be returned.
	// ACTUNG!
	// WARNING!: Never call PHANToM->setForce or
	//			PHANToM->setForce_WC from this function.
	//			It will cause an infinite recursion.

	// This method will protect current customer's code by allowing it to
	// compile and run in the new scheme where the method is called with
	// two arguments.
	virtual gstVector calcEffectForce(void *PHANToM) {
		if (PHANToM) {}     // To remove compiler warning
		return gstVector(0.0,0.0,0.0);
	}

	virtual gstVector calcEffectForce(void *PHANToM, gstVector &torques) {
		torques.init(0.0, 0.0, 0.0);
		return calcEffectForce(PHANToM);
	}

protected:

	// This class is intended as a base class only, the constructor
	// is protected so that instances can not be created.
	gstEffect() : active(FALSE) { }

	double			time;	// FOR_GHOST_EXTENSION:	
							// Time since start in seconds
	gstBoolean		active; // FOR_GHOST_EXTENSION:	
							// TRUE if effect is active

};



#endif // GST_EFFECT
