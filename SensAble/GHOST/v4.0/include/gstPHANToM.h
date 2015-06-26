//=============================================================================
//   Filename : gstPHANToM.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents the PHANToM device in the scene graph
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_PHANTOM
#define GST_PHANTOM

#define MAX_COLLISIONS 10

#include <gstDllExport.h>
#include <gstDynamic.h>
#include <gstEffect.h>
#include <gstManipulator.h>
#include <gstPHANToMInfo.h>
#include <gstCollisionInfo.h>

// gstShape defined in gstBasic
//class gstShape;

class gstBoundary;
class gstBoundaryCube;
class gstPHANToM_SCP;

// Graphics callback data structure for gstPHANToM.
// This data structure replaces (i.e., is a superset of) 
// gstTransformDynamicCBData.
typedef struct GHOST_DLL_IMPORT_EXPORT _gstPHANToMGraphicsCBData {
    gstTransformMatrix transform;
    gstTransformMatrix cumulativeTransform;
    gstVector          reactionForce;          // Newtons
    gstVector          reactionTorque;
    gstVector          velocity;               // mm/s
    gstVector          acceleration;           // mm/s^2
    gstVector          angularVelocity;        // rad/s
    gstVector          angularAcceleration;    // rad/s^2
    gstBoolean         stylusSwitch;           // on/off
    gstBoolean         stylusPresence;         // on/off
    gstPoint           SCP;                    // Surface Contact Point wrt local ref frame
    gstPoint           SCP_WC;                 // Surface Contact Point wrt global ref frame
    gstVector          PHANToMForce;           // Newtons
    gstVector          PHANToMTorque;          // milliNewton-meters
} gstPHANToMGraphicsCBData;


// PHANToM haptic interface class.  This class is used to set and 
// access all the basic state of the PHANToM haptic interface.
// State information includes: 1) whether or not forces are enabled;
// 2) the position of the PHANToM; 3) attached boundary classes;  
// 4) PHANToM/object collision information; and 4) whether or not the
// PHANToM is in the haptic scene.  Two types of PHANToM position, as well
// as two reference frames for these positions, are available.  There is a 
// local reference frame (the default) and a world coordinate reference
// frame (used by any methods appended with "WC").  Information on
// both the "real" position of the PHANToM as well as information on 
// a Surface Contact Point (SCP) projection are available.
// NOTE: The PHANToM encoders are reset by default upon creation of a
// gstPHANToM object.  For more information on reseting the
// PHANToM, refer to the section of the GHOST Programming Guide
// pertaining to the gstPHANToM class.
class GHOST_DLL_IMPORT_EXPORT gstPHANToM : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstPHANToM, gstDynamic);

