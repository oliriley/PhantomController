//*****************************************************************************
//      Filename : PhantomContext_haptics.cpp
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include "gstAfx.h"
#include "PhantomContext_graphics.h"
#include "PhantomContext_haptics.h"

#include "HapticFrustum.h"
#include "PhantomContextView.h"

//*****************************************************************************
//                     S t a t i c   V a r i a b l e s 
//*****************************************************************************
static gstBoolean bHapticsOn = TRUE;
static gstBoolean bSuspended = FALSE;   // True if servo loop has been suspended

static gstPHANToM *myPhantom = NULL;
static gstScene   *myScene = NULL;

//*****************************************************************************
// Function : init_haptics
// -----------------------
// Description : Creates scene graph.  The scene graph is composed of the scene
//               node, then a root separator, then the actual scene graph
//               containing all the haptics objects
// Return type : void 
//*****************************************************************************
gstScene *init_haptics(int bResetPHANToM)
{
    gstSeparator *rootSep, *phantomSep, *geomSep;
    
    // create the scene and append the root
    myScene = new gstScene();
    rootSep = new gstSeparator();

    phantomSep = new gstSeparator();
    phantomSep->setName(gstNodeName(PHANTOM_PARENT_NAME));

    geomSep = new gstSeparator();
    geomSep->setName(gstNodeName(GEOM_SEP_NAME));

    myScene->setRoot(rootSep);
    rootSep->addChild(phantomSep);
    rootSep->addChild(geomSep);

    // Create the phantom object.  When this line is processed, the phantom position is zeroed
    myPhantom = new gstPHANToM(PHANTOM_NAME, bResetPHANToM);
    if (!myPhantom->getValidConstruction()) {
        cerr << "Failure to create a valid construction." << endl;
        exit(-1);
    }
    myPhantom->setName(gstNodeName(PHANTOM_NAME));
    phantomSep->addChild(myPhantom);

    // Get a maximal workspace boundary from the Phantom and allow it to
    // attach it as its sibling
    myPhantom->attachMaximalBoundary();

    // Create some geometry for the scene
    create_scene_geom(geomSep);
    
    return myScene;
}

//*****************************************************************************
// Function : term_haptics
// -----------------------
// Description : Deletes scene graph
// Return type : void 
//*****************************************************************************
void term_haptics()
{
    enable_servo_loop(FALSE);

    if (myScene)
        delete myScene;
}

//*****************************************************************************
// Function : enable_servo_loop
// ----------------------------
// Description : Enable or disable the servo loop. This function is called to
//               start and stop the servo loop within the application
// Return type : void 
// Argument #1 : int enable
//*****************************************************************************
void enable_servo_loop(int bEnable)
{
    // scene_done() will report being done when the servo loop is finished
    // through means other than suspending it here
    if (bEnable && myScene)
        myScene->startServoLoop();
    else
        myScene->stopServoLoop();

    bSuspended = !bEnable;
}

//*****************************************************************************
// Function : scene_done
// ---------------------
// Description : Report whether the servo loop is done. This function is called
//               to tell whether the servo loop is still running
// Return type : int 
// Argument #1 : void
//*****************************************************************************
int scene_done(void)
{
    // getDoneServoLoop is TRUE if the servo loop is not running
    return (!bSuspended && myScene && myScene->getDoneServoLoop());
}

//*****************************************************************************
// Function : query_phantom_pos
// ----------------------------
// Description : Returns the current PHANToM position in world coordinates
// Return type : int 
// Argument #1 : double *px
// Argument #2 : double *py
// Argument #3 : double *pz
//*****************************************************************************
int query_phantom_pos(double *px, double *py, double *pz)
{
    gstPoint phantomPos = myPhantom->getPosition_WC();

    *px = phantomPos[0];
    *py = phantomPos[1];
    *pz = phantomPos[2];

    return TRUE;
}

