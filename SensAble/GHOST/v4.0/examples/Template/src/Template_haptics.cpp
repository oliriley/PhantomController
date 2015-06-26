//=============================================================================
//   Filename : Template_haptics.cpp
// Written by : 
//    Project : Template Ghost Application
//     Module : Platform Independent Haptics
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
#include "Template_graphics.h"
#include "Template_haptics.h"

static gstBoolean bSuspended = FALSE;  // True if servo loop has been suspended with enable_servo_loop()
static gstPHANToM *myPHANToM = NULL;
static gstScene *myScene = NULL;

void init_haptics(gstScene *pScene, int bResetPHANToM)
{
    gstSeparator *rootSep;

    assert(pScene);
    myScene = pScene;

    rootSep = new gstSeparator();
    myScene->setRoot(rootSep);

    // create a PHANToM instance and check to make sure it is valid
    myPHANToM = new gstPHANToM(PHANTOM_NAME, bResetPHANToM);
    if (!myPHANToM || !myPHANToM->getValidConstruction()) {
        cerr << "Unable to initialize PHANToM device." << endl;
        exit(-1);
    }

    // add the PHANToM object to the scene
    rootSep->addChild(myPHANToM);

    // Create a sphere node and add it to the scene
    gstSphere *mySphere = new gstSphere();
    mySphere->setRadius(50);
    rootSep->addChild(mySphere);
}    

void term_haptics(void)
{
}

void enable_servo_loop(int bEnable)
{
    if (bEnable)
        myScene->startServoLoop();
    else
        myScene->stopServoLoop();

    bSuspended = !bEnable;
}

int scene_done(void)
{
    return (!bSuspended && myScene->getDoneServoLoop());
}

int query_phantom_pos(double *px, double *py, double *pz)
{
    gstPoint position = myPHANToM->getPosition_WC();

    *px = position.x();
    *py = position.y();
    *pz = position.z();

    return TRUE;
}
