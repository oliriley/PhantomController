//=============================================================================
//   Filename : gstManipulator.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Base class for manipulators, for generating forces to be added 
//              to the PHANToM while altering the scene
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_MANIPULATOR
#define GST_MANIPULATOR

#include <gstBasic.h>
#include <gstTransform.h>

// Base class for PHANToM manipulator class.
class  GHOST_DLL_IMPORT_EXPORT gstManipulator {

public:

    // Destructor.
    virtual ~gstManipulator() {}

    // FOR_GHOST_EXTENSION:
    // Start the manipulator.  WARNING:  When re-starting a manipulator,
    // make sure to reset any state, such as past PHANToM position.
    // Otherwise, the next call to calcEffectForce could 
    // generate unexpectedly large forces.
    virtual void start() { 
        active = TRUE;
        if (manipNode != NULL) {
            manipNode->setTouchableByPHANToM(FALSE);
        }
    }

    // FOR_GHOST_EXTENSION:
    // Stop the effect and reset state.
    virtual void stop() { 
        active = FALSE; resetState();
        if (manipNode != NULL) {
            manipNode->setTouchableByPHANToM(TRUE);
        }
    }


    // Set "newNode" to be the node in the scene that the 
    // manipulator modifies.
    void setNode(gstTransform *newNode) {
        manipNode = newNode;
    }

    // Get node that manipulator modifies.
    gstTransform *getNode() const { return manipNode; }

    // Returns TRUE if manip is currently active.
    gstBoolean isActive() const { return active; }
  
    // If TRUE then the inverse value of the stylus
    // switch is used to activate the manipulator.
    // Otherwise, depressing the switch activates the
    // manipulator.
    void setInvertStylusSwitch(gstBoolean flag) {
        invertStylusSwitch = flag;
    }

    gstBoolean getInvertStylusSwitch() const {
        return invertStylusSwitch;
    }

    // Set whether the stylus switch is used to start
    // and stop manipulators.  If TRUE, the manipulator
    // is started when the stylus switch is pressed and
    // remains active until the switch is released.
    // Default is FALSE.
    void setUseStylusSwitch(gstBoolean _use) {
      useStylusSwitch = _use;
    }
    
    // FOR_GHOST_EXTENSION:
    // Resets all dynamic state of manipulator.
    virtual void resetState() {}

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
    //            PHANToM->setForce_WC from this function.
    //            It will cause an infinite recursion.
    virtual gstVector calcManipulatorForce(void *PHANToM) {
        if (PHANToM) {}     // To remove compiler warning
        return gstVector(0.0,0.0,0.0);
    }

    virtual gstVector calcManipulatorForce(void *PHANToM, gstVector &torques) {
        torques.init(0.0, 0.0, 0.0);
        return calcManipulatorForce(PHANToM);
    }

protected:

    // This class is intended as a base class only, the constructor
    // is protected so that instances can not be created.
    gstManipulator() {
        useStylusSwitch = FALSE;
        invertStylusSwitch = FALSE;
        manipNode = NULL;
        active = FALSE;
        last_active = FALSE;
        resetState();
    }

    gstBoolean      active;         // FOR_GHOST_EXTENSION:
                                    // TRUE if manipulator is active.
                                    
    gstTransform    *manipNode;      // FOR_GHOST_EXTENSION:
                                    // Pointer to node to be manipulated.

    gstBoolean      invertStylusSwitch;
    gstBoolean      useStylusSwitch;    // FOR_GHOST_EXTENSION:
                                        // TRUE if stylus switch is to be used.
    gstBoolean      last_active;
    gstPoint        manipStuckPos;
};

#endif // GST_MANIPULATOR