//*****************************************************************************
// Function : frustumEventCallback
// -------------------------------
// Description : Receives the event callback when a HapticFrustum event occurs
// Return type : void 
// Argument #1 : gstTransform *node
// Argument #2 : void *cbData
// Argument #3 : void *userData
//*****************************************************************************
void frustum_event_callback(gstTransform *node, void *cbData, void *userData)
{
    gstEvent *event = (gstEvent *) cbData;
    
    // if a pop through event occurs, leave this haptic context
    if (event->id == HapticFrustum::POPPED_THROUGH) {

#ifdef WIN32
        CWinApp *appPtr = (CWinApp *) AfxGetApp();
        CFrameWnd *winPtr = (CFrameWnd *) appPtr->GetMainWnd();
        CHapticView *hapticView = (CHapticView *) winPtr->GetActiveView();        
#else // SGI or SUN
        CHapticView *hapticView = (CHapticView *) ApplicationShell->GetActiveView();        
#endif

        // force the PHANToM to change into mouse mode
        hapticView->OnPhantomLeave();
    }
}       

//*****************************************************************************
// Function : create_scene_geom
// ----------------------------
// Description : Generates the geometry for the scene
// Return type : void
//*****************************************************************************
void create_scene_geom(gstSeparator *geomSep)
{
    gstCone *myCone;
    gstSphere *mySphere;

    if (!geomSep)
        return;

    // create the center sphere
    mySphere = new gstSphere();
    mySphere->setRadius(SPHERE_RADIUS);
    mySphere->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(mySphere);

    // create 6 cones pointing along the coordinate axes

    // cone pointing up
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setTranslate(0.0, CONE_HEIGHT / 2.0, 0.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);

    // cone pointing down
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setRotate(gstVector(1.0, 0.0, 0.0), M_PI);
    myCone->setTranslate(0.0, -CONE_HEIGHT / 2.0, 0.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);

    // cone pointing left
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setRotate(gstVector(0.0, 0.0, 1.0), M_PI / 2.0);
    myCone->setTranslate(-CONE_HEIGHT / 2.0, 0.0, 0.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);

    // cone pointing right
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setRotate(gstVector(0.0, 0.0, 1.0), -M_PI / 2.0);
    myCone->setTranslate(CONE_HEIGHT / 2.0, 0.0, 0.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);    

    // cone pointing forward
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setRotate(gstVector(1.0, 0.0, 0.0), M_PI / 2.0);
    myCone->setTranslate(0.0, 0.0, CONE_HEIGHT / 2.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);

    // cone pointing back
    myCone = new gstCone();
    myCone->setRadius(CONE_RADIUS);
    myCone->setHeight(CONE_HEIGHT);
    myCone->setRotate(gstVector(1.0, 0.0, 0.0), -M_PI / 2.0);
    myCone->setTranslate(0.0, 0.0, -CONE_HEIGHT / 2.0);
    myCone->setSurfaceFdynamic(SURFACE_FRICTION);
    geomSep->addChild(myCone);    

    // rotate the geometry a bit to make the initial orientation more interesting
    geomSep->setRotate(gstVector(0.0, 1.0, 0.0), M_PI / 4.0);
    geomSep->rotate(gstVector(1.0, 0.0, 0.0), -M_PI / 4.0);
}

//*****************************************************************************
// Function : toggle_haptics
// -------------------------
// Description : Turns on or off the haptics by making all nodes under the root
//               untouchable by the phantom. This is the safest way to disable
//               touch without turning off the servo loop.
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void toggle_haptics(void)
{
    if (myScene) {
        if (bHapticsOn)
            myScene->getRoot()->setTouchableByPHANToM(FALSE);
        else
            myScene->getRoot()->setTouchableByPHANToM(TRUE);
    }
    
    bHapticsOn = !bHapticsOn;
}

//*****************************************************************************
// Function : get_haptics
// ----------------------
// Description : Returns the current state of the haptics
// Return type : int 
// Argument #1 : void
//*****************************************************************************
int get_haptics(void)
{
    return bHapticsOn;
}




