//=============================================================================
//   Filename : gstPHANToMInfo.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised : Brandon Itkowitz
// ----------------------------------------------------------------------------
// Description: Structures containing information about specific PHANToM models
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_PHANTOM_INFO_H_
#define _GST_PHANTOM_INFO_H_

#include <gstBasic.h>

// Workspace dimensions
struct GHOST_DLL_IMPORT_EXPORT gstDimensionsStruct {
    int xMin, xMax;
    int yMin, yMax;
    int zMin, zMax;
};

// Information about a particular PHANToM model
struct GHOST_DLL_IMPORT_EXPORT gstPHANToMInfoStruct {
    // properties about the phantom hardware
	gstBoolean hasGimbal;
    gstBoolean is6DOF;      // determines whether this phantom has 3DOF or 6DOF capabilities
    gstBoolean isDesktop;   // determines whether encoders need to be reset

    // workspace dimensions based on the phantom model type or registry settings
    gstDimensionsStruct maxWorkspace;       // some extents may not be reachable
    gstDimensionsStruct maxUsableWorkspace; // maximizes reachable extents for a rectangular workspace
    gstDimensionsStruct cubeWorkspace;      // maximizes reachable extents for a cube workspace

    int tableTopOffset;     // offset in the y direction from the reset position
};

#endif // _GST_PHANTOM_INFO_H_
