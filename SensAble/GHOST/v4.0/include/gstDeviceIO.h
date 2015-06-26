//=============================================================================
//   Filename : gstDeviceIO.h
// ----------------------------------------------------------------------------
// Description: Class giving low-level access to the PHANTOM.  Can be used to 
//              create own servo loop, access encoders and motors, etc.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-2002 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef __gstBasicIO_h__
#define __gstBasicIO_h__

#include <gstPoint.h>
#include <gstVector.h>
#include <gstTransformMatrix.h>

// Error codes returned by gstDeviceIO functions
#define PHTM_OK                     0
#define PHTM_WARM_MOTORS            -1
#define PHTM_MAX_FORCE              -2
#define PHTM_BAD_CHANNEL_SELECT     -3
#define PHTM_DEV_FAULT              -4
#define PHTM_BAD_FILE               -5
#define PHTM_BAD_FILE_NAME          -6
#define PHTM_MEMORY_ERROR           -8
#define PHTM_DRIVER_ERROR           -10
#define PHTM_BAD_ID                 -11
#define PHTM_FORCES_DISABLED        -12
#define PHTM_EXCEEDED_MAX_VELOCITY  -13
#define PHTM_ILLEGAL_UPDATE_RATE    -14
#define PHTM_ILLEGAL_MAX_VELOCITY   -15
#define PHTM_NO_RESET               -16
#define PHTM_SAFETY_DRIVER_ERROR    -17
#define PHTM_BAD_ADDRESS            -18
#define PHTM_NO_MMAP                -19
#define PHTM_NO_CARRIER             -20
#define PHTM_CARD_IN_USE            -21
#define PHTM_BAD_CARD_TYPE          -22
#define PHTM_NO_CARD                -23
#define PHTM_BAD_SLOT               -24
#define PHTM_NO_DRIVER              -25
#define PHTM_NO_TIMER               -26
#define PHTM_EVENT_NOT_OWNED        -27
#define PHTM_COMM_ERROR             -28
#define PHTM_EPP_CONFIG_ERROR       -29
#define PHTM_COMM_CONFIG_ERROR      -30

// API functions

// Initializes the Phantom - must call before you do anything else with the 
// phantom, returns a phantom ID if successful, and one of the error codes
// above if not.
GHOST_DLL_IMPORT_EXPORT int     gstInitializePhantom(char *phantomName);

// Disables the phantom whose id is given.  
GHOST_DLL_IMPORT_EXPORT int     gstDisablePhantom(int id);

// sets the phantom encoders to zero.  Should be used by all phantoms
// that require a reset.
GHOST_DLL_IMPORT_EXPORT int     gstResetPhantomEncoders(int id);

// returns whether the given phantom id is a phantom type that requires
// a reset or not.
GHOST_DLL_IMPORT_EXPORT bool     gstIsPhantomResetNeeded(int id);

// enables the amplifiers so that forces can be sent to the phantom.
// (typically done once at the beginning)
GHOST_DLL_IMPORT_EXPORT int     gstEnablePhantomForces(int id);

// disables the amplifiers so the Phantom stops sending forces
GHOST_DLL_IMPORT_EXPORT int     gstDisablePhantomForces(int id);

// gets the gstPhantomInfoStruct, which contains various data on the current Phantom.
GHOST_DLL_IMPORT_EXPORT int     gstGetPhantomInfo(int id, gstPHANToMInfoStruct& pInfo);

// gets the MaxStiffness for the phantom whose ID is given. MaxStiffness should be
// used to scale all phantom stiffnesses, if you plan to use your code with 
// multiple phantom models.
GHOST_DLL_IMPORT_EXPORT float   gstGetPhantomMaxStiffness(int id);

// return a device fault (but not other phantom error states)
// It makes more sense to check the return state of updatePhantom to find out
// what the current error state is.
GHOST_DLL_IMPORT_EXPORT int     gstGetPhantomError(int id);

// updates the Phantom's internal picture of itself by reading encoders, calculating
// position, etc.  Should call each servo loop (if creating own).
GHOST_DLL_IMPORT_EXPORT int     gstUpdatePhantom(int id);

// sends a force to the Phantom motors. Use this version for 6dof.
// the force vector is cartesian forces, the torque vector is joint torques
// for the 6dof axes.
GHOST_DLL_IMPORT_EXPORT int     gstSetPhantomForce(int id, 
                           const gstVector& force, 
                           const gstVector& torque);

// Sends a force to the phantom motors. Use this version for non-6dof.
GHOST_DLL_IMPORT_EXPORT int     gstSetPhantomForce(int id,
                           const gstVector& force);

// gets the Phantom position in cartesian space.
GHOST_DLL_IMPORT_EXPORT int     gstGetPhantomPosition(int id, gstPoint& pos);

// gets the phantom velocity in cartesian space.  Will only work if the update rate
// is greater than 900 Hz.  Otherwise, returns last known "good" velocity.
GHOST_DLL_IMPORT_EXPORT int     gstGetPhantomVelocity(int id, gstVector& vel);

// gets the instantaneous phantom update rate.
GHOST_DLL_IMPORT_EXPORT float   gstGetPhantomUpdateRate(int id);

// gets the state of the stylus switch.
GHOST_DLL_IMPORT_EXPORT int     gstGetStylusSwitchState(int id);

// gets the Phantom temperature (according to internal temp model.)
// returns 6 values (3 will be zero for non-6dofs)
GHOST_DLL_IMPORT_EXPORT int     gstGetPhantomTemperature(int id, float temp[]);

// gets the transform matrix representing the stylus position and orientation
GHOST_DLL_IMPORT_EXPORT int     gstGetStylusMatrix(int id, gstTransformMatrix& mat);

// gets the phantom joint angles. order: (left(+); up(+); out(+))
GHOST_DLL_IMPORT_EXPORT int     gstGetJointAngles(int id, gstVector& theta);

// gets the phantom stylus joint angles. order: (right(+), up(+), right(+))
GHOST_DLL_IMPORT_EXPORT int     gstGetStylusJointAngles(int id, gstVector& theta);

// gets the raw encoder values (order: the 3 base encoders, then the 3 gimbal encoders)
GHOST_DLL_IMPORT_EXPORT int     gstGetRawEncoderValues(int id, long encoders[]);

// performs Phantom kinematics to convert encoder values to a stylus transform.
// this does not affect the phantom's intenral picture of itself - if you've
// changed the encodrs so that you get a different answer, you may get weird results.
GHOST_DLL_IMPORT_EXPORT int     gstEncodersToTransform(int id, long encoders[], gstTransformMatrix& mat);

// intialization necessary to start a servo loop
GHOST_DLL_IMPORT_EXPORT int     gstInitServoScheduler();

// typedefs for defining your servo loop callback function. see example code.
typedef int (WINAPI gstServoSchedulerCallback) (void *pUserData); 
#define GST_SCHEDULER_CALLBACK int WINAPI

// start the servo loop whose callback is given here
GHOST_DLL_IMPORT_EXPORT  int     gstStartServoScheduling(gstServoSchedulerCallback pCallback, void *userData);

// stop the currently running servo loop.
GHOST_DLL_IMPORT_EXPORT void     gstStopServoScheduling();

#endif  // __gstDeviceIO_h__
