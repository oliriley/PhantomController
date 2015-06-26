//  -*- C++ -*-
// Copyright 1996-1999, SensAble Technologies, Inc.
//  file: gstSundefs.h


/*
_______________________________________________________
_____________ SensAble Technologies, Inc. _____________
_______________________________________________________
__
__	Rev:			1.0
__
__  Class:			
__
__	Description:	Defines and includes needed for Sun
__
__	Authors:		Chris Tarr	
__
__	Revision:		W. A. Aviles	Rev 1.0	
_______________________________________________________
_______________________________________________________
*/

#ifndef GST_SUNDEFS
#define GST_SUNDEFS 

#ifdef sun

#define UINT uint
#define fprintf2 fprintf
#include <unistd.h>
#undef Sleep
#define Sleep(s) usleep((s) * 1000)

#define GHOST_INLINE inline

#endif  // sun

#endif  // GST_SUNDEFS
