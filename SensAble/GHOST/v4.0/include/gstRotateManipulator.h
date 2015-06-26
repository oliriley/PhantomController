//=============================================================================
//   Filename : gstRotateManipulator.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Rotate Manipulator to allow gstPHANToM to rotate a node
//              with feedback force sent back to gstPHANToM.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_ROTATE_MANIPULATOR
#define GST_ROTATE_MANIPULATOR

#include <gstBasic.h>
#include <gstQuaternions.h>
#include <gstManipulator.h>


// Rotational manipulator.  This uses the PHANToM to rotate a 
// node, with force feedback.  When active, the manipulator confines
// the PHANToM to a sphere around the object.  The PHANToM also must exceed
// a threshold force before the manipulator allows the object to be moved.
// The force feedback is that of a simulated rotating mass with rotational
// damping.
class GHOST_DLL_IMPORT_EXPORT gstRotateManipulator: public gstManipulator
{
    
public:
    
    // Constructor.
    gstRotateManipulator() {
        manipSpringK = 0.5;
        manipMass = .01;
        manipDamping = 0.001;
        resetState();
    }
    
    // Destructor.
    ~gstRotateManipulator() {}
    
    // Set rotational damping of simulated rotating mass [Kg/(1000.0*sec)].
    void setDamping(double _d) { manipDamping = _d; };
    // Get rotational damping of simulated rotating mass [Kg/(1000.0*sec)]
    double getDamping() { return manipDamping; };
    
    // Set mass of rotate manipulator [kilograms].
    void setMass(double mass) { manipMass = mass; }
    
    // Get mass of rotate manipulator [kilograms].
    double getMass() const { return manipMass; }
    
    // Set stiffness of spring attached from gstPHANToM to rotating object [Kg/(1000.0*sec^2)].
    void setSpringK(double springK) { manipSpringK = springK; }
    
    // Get stiffness of spring attached from gstPHANToM to rotating object [Kg/(1000.0*sec^2)].
    double getSpringK() const { return manipSpringK; }
    
    // Resets all dynamic state (e.g. acceleration, velocity) of manipulator.
    virtual void resetState() {
        manipAccel.init(0.0,0.0,0.0);
        manipVel.init(0.0,0.0,0.0);
        angularVel.init(0.0,0.0,0.0);
        //L.init(0.0,0.0,0.0);
        L.identity();
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
    gstQuaternion		q;  // rotation quaternion
    gstTransformMatrix    L; // angular momentum
    gstTransformMatrix    I; // inertia matrix;
    gstTransformMatrix    Iinv; // inverse of I
    gstQuaternion qL;
    gstVector		angularVel;
    gstVector		manipVel, manipAccel;
    double			manipSpringK;
    double			manipDamping;
    double			manipMass;
};



#endif // GST_SCALE_MANIPULATOR
