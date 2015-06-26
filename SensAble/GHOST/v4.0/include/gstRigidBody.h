//=============================================================================
//   Filename : gstRigidBody.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Rigid body dynamic node.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================


#ifndef GST_RIGID_BODY
#define GST_RIGID_BODY

#include <gstDllExport.h>
#include <gstDynamic.h>
#include <gstQuaternions.h>

#define BUTTON_RESTFORCE 0.1
#define BUTTON_DEADBAND 5.0
#define BUTTON_THROWDIST 20.0
#define BUTTON_K 0.00007

#define BUTTON_MASS 0.5
#define BUTTON_DAMPING 0.5

//	Rigid body dynamic.  Simulates an inertial body containing
//  both rotational and linear inertia.  Mass and damping are
//  inherited from gstDynamic.
class GHOST_DLL_IMPORT_EXPORT gstRigidBody : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstRigidBody, gstDynamic);

public:

	// Constructor.
	gstRigidBody();

	// Destructor.
	~gstRigidBody() {}

	// set gravity (acceleration) [mm/sec^2].
    // Gravity is in world coordinates
	// Default is 0.0,0.0,0.0    
	void	setGravity(gstVector &newGravity) {
		gravity = newGravity;
	}

gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// When body has velocity or a force, this method
	// does Euler integration of rigid body state state.
	void updateDynamics();

protected:

	double				Ibody[3][3];	// Inertia Matrix
	gstVector			L;				// Angular Momentum
	gstVector			gravity;		// Gravity
  gstQuaternion			q, qDot, qL;
};

#endif // RIGID_BODY
