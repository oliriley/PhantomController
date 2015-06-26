//=============================================================================
//   Filename : RobotFramework_graphics.cpp
// Written by : Brandon Itkowitz
//    Project : Example GhostGL Application w/ Transforms & Color
//     Module : Platform Independent Graphics
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef ROBOT_FRAMEWORK_GRAPHICS_H
#define ROBOT_FRAMEWORK_GRAPHICS_H

#include "RobotFramework.h"

//=============================================================================
//                     F u n c t i o n  P r o t o t y p e s
//=============================================================================

void init_graphics(gstScene *pScene = NULL);
void term_graphics(void);
void update_graphics(void);
void reshape(GLint width, GLint height);
void set_workspace_color(void);
void set_robot_color(void);

#endif // ROBOT_FRAMEWORK_GRAPHICS_H
