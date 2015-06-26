//=============================================================================
//   Filename : RobotFramework_haptics.cpp
// Written by : Brandon Itkowitz
//    Project : Example GhostGL Application w/ Transforms & Color
//     Module : Platform Independent Haptics
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include "gstAfx.h"
#include "RobotFramework_haptics.h"
#include "RobotFramework_graphics.h"

static gstBoolean bSuspended = FALSE;  // True if servo loop has been suspended with enable_servo_loop()

static gstScene *myScene = NULL;       // a pointer to the GHOST scene
static gstPHANToM *myPhantom = NULL;

//=============================================================================
// Function : init_haptics
// -----------------------
// Initializes the scene graph and creates a PHANToM instance. This also
// demonstrates how to add a workspace boundary to the PHANToM
//=============================================================================
gstScene *init_haptics(int bResetPHANToM)
{
    // create the scene instance and set the root node
    myScene = new gstScene();
    gstSeparator *rootSep = new gstSeparator();
    myScene->setRoot(rootSep);
    
    // create a PHANToM instance and check to make sure it is valid
    myPhantom = new gstPHANToM(PHANTOM_NAME, bResetPHANToM);
    if (!myPhantom || !myPhantom->getValidConstruction()) {
        cerr << "Unable to initialize PHANToM device." << endl;
        exit(-1);
    }
    // assign a name for the PHANToM instance so we can retrieve it later
    myPhantom->setName(gstNodeName(PHANTOM_NAME));
    
    // create a parent for the PHANToM and add both to the scene
    gstSeparator *phantomParent = new gstSeparator();
    rootSep->addChild(phantomParent);
    phantomParent->addChild(myPhantom);
    
    // attach a workspace boundary (added as a sibling to the PHANToM)
    // this routine requires that the PHANToM to have a distinct parent
    gstBoundaryCube *myBoundary = myPhantom->attachMaximalBoundary();
    myBoundary->setName(gstNodeName(WORKSPACE_NAME));
    
    // create the robot geometry for the scene
    gstSeparator *robotSep = create_robot();
    rootSep->addChild(robotSep);
    robotSep->setName(gstNodeName(ROBOT_NAME));
    
    return myScene;
}

//=============================================================================
// Function : init_robot_transform
// -------------------------------
// Sets the initial size and position based on the resized workspace boundary
//=============================================================================
void init_robot_transform(void)
{
    gstSeparator *robotSep;
    gstBoundaryCube *workspace;
    
    robotSep = (gstSeparator *) myScene->getRoot()->getByName(ROBOT_NAME);
    workspace = (gstBoundaryCube *) myScene->getRoot()->getByName(WORKSPACE_NAME);
    robotSep->setTranslate(workspace->getPosition_WC());
    robotSep->setScale(workspace->getHeight() / ROBOT_TOTAL_HEIGHT * .75);
}

//=============================================================================
// Function : create_robot
// -----------------------
// Create the haptic geometry for a robot and return its separator. This routine
// demostrates how to use the gstTransform convenience methods for transforming
// geometry
//=============================================================================
gstSeparator *create_robot(void)
{
    gstSeparator *robotSep = new gstSeparator();
    
    // first generate the robot torso which will be at the root of the robot
    gstSeparator *torsoSep = new gstSeparator();
    gstCylinder *robotTorso = new gstCylinder();
    robotTorso->setHeight(ROBOT_TORSO_HEIGHT);
    robotTorso->setRadius(ROBOT_TORSO_RADIUS);
    torsoSep->addChild(robotTorso);
    robotSep->addChild(torsoSep);
    
    // create and add the robot's head
    gstSphere *robotHead = new gstSphere();
    robotHead->setRadius(ROBOT_TORSO_RADIUS);
    robotHead->setTranslate(0.0,
        robotTorso->getHeight() / 2.0 + robotHead->getRadius(),
        0.0);
    torsoSep->addChild(robotHead);
    
    // create the right arm
    gstSeparator *rightArmSep = create_robot_arm();
    rightArmSep->setName(gstNodeName("rightarm"));
    torsoSep->addChild(rightArmSep);
    
    // transform the arm to be on the right side of the body
    rightArmSep->setRotate(gstVector(0.0, 0.0, 1.0), -M_PI * .90);
    rightArmSep->setTranslate(robotTorso->getRadius(),
        robotTorso->getHeight() / 2.0 - ROBOT_SHOULDER_RADIUS, 0.0);
    
    // create the left arm
    gstSeparator *leftArmSep = create_robot_arm();
    leftArmSep->setName(gstNodeName("leftarm"));
    torsoSep->addChild(leftArmSep);
    
    // transform the arm to be on the left side of the body
    leftArmSep->setRotate(gstVector(0.0, 1.0, 0.0), M_PI);
    leftArmSep->rotate(gstVector(0.0, 0.0, 1.0), -M_PI * .90);
    leftArmSep->setTranslate(-robotTorso->getRadius(),
			     robotTorso->getHeight() / 2.0 - ROBOT_SHOULDER_RADIUS, 0.0);
    
    // add the legs
    gstSeparator *rightLeg = create_robot_leg();
    rightLeg->setRotate(gstVector(0.0, 1.0, 0.0), M_PI / 4);
    rightLeg->setTranslate(robotTorso->getRadius() - ROBOT_LEG_RADIUS,
        -robotTorso->getHeight() / 2.0, 0.0);
    torsoSep->addChild(rightLeg);
    
    gstSeparator *leftLeg = create_robot_leg();
    leftLeg->setRotate(gstVector(0.0, 1.0, 0.0), -M_PI / 4);
    leftLeg->setTranslate(-robotTorso->getRadius() + ROBOT_LEG_RADIUS,
        -robotTorso->getHeight() / 2.0, 0.0);
    torsoSep->addChild(leftLeg);
    
    return robotSep;
}

