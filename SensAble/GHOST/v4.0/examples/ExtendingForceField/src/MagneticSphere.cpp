//*****************************************************************************
//      Filename : MagneticSphere.cpp
// ----------------------------------------------------------------------------
// ForceField derived class that represents a sphere with magnetic
// attraction.
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include <MagneticSphere.h>
#include <gstPHANToM.h>

gstType	MagneticSphere::MagneticSphereClassTypeId;

// Constructor.
MagneticSphere::MagneticSphere(double sphereRadius, double fieldRadius)
: msphereRadius(sphereRadius),
  mfieldRadius(fieldRadius)
{
    boundBySphere(gstPoint(0.0,0.0,0.0), mfieldRadius);
}

// gstForceField method overloaded to define the force applied by the
// PHANToM when the  PHANToM has entered the bounding volume associated
// with the force field
gstVector MagneticSphere::calculateForceFieldForce(gstPHANToM *phantom)
{
    // Convert phantom pos to local coords
    gstPoint pos = fromWorld(phantom->getPosition_WC());

    // stiffness constant
    const double k = 0.05;

    // vector from phtm pos to center
    gstVector v = gstPoint(0,0,0) - pos; 

    // f = kx for sphere
    return k * (v.norm() - msphereRadius) *  (1.0/v.norm()) * v;
}

gstVector MagneticSphere::calculateForceFieldForce(gstPHANToM *phantom,
						   gstVector &torque)
{
    torque = gstVector(0, 0, 0);

    return calculateForceFieldForce(phantom);
}






