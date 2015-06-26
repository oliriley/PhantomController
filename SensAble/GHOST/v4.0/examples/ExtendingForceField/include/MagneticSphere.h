//*****************************************************************************
//      Filename : MagneticSphere.h
// ----------------------------------------------------------------------------
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef MAGNETIC_SPHERE_INCLUDE
#define MAGNETIC_SPHERE_INCLUDE

#include <gstForceField.h>

// ForceField derived class that represents a sphere with magnetic
// attraction.
class MagneticSphere : public gstForceField 
{
public:
	
    // Constructor.
    MagneticSphere(double sphereRadius, // radius of sphere
                   double fieldRadius); // radius of attracting field

    // Standard GHOST SDK node methods
    // Must be defined for all derived classes.

    // Get type of this class.  No instance needed.
    static gstType getClassTypeId()
    {
        gstAssignUniqueId(MagneticSphereClassTypeId);
	return MagneticSphereClassTypeId;
    }

    // Get type of this instance.
    virtual gstType getTypeId() const { return getClassTypeId(); }

    // Returns TRUE if this class is same or derived class of type.
    virtual gstBoolean isOfType(gstType type) const {return staticIsOfType(type);}

    // Returns TRUE if subclass is of type.
    static gstBoolean staticIsOfType(gstType type)
    {
        if (type == getClassTypeId()) return TRUE;
	else return (gstForceField::staticIsOfType(type));
    }

    // gstForceField method overloaded to define the force applied by
    // the PHANToM when the  PHANToM has entered the bounding volume
    // associated with the force field
    virtual gstVector calculateForceFieldForce(gstPHANToM *phantom);
    virtual gstVector calculateForceFieldForce(gstPHANToM *phantom,
                                               gstVector &torque);

private:
	static gstType MagneticSphereClassTypeId;

	double msphereRadius, mfieldRadius;
};

#endif

