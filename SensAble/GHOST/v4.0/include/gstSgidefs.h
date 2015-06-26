//=============================================================================
//   Filename : gstSgidefs.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Defines and includes needed for SGI/IRIX
//
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SGIDEFS
#define GST_SGIDEFS 

#ifdef SGI

#define UINT uint
#define fprintf2 fprintf
#include <unistd.h>
#undef Sleep
#define Sleep sginap

#define GHOST_INLINE inline

#endif  // SGI

#endif  // GST_SGIDEFS
