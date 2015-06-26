//*****************************************************************************
//      Filename : ViscEffect.cpp
// ----------------------------------------------------------------------------
// Viscosity effect.  Generates a force opposite of 
// velocity to simulate a viscous field.
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include "ViscEffect.h"
#include <gstPHANToM.h>

gstVector ViscEffect::calcEffectForce(void *phantom)
{
    // No force if inactive
    if (!active) 
    {
        return gstVector(0,0,0);
    }

    // Force = -k * velocity of phantom
    gstPHANToM *PHANToM = (gstPHANToM *)phantom;
    gstVector v = PHANToM->getVelocity();
    return -m_k * v;
}

gstVector ViscEffect::calcEffectForce(void *phantom, gstVector & torques) 
{
    // Not using torque
    torques = gstVector(0,0,0);
    return calcEffectForce(phantom);
}
