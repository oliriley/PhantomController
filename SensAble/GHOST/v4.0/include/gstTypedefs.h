//=============================================================================
//   Filename : gstTypedefs.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Basic typdefs and defines
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TYPEDEFS
#define GST_TYPEDEFS

// Used for internal class members
#define gstInternal

// Type definitions
typedef int gstType;
typedef int gstEventType;
typedef int gstBoolean;

// Global definitions
#define GST_DISTANCE_EPSILON 0.001
#define GST_EPSILON 0.0000000001
#define MAX_DAMPING 100.0
#define MAX_SURFACE_DAMPING 20.0
#define MAX_DYNAMIC_FRICTION 20.0
#define MAX_STATIC_FRICTION 20.0

// Meters to millimeters macro
#define MtoMM(x) (1000.0*(x))

// Millimeters to meters macro
#define MMtoM(x) (0.001*(x))

// Boolean definitions
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#endif
