//=============================================================================
//   Filename : Template.cpp
// Written by :
//    Project : Template Ghost Application 
//     Module : Platform Independent Application Entry
// ----------------------------------------------------------------------------
// This is a template GHOST application
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include "gstAfx.h"
#include "Template.h"
#include "Template_graphics.h"
#include "Template_haptics.h"

static gstScene *myScene = NULL;

void start_program(int bResetPHANToM)
{
    myScene = new gstScene();              // Create a shared static instance of the scene 
    init_haptics(myScene, bResetPHANToM);  // Initialize scene graph
    init_graphics(myScene);                // Now force init_graphics to load the scene graph
}

void end_program(void)
{
    term_haptics();                        // Perform whatever cleanup needs to be done
    
    if (myScene)
        delete myScene;
}
