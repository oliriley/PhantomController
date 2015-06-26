//=============================================================================
//   Filename : gstConstantForceField.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz, Chris Tarr, Tim Gallagher	
//    Revised : Ted Bardasz
// ----------------------------------------------------------------------------
// Description: A gstForceField to put a constant force on the PHANToM
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_CONSTANT_FORCE_FIELD
#define GST_CONSTANT_FORCE_FIELD

#include <gstDllExport.h>
#include <gstVector.h>
#include <gstForceField.h>

// A particular type of force field.
// When the user enters the bounding volume associated with this force
// field, they will feel a force vector as set by setConstantForceVector
class GHOST_DLL_IMPORT_EXPORT gstConstantForceField : public gstForceField
{
    GHOST_INHERIT_HEADER(gstConstantForceField, gstForceField);

public:
	
	// Constructor.
	gstConstantForceField();

	// Constructor.
	gstConstantForceField(gstVector &force);

	// Constructor.
	gstConstantForceField(gstVector &force, gstVector &torque);

	// Constructor.
	gstConstantForceField(const gstConstantForceField *);

	// Constructor.
	gstConstantForceField(const gstConstantForceField &);

	// Destructor.
	virtual ~gstConstantForceField();

	// Clone.
	virtual gstNode *Clone() const {return CloneForceField();}
	gstConstantForceField *CloneForceField() const {return (new gstConstantForceField(*this));}

	// Method to set the force vector that the user will feel whenever the 
	// PHANToM enters the bounding volume associated with this force field
	void setConstantForceVector(const gstVector &force) {
		mConstantForceVector = force;
	}

	// Method to get the force vector that the user will feel whenever the 
	// PHANToM enters the bounding volume associated with this force field
	gstVector getConstantForceVector() const{
		return mConstantForceVector;
	}

	// Method to set the torque vector that the user will feel whenever the 
	// PHANToM enters the bounding volume associated with this force field
	void setConstantTorqueVector(const gstVector &torque) {
		mConstantTorqueVector = torque;
	}

	// Method to get the torque vector that the user will feel whenever the 
	// PHANToM enters the bounding volume associated with this force field
	gstVector getConstantTorqueVector() const{
		return mConstantTorqueVector;
	}

	// gstForceField method overloaded to define the force applied by the PHANToM when the 
	// PHANToM has entered the bounding volume associated with the force field
	virtual gstVector calculateForceFieldForce(gstPHANToM *phantom);

	// gstForceField method overloaded to define the forces and torques applied 
	// by the PHANToM when the PHANToM has entered the bounding volume associated 
	// with the force field
	virtual gstVector calculateForceFieldForce(gstPHANToM *phantom, gstVector &torque);

private:

    gstVector mConstantForceVector;
	gstVector mConstantTorqueVector;
};

#endif
