//=============================================================================
//   Filename : gstInertiaEffect.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: A gstEffect for simulating inertial at the PHANToM tip
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_INERTIA_EFFECT
#define GST_INERTIA_EFFECT

#include <math.h>
#include <gstBasic.h>
#include <gstEffect.h>
#include <gstErrorHandler.h>

// Inertia effect for PHANToM.  This effect simulates
// inertia at the endpoint of a PHANToM as if a mass were attached 
// there, using a spring/damper model.  The mass, spring constant,
// and damping coefficient can be specified.
class GHOST_DLL_IMPORT_EXPORT gstInertiaEffect : public gstEffect 
{
public:

	// Constructor.
	gstInertiaEffect():gstEffect(){
				  mass = 0.01;
				  damping = 0.0;
				  springStiffness = 0.6;
				  gravity.init(0.0,-9.81,0.0);
				  velocity.init(0.0,0.0,0.0);
				  position.init(0.0,0.0,0.0);
				  }

	// Destructor.
	~gstInertiaEffect() {}

	// Set mass [kilograms].
	void	setMass(double newMass) {
		if (mass < 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstInertiaEffect:setMass Invalid < 0",newMass);
			return;
		}
		mass = newMass; 
	}

	// Get mass [kilograms].
	double 	getMass() const { return mass; }

	// Set Gravity in Meters/Sec^2
	void setGravity(gstVector _gravity) {
		gravity = _gravity;
	}

	// Get Gravity in Meters/Sec^2
	gstVector getGravity() {
		return gravity;
	}

	// Set damping coefficient [Kg/(1000.0*sec)].
	void	setDamping(double newDamping) {
		if (newDamping < 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstInertiaEffect:setDamping Invalid < 0",newDamping);
			return;
		}
		damping = newDamping; 
	}

	// Get damping coefficient [Kg/(1000.0*sec)].
	double 	getDamping()const { return damping; }

	// Set spring constant (stiffness) [Kg/(1000.0*sec^2)].
	void	setSpringStiffness(double newStiffness) {
		if (newStiffness <= 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstInertiaEffect:setSpringStiffness Invalid <= 0",newStiffness);
			return;
		}
		springStiffness = newStiffness; 
	}

	// Get spring constant (stiffness) [Kg/(1000.0*sec^2)].
	double 	getSpringStiffness()const { return springStiffness; }

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
	virtual gstVector calcEffectForce(void *data);
	virtual gstVector calcEffectForce(void *data, gstVector &torques) {
		torques.init(0.0, 0.0, 0.0);
		return calcEffectForce(data);
	}

protected:

	double		mass, damping, springStiffness;
	gstVector	velocity, acceleration, gravity;
	gstPoint	position;

};

#endif // GST_INERTIA_EFFECT
