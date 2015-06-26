//*****************************************************************************
//      Filename : PhantomContext.h
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef _PHANTOM_CONTEXT_H_
#define _PHANTOM_CONTEXT_H_

#ifdef _WIN32
#pragma warning(disable : 4305)     // Disable data conversion warnings
#endif

#include <reset_screen.h>
#include "HapticFrustum.h"

//*****************************************************************************
//                             C o n s t a n t s 
//*****************************************************************************

#define PHANTOM_NAME        "Default PHANToM"
#define PHANTOM_PARENT_NAME "PHANToM Parent"
#define GEOM_SEP_NAME	    "Scene Geometry"

#define FORCE_THRESHOLD     2.0         // Force units are in Newtons

#define NEAR_PLANE_OFFSET   15
#define FAR_PLANE_OFFSET    15

//*****************************************************************************
//                  F u n c t i o n   P r o t o t y p e s 
//*****************************************************************************
void start_program(int bResetPHANToM);
void end_program(void);

#endif