public:

    // Constructor.  Requires character string indicating
    // name of the PHANToM initialization file.  ResetEncoders
    // specifies if PHANToM encoders are to be reset to zero
    // when creating instance.  Encoders will be reset if TRUE,
    // otherwise encoders are not reset.  Default value is TRUE.
    gstPHANToM(char *configFile, int resetEncoders = TRUE);

    // Destructor.
    virtual ~gstPHANToM();

    // Resets the encoders on the PHANToM device.
    void resetEncoders();
    
    // FOR_GHOST_EXTENSION:
    // Call when object is added to scene graph.
    virtual void putInSceneGraph();

    // FOR_GHOST_EXTENSION:
    // Called when object is removed from scene graph.
    virtual void removeFromSceneGraph();

    // Get reaction force in world reference frame 
    // from PHANToM [Newtons].
    virtual gstVector getReactionForce_WC() {
        return parent->toWorld(reactionForce);
    }

    // Get reaction torque in world reference frame
    // from PHANToM [Newton*millimeters].
    virtual gstVector getReactionTorque_WC() {
        return parent->toWorld(reactionTorque);
    }
    
    // Returns TRUE is instance had no errors during construction.
    gstBoolean getValidConstruction() const {
        return validConstruction;
    }
    // Attach a boundary object (e.g. an object of gstBoundary
    // type such as gstBoundaryCube) to the PHANToM.  
    // The gstBoundary node should be IN the scene
    // graph, but only this instance of gstPHANToM will be able to
    // feel the boundary object.
    void setBoundaryObj(gstBoundary *newBoundaryObj) {
        boundaryObj = newBoundaryObj;
    }

    // Get currently attached boundary object.
    gstBoundary    *getBoundaryObj() { return boundaryObj; }

    // Attaches a maximally sized boundary object to the PHANToM
    // using the information retrieved from gstPHANToMInfoStruct
    gstBoundaryCube *attachMaximalBoundary();

    // Attaches a cube boundary object to the PHANToM
    // using the information retrieved from gstPHANToMInfoStruct
    // Note: Make sure the PHANToM has already been added to the scene graph
    // and has a parent node, or else this routine will not succeed.
    gstBoundaryCube *attachCubeBoundary();

    // Detaches the current boundary from the PHANToM and removes it from the scene
    gstBoundary *detachBoundary();

    // Get average servo-loop (PHANToM update) rate [Hz].
    double getAverageUpdateRate();

    // Get the time increment between the last two PHANToM
    // updates [seconds].
    double getDeltaT();

    // Get position of PHANToM in world coordinates. 
    // The origin of this object's
    // local reference frame is actually the PHANToM
    // position.
    void getPosition_WC(gstPoint &pt) {
        pt.init(0.0,0.0,0.0);
        pt = toWorld(pt);
    }

    // Get position of PHANToM in world coordinates. 
    // The origin of this object's
    // local reference frame is actually the PHANToM
    // position.
    virtual gstPoint getPosition_WC() {
        gstPoint pos;
        getPosition_WC(pos);
        return pos;
    }



    // Get previous position of PHANToM in world coordinates (i.e.,
    // position before previous call to gstPHANToM::update()).
    void getLastPosition_WC(gstPoint &pt) {
        pt = lastPosition_WC;
    }

    // Get current Surface Contact Point (SCP) in parent reference frame.
    void getSCP_P(gstPoint &pt) {
        if (parent != NULL) 
            pt = parent->fromWorld(SCP_WC);
        else
            fromWorld(SCP_WC);
    }

    // Get current Surface Contact Point (SCP) in world coordinates.
    void getSCP_WC(gstPoint &pt) {
        pt = SCP_WC;
    }

    // FOR_GHOST_EXTENSION:
    // Get previous position of PHANToM SCP in world coordinates (i.e.
    // position before previous call to gstPHANToM::update().
    void getLastSCP_WC(gstPoint &_lastSCP_WC) {
        _lastSCP_WC = lastSCP_WC;
    }


    // Set pointer to an SCP node.  The position of
    // newSCPNode will be set to the current SCP location
    // every call to gstPHANToM::update().
    void setSCPNode(gstPHANToM_SCP *newSCPNode) {
        SCPNode = newSCPNode;
    }

    // Get pointer of SCP node. 
    gstPHANToM_SCP *getSCPNode() {
        return SCPNode;
    }

    // Returns whether there is a dev fault.
    gstBoolean getStatus();

    // Returns TRUE if stylus switch is depressed, otherwise FALSE.
    // If no stylus is attached, output is undefined.
    gstBoolean getStylusSwitch() const { return stylusSwitch; }
        
    // Returns TRUE if stylus presence sensor is on, otherwise FALSE.
    // If no stylus is attached, output is undefined.
    gstBoolean getStylusPresence() const { return stylusPresence;}
    
    // Returns rotation angles of encoder gimbal in radians.  Angles are relative
    // to PHANToM tip not to base ref frame. 
    gstVector getGimbalAngles() {return gimbalAngles;}

    // Provides PHANToM setup info.
    // Contains information about the PHANToM configuration, like:
    // is6DOF, isDesktop, and PHANToM specific workspace dimensions
    const gstPHANToMInfoStruct *getInfo() { return &info; }
    
    // Get internal hardware rev number of phantom.
    // Only supported for desktop phantom
    int getHardwareRevision();

    // Different PHANTOM models can simulate different material
    // stiffnesses without causing buzzing or other artifacts.
    // The gain here is such that when multiplied by a normalized
    // stiffness of 1.0 it represent that max material
    // spring constant the PHANTOM can simulate
    double getMaxGain() const { return maxGain; }

    // See above for info about max gain.  The set function should
    // normally not be used since each PHANTOM model has a correct
    // max gain value as part of its configuration
    void setMaxGain(double newMaxGain)  { 
        maxGain = newMaxGain; 
    }
    
    // If flag is TRUE then forces will be turned on and sent to 
    // PHANToM when the simulation is active (i.e. the servo loop is running).  
        // Otherwise, forces will not be turned on nor used at any time.
    void setForceOutput(gstBoolean flag); 

    //    Returns TRUE if forces are to be used during simulation. Default is TRUE.
    gstBoolean getForceOutput() const { return _useForces; }

    // Gets estimated motor temperatures where 0 is room temperature and
    // 1.0 is max temperature before overheat.  Takes array of 6 floats
    // to get temperature for each motor.
    void getMotorOverheatState(float motorTemps[]);

    // FOR_GHOST_INTERNAL:
    // Turn forces on for this PHANToM.
    virtual int    forcesOn();

    // FOR_GHOST_INTERNAL:
    // Turn forcess off for this PHANToM.
    virtual int    forcesOff();

    // Returns the first PHANToM in the scene graph.  Use
    // getNextPHANToM to retrieve next PHANToM in
    // the scene graph.
    static    gstPHANToM *getPHANToMsInScene() {
        return gstPHANToMHead;
    }

    // Gets next PHANToM in the scene graph.
    gstPHANToM *getNextPHANToM() const {
        return nextPHANToM;
    }

    // Associate an special effect (i.e. from gstEffect class and sub-classes)
    // with the PHANToM.  If a previous effect
    // was in place, it is stopped before the new effect is added.
    void                setEffect(gstEffect *newEffect) {
        // Before we set a new effect, if we had an old effect then
        //  stop it first.
        stopEffect();
        effect = newEffect; }
    
    // Return current effect currently associated with the PHANToM, or NULL
        // if none exists.
    gstEffect *getEffect() const {
        return effect;
    }
    
    // Associate a manipulator (i.e. from gstManipulator class and sub-classes)
    // with the PHANToM.  If a previous manipulator exists, it is stopped 
    // before the new manipulator is added.
    void setManipulator(gstManipulator *newManipulator) {
        // Before we set a new effect, if we had an old effect then
        //  stop it first.
        if (manipulator != NULL)
            manipulator->stop();
        manipulator = newManipulator; }
    
    // Returns current manipulator currently associated with the PHANToM.
    gstManipulator *getManipulator() const {
        return manipulator;
    }
    
    // Start the effect (if any) associated with the PHANToM.
    gstBoolean startEffect() { 
        if (effect != NULL) effect->start();
        return TRUE;
    }
    
    // Stop the effect (if any) associated with the PHANToM.
    gstBoolean stopEffect() { 
        if (effect != NULL) effect->stop();
        return TRUE;
    }

    // Start the manipulator (if any) associated with the PHANToM.
    gstBoolean startManipulator() { 
        if (manipulator != NULL) manipulator->start();
        return TRUE;
    }
    
    // Stop the manipulator (if any) associated with the PHANToM.
    gstBoolean stopManipulator() { 
        if (manipulator != NULL) manipulator->stop();
        return TRUE;
    }
    
    // FOR_GHOST_INTERNAL:
    gstPoint scene_SCP;

