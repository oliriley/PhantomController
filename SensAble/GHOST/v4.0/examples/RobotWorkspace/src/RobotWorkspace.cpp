//=============================================================================
//     Filename : RobotWorkspace.cpp
//   Written by : Brandon Itkowitz
//     Contains : Example GhostGL Application w/ Transforms & Workspace
// ----------------------------------------------------------------------------
// This program haptically and graphically renders the upper body of a robot.
// The robot is built using gstShape primitives and gstSeparator nodes.
// The graphics are managed by GLUT and GhostGL.
// This example additionally demonstrates the use of a workspace and a
// ghostGLSyncCamera to provide a bounded work environment
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include <gstScene.h>
#include <gstPHANToM.h>
#include <gstCylinder.h>
#include <gstSphere.h>
#include <gstSeparator.h>
#include <gstBoundaryCube.h>
#include <iostream.h>

#include <ghostGLUTManager.h>
#include <ghostGLSyncCamera.h>

inline double degToRad(double degrees)
{
    return (degrees * (M_PI / 180.0));
}

//=============================================================================
// Function : createRobotGeometry
// ------------------------------
// Generates a collection of Ghost primitives that are constructed to form
// a robot. The body separator gets returned.
//=============================================================================
gstSeparator *createRobotGeometry()
{
    // create body for robot
    gstSeparator *body = new gstSeparator;

    // Head
    gstSeparator *head = new gstSeparator;
    head->setTranslate(0.0,70.0,0.0);
    body->addChild(head);

    gstSphere *headSphere = new gstSphere;
    headSphere->setRadius(15.0);
    head->addChild(headSphere);

    // Torso
    gstSeparator *torso = new gstSeparator;
    torso->setTranslate(0.0,25.0,0.0);
    body->addChild(torso);

    gstCylinder *torsoCylinder = new gstCylinder;
    torsoCylinder->setRadius(25.0);
    torsoCylinder->setHeight(50.0);
    torso->addChild(torsoCylinder);

    // Right Shoulder
    gstSeparator *rightShoulder = new gstSeparator;
    rightShoulder->setTranslate(25.0,25.0,0.0);
    torso->addChild(rightShoulder);

    gstCylinder *rightShoulderCylinder = new gstCylinder;
    rightShoulderCylinder->setRotate(gstVector(0.0,0.0,1.0),degToRad(90.0));
    rightShoulderCylinder->setTranslate(15.0,0.0,0.0);
    rightShoulderCylinder->setHeight(30.0);
    rightShoulderCylinder->setRadius(6.0);
    rightShoulder->addChild(rightShoulderCylinder);

    // Right Elbow
    gstSeparator *rightElbow = new gstSeparator;
    rightElbow->setTranslate(30.0,0.0,0.0);
    rightElbow->setRotate(gstVector(0.0,0.0,1.0),degToRad(-45.0));
    rightShoulder->addChild(rightElbow);

    gstCylinder *rightElbowCylinder = new gstCylinder;
    rightElbowCylinder->setRotate(gstVector(0.0,0.0,1.0),degToRad(90.0));
    rightElbowCylinder->setTranslate(15.0,0.0,0.0);
    rightElbowCylinder->setHeight(30.0);
    rightElbowCylinder->setRadius(6.0);
    rightElbow->addChild(rightElbowCylinder);

    // Left Shoulder
    gstSeparator *leftShoulder = new gstSeparator;
    torso->addChild(leftShoulder);
    leftShoulder->setTranslate(-25.0,25.0,0.0);
    
    gstCylinder *leftShoulderCylinder = new gstCylinder;
    leftShoulder->addChild(leftShoulderCylinder);
    leftShoulderCylinder->setRotate(gstVector(0.0,0.0,1.0),degToRad(90.0));
    leftShoulderCylinder->setTranslate(-15.0,0.0,0.0);
    leftShoulderCylinder->setHeight(30.0);
    leftShoulderCylinder->setRadius(6.0);

    // Left Elbow
    gstSeparator *leftElbow = new gstSeparator;
    leftShoulder->addChild(leftElbow);
    leftElbow->setTranslate(-30.0,0.0,0.0);
    leftElbow->setRotate(gstVector(0.0,0.0,1.0),degToRad(45.0));
    
    gstCylinder *leftElbowCylinder = new gstCylinder;
    leftElbow->addChild(leftElbowCylinder);
    leftElbowCylinder->setRotate(gstVector(0.0,0.0,1.0),degToRad(-90.0));
    leftElbowCylinder->setTranslate(-15.0,0.0,0.0);
    leftElbowCylinder->setHeight(30.0);
    leftElbowCylinder->setRadius(6.0);

    return body;
}

int main(int argc, char *argv[])
{
    // create a GHOST scene object
    gstScene myScene;
    
    // create the root separator and set it as the root of the scene graph
    gstSeparator *rootSep = new gstSeparator();
    myScene.setRoot(rootSep);
    
    // prompt the user to place the PHANToM in the reset position
    cout << "Place the PHANToM in its reset position and press <ENTER>." << endl;
    cin.get();

    // create a PHANToM instance and check to make sure it is valid
    gstPHANToM *myPhantom = new gstPHANToM("Default PHANToM");
    if (!myPhantom || !myPhantom->getValidConstruction()) {
        cerr << "Unable to initialize PHANToM device." << endl;
        exit(-1);
    }
    
    // create a parent for the PHANToM and add both to the scene
    gstSeparator *phantomParent = new gstSeparator();
    rootSep->addChild(phantomParent);
    phantomParent->addChild(myPhantom);

    // attach a workspace boundary (added as a sibling to the PHANToM)
    // this routine requires that the PHANToM to have a distinct parent
    gstBoundaryCube *myBoundary = myPhantom->attachMaximalBoundary();

    // add robot geometry to scene and scale it to fit the workspace
    gstSeparator *robotSep = createRobotGeometry();
    robotSep->scale(2.0 * myBoundary->getHeight() / robotSep->getBoundingRadiusOfChildren());
    rootSep->addChild(robotSep);

    // start the haptic simulation
    myScene.startServoLoop();

    // create an instance of the GLUT OpenGL Manager
    ghostGLUTManager *glutManager = ghostGLUTManager::CreateInstance(argc, argv, "Robot Workspace");
   
    // use a sync camera which by default syncs the camera to the workspace
    // as well as adjusts the workspace to fit in the window
    ghostGLSyncCamera *myCamera = new ghostGLSyncCamera();
    glutManager->setCamera(myCamera);
    myCamera->setWorkspaceBoundary(myBoundary);

    // load the scene graph into the ghostGLUTManager instance
    glutManager->loadScene(&myScene);

    // start the display of graphics
    glutManager->startMainloop();

    // stop the haptic simulation
    myScene.stopServoLoop();

    return 0;
}
