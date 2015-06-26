//=============================================================================
//   Filename : gstScene.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Manages dynamic/haptic simulation and owns haptic
//              process or thread.  Also, arbitrates communication to
//              and from the haptic process or thread.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SCENE
#define GST_SCENE

#include <gstBasic.h>
#include <gstTransform.h>

typedef void (gstServoCallback) (void *pUserData);

// Holds haptic scene and mediates interaction with the PHANToM and servo loop.
class GHOST_DLL_IMPORT_EXPORT gstScene {

public:

    enum {
        SMOOTHING_OFF,
        SMOOTHING_ON
    };

    // Constructor.
    gstScene();

    // Performs a recursive delete on all nodes under the root
    ~gstScene();

    // lock() a gstScene before making changes to it if the servo loop
    // is running.  NOTE: Changes to the phantom or its ancestor nodes are
    // not supported when the servo loop is running, even if the scene is
    // locked.  Also, effects and manipulators will still be run even
    // if the scene is locked, so if this is incompatible with the
    // changes being made they should be stopped before making changes.
    // Finally, force fields are not enabled when the scene is locked.
    // returns TRUE if lock acquired, FALSE otherwise (if servo loop stopped)
    gstBoolean lock();

    // Call unlock() after a lock() to resume normal servo loop behavior.
    // NOTE: the scene should remain locked for only a few servo loop
    // iterations (each iteration is 1ms) to maintain an accurate haptic
    // simulation.
    void unlock();

    // In addition to the standard Ghost servoloop activity, you can hook in
    // your own callback function that will get called at servo rate.
    void setServoCallback(gstServoCallback *pCallback, void *pUserData)
    {
        m_pServoCallback = pCallback;
        m_pServoCBUserData = pUserData;
    }

    // Provide a callback to be called in the servoloop thread before the
    // first servoloop tick
    void setPreServoCallback(gstServoCallback *pCallback, void *pUserData)
    {
        m_pPreServoCallback = pCallback;
        m_pPreServoCBUserData = pUserData;        
    }

    // Provide a callback to be called in the servoloop thread following
    // the last servoloop tick
    void setPostServoCallback(gstServoCallback *pCallback, void *pUserData)
    {
        m_pPostServoCallback = pCallback;
        m_pPostServoCBUserData = pUserData;
    }

    // Set root node of haptic scene graph.
    void setRoot(gstTransform *newRoot);

    // Get root node of haptic scene graph.
    const gstTransform *getRoot() const { return rootNode; }
    gstTransform *getRoot() { return rootNode; }

    // FOR_GHOST_INTERNAL:
    // Turns ON forces of all PHANToMs in the scene graph.
    void turnForcesOn()
    {
        forcesOn = TRUE;
        firstLoop = TRUE;
    }

    // FOR_GHOST_INTERNAL:
    // Turns OFF forces of all PHANToMs in the scene graph.
    void turnForcesOff()
    {
        forcesOn = FALSE;
        firstLoop = TRUE;
    }

    // Set smoothing level for scene.  Smoothing attempts to 
    // remove high-frequency variations in gstPolyMesh geometries.
    // Currently only 0 (OFF) and 1 (ON) are supported.
    void setSmoothing(int newLevel) { smoothingLevel = newLevel; }

    // Get smoothing level of the scene.
    int getSmoothing() const { return smoothingLevel; }

    // Calls the graphics callbacks for all nodes in the scene
    // which have changed since the last call to updateGraphics().
    // This means nodes which have not changed will not have their
    // graphics callback called.
    void updateGraphics();

    // Calls the event callback for all nodes in the scene
    // which have new events since the last call to updateEvents().
    // Each callback is called once for each new event.
    void updateEvents();
         
    // Start the haptic simulation as separate process.
    // Control is returned immediately.
    int startServoLoop();

    // Clean up the haptic simulation after an internal error.
    void cleanupServoLoop();

    // Stop the haptic simulation.
    void stopServoLoop();

    // FOR_GHOST_INTERNAL:
    // Returns whether the haptics process is currently in
    //  the servoLoop.
    int getDoneOneLoop() const {return doneOneLoop;}

    // Returns TRUE if haptics process has finished (i.e.
    // if the servo loop is not running).
    int getDoneServoLoop() const { return doneServoLoop;}

    // Turns safety on and off.  If safety is off, the servo loop is
    // allowed to take as much CPU time as necessary to finish.
    // This may result in instability or crashes.  The default is TRUE (on).
    void setSafety(gstBoolean _s) { dutyCycleSafety = _s; }
  
    // Returns TRUE if safety limits are on.
    gstBoolean getSafety() const { return dutyCycleSafety; }

    // Sets the duty cycle limit in seconds that gets checked by the safety
    void setSafetyLimit(double limit) { dutyCycleLimit = limit; }

    // Returns the currently set duty cycle limit (sec)
    double getSafetyLimit() const { return dutyCycleLimit; }

gstInternal public:

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    // These are the main callback hooks for OSExtender to tick the servoloop
    int servoLoop();
    int preServoLoop();
    int postServoLoop();

    // FOR_GHOST_INTERNAL:
    // Used by system or for creating sub-classes only.
    void setDoneOneLoop(gstBoolean newVal) {doneOneLoop = newVal;}

protected:

    gstTransform *rootNode;

    // These callback function pointers can be set to extend the standard
    // hooks into the servoloop thread.
    gstServoCallback *m_pServoCallback;
    void *m_pServoCBUserData;

    gstServoCallback *m_pPreServoCallback;
    void *m_pPreServoCBUserData;

    gstServoCallback *m_pPostServoCallback;
    void *m_pPostServoCBUserData;

    int preparingGraphics, preparingEvents;    
    int doneServoLoop, doneOneLoop;

    int firstLoop, forcesOn;
    gstBoolean checkStatus;

    gstBoolean needLock, locked;

    int dynamicF;
    double dynamicFrictionAvg, staticFrictionAvg, surfaceDampingAvg;
    double Kfriction;

    int smoothingLevel;

    gstBoolean dutyCycleSafety;
    double dutyCycleLimit;
};



#endif // GST_SCENE