//==============================================================================
// Function : create_robot_arm
// ---------------------------
// Create a robot arm out of Ghost primitives and point it up the y axis.
//==============================================================================
gstSeparator *create_robot_arm(void)
{
    gstSeparator *shoulderSep = new gstSeparator();
    
    // create the shoulder joint
    gstSphere *shoulder = new gstSphere();
    shoulder->setRadius(ROBOT_SHOULDER_RADIUS);
    shoulderSep->addChild(shoulder);
    
    // create the arm
    gstCylinder *robotArm = new gstCylinder();
    robotArm->setHeight(ROBOT_ARM_LENGTH);
    robotArm->setRadius(ROBOT_ARM_RADIUS);
    robotArm->setTranslate(0.0,
        robotArm->getHeight() / 2.0 +
        shoulder->getRadius(),
        0.0);
    shoulderSep->addChild(robotArm);
    
    // create the hand
    gstSphere *robotHand = new gstSphere();
    robotHand->setRadius(ROBOT_HAND_RADIUS);
    robotHand->setTranslate(0.0,
        robotHand->getRadius() +
        robotArm->getHeight() +
        shoulder->getRadius(),
        0.0);
    shoulderSep->addChild(robotHand);
    
    return shoulderSep;
}

//==============================================================================
// Function : create_robot_leg
// ---------------------------
// Creates a robot leg and returns its separator
//==============================================================================
gstSeparator *create_robot_leg(void)
{
    gstSeparator *legSep = new gstSeparator();    
    
    // create the leg and dangle it from the origin
    gstCylinder *robotLeg = new gstCylinder();
    robotLeg->setHeight(ROBOT_LEG_LENGTH);
    robotLeg->setRadius(ROBOT_LEG_RADIUS);
    robotLeg->setTranslate(0.0,
        -robotLeg->getHeight() / 2.0,
        0.0);
    legSep->addChild(robotLeg);
    
    // create a foot and place it at the bottom of the leg
    gstCube *robotFoot = new gstCube();
    robotFoot->setLength(ROBOT_FOOT_LENGTH);
    robotFoot->setWidth(ROBOT_FOOT_WIDTH);
    robotFoot->setHeight(ROBOT_FOOT_HEIGHT);
    robotFoot->setTranslate(0.0,
        -(robotLeg->getHeight() + robotFoot->getHeight() / 2.0),
        robotFoot->getLength() / 2.0 - robotLeg->getRadius());
    legSep->addChild(robotFoot);
    
    return legSep;
}

//==============================================================================
// Function : term_haptics
// -----------------------
// Description : Terminate haptics and delete the scene graph
// Return type : void 
//==============================================================================
void term_haptics(void)
{
    if (myScene)
        delete myScene;
}

//==============================================================================
// Function : enable_servo_loop
// ----------------------------
// This function is called to start and stop the servo loop within the application
//==============================================================================
void enable_servo_loop(int bEnable)
{
    if (myScene) {
        if (bEnable) {
            if (!myScene->startServoLoop()) {
                printf("Unable to start servo loop, exiting\n");
                exit(-1);
            }
        } else
            myScene->stopServoLoop();
    }
    
    // suspend state is active when the servo loop is disabled
    bSuspended = !bEnable;
}

//==============================================================================
// Function : scene_done
// ---------------------
// Reports whether the servo loop is done.
//==============================================================================
int scene_done(void)
{
    // getDoneServoLoop is TRUE if the servo loop is not running
    return (!bSuspended && myScene && myScene->getDoneServoLoop());
}

//==============================================================================
// Function : query_phantom_pos
// ----------------------------
// Returns the current PHANToM position
//==============================================================================
int query_phantom_pos(double *px, double *py, double *pz)
{
    if (!myPhantom)
        return FALSE;
    
    gstPoint phantomPos = myPhantom->getPosition_WC();
    
    *px = phantomPos[0];
    *py = phantomPos[1];
    *pz = phantomPos[2];
    
    return TRUE;
}


