//*****************************************************************************
//      Filename : globals.h
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "PhantomContext.h"

//*****************************************************************************
//                     G l o b a l   V a r i a b l e s 
//*****************************************************************************
 
extern gstPHANToM  *gPhantom;	 /* a global instance of the PHANToM */
extern gstScene	   *gScene;	 /* a global instance of the scene graph */
extern gstBoolean  PHANToMReset; /* TRUE if PHANToM encoders must be reset. */

#endif