gstInternal public:

    // FOR_GHOST_INTERNAL: 
    // Determines if any of the PHANToMs require a reset 
    static int IsResetNeeded(const char **configFiles, int num);

    // FOR_GHOST_INTERNAL:
    // Returns a bitwise information about the current calibration
    // for the PHANToM Desktop. This method will be useless if your
    // unit is not a PHANToM Desktop.
    int getCalibrationStatus();

    // FOR_GHOST_INTERNAL:
    // Performs an update on the current calibration for the PHANToM
    // Desktop. The servoloop must be inactive in order for this
    // update to be successful.
    void updateCalibration();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Returns TRUE if no PHANToMs have dev fault.
    static gstBoolean statusOfAll();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Enable forces for all PHANToMs in scene graph.
    static int enableAllForces();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Disable forces for all PHANToMs in scene graph.
    static int disableAllForces();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Update state (e.g. position, velocity) of all PHANToMs 
    // in the scene graph.
    static int updateAll();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Update the PHANToM.
    int update();
            
    // FOR_GHOST_INTERNAL:
    virtual int    setForce(const gstVector &force, const gstVector &torque);

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Set PHANToM forces and torques in world coordinate sytem.
    virtual int    setForce_WC(const gstVector &forceArg_WC, const gstVector &torqueArg_WC);
    
    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Set the SCP point in world coordinate reference frame.
    void setSCP_WC(const gstPoint &newSCP_WC) ;

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Set the "last" SCP point in world coordinate reference frame.
    void setLastSCP_WC(gstPoint &newLastSCP_WC) {
        lastSCP_WC = newLastSCP_WC;
    }

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Get last SCP and last SCPBeforeDynamicMove in world coordinate
    // reference frame.
    void getLastSCP_WC(gstPoint &_lastSCP_WC, 
        gstPoint &_lastSCPBeforeDynamicMove_WC) {
        _lastSCP_WC = lastSCP_WC;
        _lastSCPBeforeDynamicMove_WC = lastSCPBeforeDynamicMove_WC;
    }

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Get dynamic collision object.
    gstTransform *getDynamicCollisionObj() { return dynamicCollisionObj; }

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Get collision information structure for current collision.
    gstCollisionInfoStruct *getCollisionInfo() { return collisionInfo; }
    int getNumCollisionObjs() { return numCollisionObjs; }

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Get collision information structure for previous collision.
    gstCollisionInfoStruct *getLastCollisionInfo() { return lastCollisionInfo; }
    int    getLastNumCollisionObjs() { return lastNumCollisionObjs; }
    
    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Get collision information structure for current collision.
    gstCollisionInfoStruct *getCurrentCollisionInfo() {
        if (numCollisionObjs < maxCollisions)
            return &(collisionInfo[numCollisionObjs]);
        else
            return NULL;
    }

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Increments collision counter.  If a dynamic collision object does not
    // already exist, check to see if new one has a dynamic dependent.a
    gstBoolean addedCollision() {
        if (numCollisionObjs < maxCollisions) {
            if (dynamicCollisionObj == NULL) {
                if ( ((gstTransform *) collisionInfo[numCollisionObjs].obj)->getDynamicDependent() != NULL) {
                    dynamicCollisionObj = (gstDynamic *)
                        ((gstTransform *) collisionInfo[numCollisionObjs].obj)->getDynamicDependent();

                }
            }
            numCollisionObjs++;
            return TRUE;
        } else {
            // Too many collisions don't add this one.
            return FALSE;
        }
    }


    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // When this object or any object above it in the scene changes its local
    // transformMatrix, then this node's cumulative transform matrix is not valid
    // until it is recomputed based on the new data.  This function invalidates
    // the cumulative transform matrix for this node and its inverse.
    virtual void invalidateCumTransf();


    // FOR_GHOST_EXTENSION:
    // Used by system or for creating sub-classes only.
    // This function prepares data to be sent to a graphics callback.  When
    // gstScene::updateGraphics() is called by the application, gstScene stalls
    // the application process and--in the haptic process--calls this method for each 
    // node in the scene that has had graphics information changes since the last 
    // call to gstScene::updateGraphics().  When finished, the application process
    // continues by calling updateGraphics for all the same nodes.  updateGraphics()
    // actually calls the user graphics callback with the current graphics information
    // that was copied over in the calls to this method (prepareToUpdateGraphics()).  
    // The haptics process, therefore, is ONLY used to copy the current graphics 
    // information and the application process calls the callback functions.
    //
    // Each subclass of gstShape that passes additional data to this graphics callback 
    // must redefine this method and call <PARENTCLASS>::prepareToUpdateGraphics() 
    // before exiting.  In order to pass additional data to graphics callback, cbData 
    // must point to the new datatype that adds any additional fields.  These 
    // fields are then filled in by prepareToUpdateGraphics().
    virtual    void prepareToUpdateGraphics();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // Maximum number of collisions.
    static int maxCollisions;

        void   setLastKavg(double kavg) { lastKavg = kavg; }
        double getLastKavg()            { return lastKavg; }

        // Used internally to track SCP under various conditions
        gstPoint getLastFrictionSCP() const {return d_lastFrictionSCP;}
        void setLastFrictionSCP(const gstPoint &lastFrictionSCP)
        {
            d_lastFrictionSCP = lastFrictionSCP;
        }

        // Used internally to keep track of friction state
        gstBoolean getDynamicFriction() const {return d_dynamicFriction;}
        void setDynamicFriction(gstBoolean dynFriction)
        {
            d_dynamicFriction = dynFriction;
        }

