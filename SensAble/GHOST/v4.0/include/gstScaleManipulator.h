//=============================================================================
//   Filename : gstScaleManipulator.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Rotate Manipulator to allow gstPHANToM to scale a node
//              with feedback force sent back to gstPHANToM.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SCALE_MANIPULATOR
#define GST_SCALE_MANIPULATOR

#include <gstBasic.h>
#include <gstManipulator.h>

// Scale manipulator.  This uses the PHANToM to scale an object,
// with force feedback.  When active, the manipulator generates a
// force effect as the object is scaled to allow gstPHANToM 
// to scale a node with force feedback.  The force effect is
// modeled as a mass attached to a spring and dashpot in
// parallel.
class GHOST_DLL_IMPORT_EXPORT gstScaleManipulator: public gstManipulator
{

public:

	// Constructor.
	gstScaleManipulator() {
		manipSpringK = 0.5;
		manipMass = 0.001;
		manipScaleThreshold = 0.6;
		manipDamping = 0.1;
	}

	// Destructor.
	~gstScaleManipulator() {}

	// Set mass of scale manipulator [kilograms].
	void setMass(double mass) { manipMass = mass; }

	// Get mass of scale manipulator [kilograms].
	double getMass() const { return manipMass; }

	// Set stiffness of spring attached from gstPHANToM for scaling object [Kg/(1000.0*sec^2)].
	void setSpringK(double springK) { manipSpringK = springK; }

	// Get stiffness of spring attached from gstPHANToM to scaling object [Kg/(1000.0*sec^2)].
	double getSpringK() const { return manipSpringK; }

	// Set damping of dashpot attached from gstPHANToM for scaling object [Kg/(1000.0*sec)].
	void setDamping(double newDamping) { manipDamping = newDamping; }

	// Get damping of dashpot attached from gstPHANToM to scaling object [Kg/(1000.0*sec)].
	double getDamping() const { return manipDamping; }

	// Set threshold force to move object.
	void setThresholdForce(double newThreshold) { 
		manipScaleThreshold = newThreshold; 
	}

	// Get threshold force to move object.
	double getThresholdForce() const { return manipScaleThreshold; }

	// FOR_GHOST_EXTENSION:
	// Resets all dynamic state (e.g. acceleration, velocity) of manipulator.
	virtual void resetState() {
		manipAccel.init(0.0,0.0,0.0);
		manipVel.init(0.0,0.0,0.0);
	}

	// FOR_GHOST_EXTENSION:
	// Calculate the force.  Force is returned in parent 
	// reference frame of phantom. When subclassing, the first 
	// parameter should be cast to gstPHANToM to retrieve 
	// any information about the state of the PHANToM that 
	// is needed to calculate the forces and move manipulated
	// node.  Also, if the manipulator is not active, the 
	// zero vector should be returned.	
	// ACTUNG!
	// WARNING!: Never call PHANToM->setForce or
	//			PHANToM->setForce_WC from this function.
	//			It will cause an infinite recursion.
	virtual gstVector calcManipulatorForce(void *PHANToMarg);
	virtual gstVector calcManipulatorForce(void *PHANToMarg, gstVector &torques) {
		torques.init(0.0, 0.0, 0.0);
		return calcManipulatorForce(PHANToMarg);
	}

protected:
	gstVector	manipVel, manipAccel;
	double		manipSpringK;
	double		manipDamping;
	double		manipMass;
	double		manipScaleThreshold;
    gstPoint    initBoundingRadii;
};


#endif // GST_SCALE_MANIPULATOR
