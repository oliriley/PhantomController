//=============================================================================
//   Filename : gstTranslateManipulator.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Translate Manipulator to allow gstPHANToM to translate an node
//              with feedback force sent back to gstPHANToM.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TRANSLATE_MANIPULATOR
#define GST_TRANSLATE_MANIPULATOR

#include <gstBasic.h>
#include <gstManipulator.h>


// Translatonal manipulator.  This uses the PHANToM to translate an object,
// with force feedback.  When active, the manipulator generates a
// damping effect as the object is translated.  The PHANToM also must exceed
// a threshold force before the manipulator allows the object to be moved.
class GHOST_DLL_IMPORT_EXPORT gstTranslateManipulator: public gstManipulator {

public:

	// Constructor.
	gstTranslateManipulator() {
		manipSpringK = 0.5;
		manipMass = 0.002;
		manipTranslateThreshold = 0.6;
		manipDamping = 0.01;
	}

	// Destructor.
	~gstTranslateManipulator() {}


	// Set mass of translate manipulator [kilograms].
	void    setMass(double mass) { manipMass = mass; }

	// Get mass of translate manipulator [kilograms].
	double  getMass() const { return manipMass; }

	// Set stiffness of spring attached from gstPHANToM to translating object.
	void    setSpringK(double springK) { manipSpringK = springK; }

	// Get stiffness of spring attached from gstPHANToM to translating object.
	double  getSpringK() const { return manipSpringK; }

	// Set damping constant of damper attached from gstPHANToM to translating object.
	void    setManipDamping(double damping) { manipDamping = damping; }

	// Get damping constant of damper attached from gstPHANToM to translating object.
	double  getManipDamping() const { return manipDamping; }

	// Set threshold force to move object.
	void	setThresholdForce(double newThreshold) { 
		manipTranslateThreshold = newThreshold; 
	}

	// Get threshold force to move object.
	double	getThresholdForce() const { return manipTranslateThreshold; }

  // FOR_GHOST_EXTENSION
	// Resets all dynamic state (e.g. acceleration, velocity) of manipulator.
	virtual void resetState() {
		manipAccel.init(0.0,0.0,0.0);
		manipVel.init(0.0,0.0,0.0);
	}

  // FOR_GHOST_EXTENSION
	// Calculate the force.  When subclassing, the first 
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
	gstVector		manipVel, manipAccel;
	double			manipSpringK;
	double			manipDamping;
	double			manipMass;
	double			manipTranslateThreshold;
};



#endif // GST_TRANSLATE_MANIPULATOR