protected:

    gstBoolean                _useForces;
    gstBoolean                validConstruction;
    static gstPHANToM        *gstPHANToMHead;
    int                        numCollisionObjs;
    gstCollisionInfoStruct    collisionInfo[MAX_COLLISIONS];
    int                        lastNumCollisionObjs;
    gstCollisionInfoStruct    lastCollisionInfo[MAX_COLLISIONS];

    gstPHANToMInfoStruct    info;
    int                        phantomId;
    gstPHANToM                *nextPHANToM;
    gstBoundary                *boundaryObj;
    gstDynamic                *dynamicCollisionObj;
    gstPoint                lastSCP_WC;
    gstPoint                lastSCPBeforeDynamicMove_WC;
    gstPoint                lastPosition_WC;
    gstPoint                SCP_DCOC;
    gstPoint                SCP, SCP_WC;
    gstPoint                lastPos;
    gstPHANToM_SCP            *SCPNode;
    gstEffect                *effect;
    gstManipulator            *manipulator;
    gstBoolean                forcesAreOn;
    gstBoolean                stylusSwitch;
    gstBoolean              stylusPresence;
    gstVector                gimbalAngles;
    gstVector                PHANToMForce;
    gstVector                PHANToMTorque;
    double                    maxGain;
    gstBoolean                resetSceneSCP;
        double                                  lastKavg;
        gstPoint                d_lastFrictionSCP;
        gstBoolean                d_dynamicFriction;

    // FOR_GHOST_INTERNAL:
    // This function resets collision object forces to zero to prepare for the next
    // servoLoop.  It also finds the first collision object that is dynamic, if one exists,
    // and transforms the lastSCP by the delta in movement that the dynamic obj underwent
    // the last servoLoop.  This tries to keep the SCP on the surface.  This breaks
    // if two collision objects move differently in one servoLoop.  
    // It is assumed that, for now, multiple simultaneous collisions with
    // moving objects are intersections of sub-objects of the same dynamic parent.
    void    prepareForNextLoop();

    // FOR_GHOST_INTERNAL:
    // This is a temporary fix that allows the error state of the hardware
    // to be reset. This functionality should ideally be moved down into PDD
    void resetError();

    // FOR_GHOST_INTERNAL:
    // This is a temporary fix that allows the auto-calibration sensor data
    // to be reset
    void resetAutoCalibration();

};

    
#endif
