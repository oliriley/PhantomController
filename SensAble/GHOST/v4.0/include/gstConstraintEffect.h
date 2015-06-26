//=============================================================================
//   Filename : gstConstraintEffect.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: An effect to constrain the PHANToM to a point, line or plane
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_CONSTRAINT_EFFECT
#define GST_CONSTRAINT_EFFECT

#define GST_POINT_CONSTRAINT 0
#define GST_LINE_CONSTRAINT 1
#define GST_PLANE_CONSTRAINT 2

#include <math.h>
#include <gstDllExport.h>
#include <gstBasic.h>
#include <gstPlane.h>
#include <gstEffect.h>
#include <gstErrorHandler.h>

// Constraint effect for PHANToM.
// This effect constrains the PHANToM to a point,
// line or plane using a spring/damper system.  When
// this effect is started, its force effects 
// ramp up over three seconds so that no significant
// forces are immediately generated.
class GHOST_DLL_IMPORT_EXPORT gstConstraintEffect :public gstEffect
{
public:

	// Constructor.
	gstConstraintEffect():gstEffect(){
		constraint = GST_POINT_CONSTRAINT;
		springStiffness = 0.6;
		damping = 0.0;
		useAttenuation = TRUE;
	}

	// Destructor.
	~gstConstraintEffect() {}

	// Set damping coefficient.
	void	setDamping(double newDamping) {
		if (newDamping <= 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstInertiaEffect:setDamping Invalid < 0",newDamping);
			return;
		}
		damping = newDamping; 
	}

	// Get damping coefficient.
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

	// Set a point constraint.
	void setPoint(gstPoint newPoint) { 
		stop();
		pointConstraint = newPoint;
		constraint = GST_POINT_CONSTRAINT;
	}

	// Set a line constraint given a point on the line and
	// a vector.
	void setLine(gstPoint newPointOnLine, gstVector newLineDir) { 
		stop();
		pointOnLine = newPointOnLine;
		lineDirection = newLineDir;
		constraint = GST_LINE_CONSTRAINT;
	}

	// Set a planar constraint.
	void setPlane(gstPlane newPlane) { 
		stop();
		planeConstraint = newPlane;
		constraint = GST_PLANE_CONSTRAINT;
	}

	void setAttenuation(gstBoolean flag) {
		useAttenuation = flag;
	}

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
	virtual gstVector calcEffectForce(void *phantom);
	virtual gstVector calcEffectForce(void *phantom, gstVector &torques) {
		torques.init(0.0, 0.0, 0.0);
		return calcEffectForce(phantom);
	}

protected:

	double		springStiffness, damping;

	int			constraint;
	gstPoint	pointConstraint;

	gstPoint	pointOnLine;
	gstVector	lineDirection;
	
	gstPlane	planeConstraint;

	double		attenuation;
	gstBoolean	useAttenuation;
};



#endif // GST_CONSTRAINT_EFFECT
