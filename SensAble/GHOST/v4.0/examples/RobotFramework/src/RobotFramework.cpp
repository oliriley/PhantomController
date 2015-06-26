//=============================================================================
//     Filename : RobotFramework.cpp
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

#include "gstAfx.h"
#include "RobotFramework.h"
#include "RobotFramework_graphics.h"
#include "RobotFramework_haptics.h"

//=============================================================================
// Function : start_program
// ------------------------
// The bootstrap routine to initialize the haptics and graphics
//=============================================================================

void start_program(int bResetPHANToM)
{
    static int bFirstTime = TRUE;
    gstScene *pScene = NULL;

    // Initialize haptics and start haptics
    if (bFirstTime) {
        pScene = init_haptics(bResetPHANToM);
        init_graphics(pScene);
        init_robot_transform();
        bFirstTime = FALSE;
    }
}

//=============================================================================
// Function : end_program
// ----------------------
// Performs the necessary clean-up at program end
//*****************************************************************************
void end_program(void)
{
    term_haptics(); // Terminate haptics
}
