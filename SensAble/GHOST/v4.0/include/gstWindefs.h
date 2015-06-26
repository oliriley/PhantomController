//=============================================================================
//   Filename : gstWindefs.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Basic Windows functions and defines
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_WINDEFS
#define GST_WINDEFS 


#ifdef _WIN32
#include <stdio.h>
#include <windows.h>
#pragma warning( disable : 4244 )

#pragma warning( disable : 4786 4804 )
// both of these are STL induced
// 4786 = identifier was truncated to '255' characters in the debug information
// 4804 = unsafe use of type 'bool' in operation

#define M_PI 3.14159265358979323846

void waitTillErrorMessageBoxGone();

void printToMessageBox(FILE *,const char *message,...);

#define fprintf2 printToMessageBox

#define GHOST_INLINE __forceinline

#endif

#endif

