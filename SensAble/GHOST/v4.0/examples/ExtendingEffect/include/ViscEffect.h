//*****************************************************************************
//      Filename : ViscEffect.h
// ----------------------------------------------------------------------------
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef VISC_EFFECT_INCLUDE
#define VISC_EFFECT_INCLUDE

#include <gstEffect.h>

// Viscosity effect.  Generates a force opposite of 
// velocity to simulate a viscous field.
class  ViscEffect : public gstEffect 
{
public:

    // Constructor.
    ViscEffect() : m_k(0.001) {}

    // Calculate force for effect at each servo tick.
    virtual gstVector calcEffectForce(void *phantom); 
    virtual gstVector calcEffectForce(void *phantom, gstVector &torques); 

    // Get/Set k coefficient for viscosity with f = -kv
    void setK(double k) { m_k = k; }
    double getK() { return m_k; }

private:
    double m_k;
};

#endif // VISC_EFFECT_INCLUDE
