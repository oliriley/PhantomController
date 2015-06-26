//=============================================================================
//   Filename : gstErrorHandler.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Ways to process/handle errors from GHOST
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_ERROR_HANDLER
#define GST_ERROR_HANDLER

#include <gstDllExport.h>
#include <gstTypedefs.h>
#include <gstVector.h>
#include <PHANTOMErrors.h>

typedef void (gstErrorCallback) (int, char *, void *);

// Error codes
#define GST_ERROR_BASE                  100                     // avoid overlap between GST and PHTM error codes              
#define GST_OUT_OF_RANGE_ERROR          -(GST_ERROR_BASE + 1)
#define GST_INVALID_SCALE               -(GST_ERROR_BASE + 2)   // cannot perform non-uniform scaling
#define GST_ALLOC_MEMORY_ERROR          -(GST_ERROR_BASE + 3)
#define GST_CHILD_NOT_FOUND             -(GST_ERROR_BASE + 4)
#define GST_BOUNDS_ERROR                -(GST_ERROR_BASE + 5)
#define GST_MISC_ERROR                  -(GST_ERROR_BASE + 6)
#define GST_NESTED_DYNAMICS_ERROR       -(GST_ERROR_BASE + 7)
#define GST_PHANTOM_INIT_ERROR          -(GST_ERROR_BASE + 8)
#define GST_ALGEBRAIC_SURFACE_MAX_ITER  -(GST_ERROR_BASE + 9)
#define GST_PHANToM_MAX_COLLISIONS      -(GST_ERROR_BASE + 10)
#define GST_TIMER_ERROR                 -(GST_ERROR_BASE + 11)
#define GST_INVALID_TRANSFORM           -(GST_ERROR_BASE + 12)
#define GST_DUTY_CYCLE_ERROR            -(GST_ERROR_BASE + 13)
#define GST_PHANTOM_FORCE_ERROR         -(GST_ERROR_BASE + 14)
#define GST_PHANTOM_ERROR               -(GST_ERROR_BASE + 15)
#define GST_TRANSFORM_RESET_ERROR       -(GST_ERROR_BASE + 16)

static gstBoolean printErrorMessage = TRUE;

// Retrieve generic error message string.
GHOST_DLL_IMPORT_EXPORT char *getErrorMessage(int errorId);

// Check whether the error is recoverable 
GHOST_DLL_IMPORT_EXPORT gstBoolean IsRecoverableError(int errorId);

// If set to TRUE then error messages will be automatically printed.
//  Otherwise, user must print error messages using errorCallback.
GHOST_DLL_IMPORT_EXPORT void printErrorMessages(gstBoolean flag);


// Set user error callback.  This callback is called for each error.
// The parameters passed in are the error number, character string
// description of error, and pointer to user data.
GHOST_DLL_IMPORT_EXPORT void setErrorCallback(gstErrorCallback *newCallback,
                                              void *userdata);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, int errorValue);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, void *errorValue);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, unsigned int errorValue);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, double errorValue);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, gstVector errorValue);

// Generic error handler.  Prints error message to stderror.
GHOST_DLL_IMPORT_EXPORT void
gstErrorHandler(int errorId, char *quote, gstPoint errorValue);

#endif
