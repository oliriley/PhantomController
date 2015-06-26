//=============================================================================
//     Filename : RobotFramework.h
//   Written by : Brandon Itkowitz
//     Contains : Example GhostGL Application w/ Transforms & Color
// ----------------------------------------------------------------------------
// This program haptically and graphically renders the upper body of a robot.
// The robot is built using gstShape primitives and gstSeparator nodes.
// The graphics are managed by GhostGL.
// Additionally, this example shows how to modify OpenGL rendering properties
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef ROBOT_FRAMEWORK_H
#define ROBOT_FRAMEWORK_H

#ifdef _WIN32
#pragma warning(disable : 4305) // Disable data conversion warnings
#endif

// OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>

#define PHANTOM_NAME 	"Default PHANToM"  // PHANToM configuration string
#define WORKSPACE_NAME	"WorkspaceBounds"
#define ROBOT_NAME	    "Robby"

//=============================================================================
//                     F u n c t i o n  P r o t o t y p e s
//=============================================================================

void start_program(int bResetPHANToM);
void end_program(void);

#endif // ROBOT_FRAMEWORK_H
