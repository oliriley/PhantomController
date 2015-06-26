//*****************************************************************************
//      Filename : main.cpp
// ----------------------------------------------------------------------------
// This program renders a sphere with magnetic attraction using a force field.
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include <gstScene.h>
#include <gstPHANToM.h>
#include <gstSphere.h>
#include <iostream.h>
#include <ghostGLUTManager.h>
#include <MagneticSphere.h>

int main(int argc, char *argv[])
{
    // Create a GHOST scene object.
    gstScene *scene = new gstScene;

	// Create root separator of scene graph for robot
	gstSeparator *root = new gstSeparator;
    scene->setRoot(root);

    // prompt the user to place the PHANToM in the reset position
    cout << "Place the PHANToM in its reset position and press <ENTER>." << endl;
    cin.get();

	gstPHANToM *phantom = new gstPHANToM("Default PHANToM");
	if (!phantom || !phantom->getValidConstruction()) {
        cerr << "Unable to initialize PHANTOM device." << endl;
		exit(-1);
	}
	root->addChild(phantom);

    // Add sphere to scene
    gstSphere *sphere = new gstSphere;
    sphere->setRadius(40);
    root->addChild(sphere);

    // Add force field to make sphere magnetic
    MagneticSphere *magSphere = new MagneticSphere(40, 48);
    root->addChild(magSphere);

	// Run haptic simulation
	scene->startServoLoop();
	
    // create an instance of the GLUT OpenGL Manager
    ghostGLUTManager *glutManager = ghostGLUTManager::CreateInstance(argc, argv, 
                                                              "Magnetic Sphere");
   
    // load the scene graph into the ghostGLUTManager instance
    glutManager->loadScene(scene);

    // start the display of graphics
    glutManager->startMainloop();

    return 0;
}
