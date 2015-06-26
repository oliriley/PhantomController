//*****************************************************************************
//      Filename : main.cpp
// ----------------------------------------------------------------------------
// This program demonstrates how to extend the gstEffect class
// by creating a viscosity effect.
//
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include <gstScene.h>
#include <gstPHANToM.h>
#include <ViscEffect.h>
#include <iostream.h>

int main()
{
    // Create a GHOST scene object.
    gstScene *scene = new gstScene;

    // create the root separator and set it as the root of the scene graph
    gstSeparator *root = new gstSeparator();
    scene->setRoot(root);

    // prompt the user to place the PHANToM in the reset position
    cout << "Place the PHANToM in its reset position and press <ENTER>." << endl;
	cin.get();

    // create a PHANToM instance and check to make sure it is valid
    gstPHANToM *myPhantom = new gstPHANToM("Default PHANToM");
    if (!myPhantom || !myPhantom->getValidConstruction()) {
        cerr << "Unable to initialize PHANTOM device." << endl;
        exit(-1);
    }
    
    // add the PHANToM object to the scene
    root->addChild(myPhantom);

    // Create the effect and add to PHANToM
    ViscEffect *effect = new ViscEffect;
    effect->setK(0.003);
    myPhantom->setEffect(effect);
    myPhantom->startEffect();

    // start the haptic simulation
    scene->startServoLoop();

    cout << "Press <ENTER> to quit" << endl;
    cin.get();

    scene->stopServoLoop();

    return 0;
}
