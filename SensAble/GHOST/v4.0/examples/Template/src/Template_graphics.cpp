//=============================================================================
//   Filename : Template_graphics.cpp
// Written by : 
//    Project : Template Ghost Application
//     Module : Platform Independent Graphics
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

#include <ghostGLManager.h>

static ghostGLManager *myGLManager = NULL;

void init_graphics(gstScene *pScene)
{
    if (!myGLManager)
        myGLManager = new ghostGLManager();

    if (myGLManager && pScene)
        myGLManager->loadScene(pScene);
}

void term_graphics (void)
{
    if (myGLManager)
    {
        delete myGLManager;
        myGLManager = NULL;
    }
}

void reshape(GLint width, GLint height)
{
    if (myGLManager)
        myGLManager->reshape(width, height);
}

void update_graphics(void)
{
    if (myGLManager)
        myGLManager->redraw();
}
