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

#include "gstAfx.h"
#include "RobotFramework_graphics.h"

#include <ghostGLManager.h>
#include <ghostGLSyncCamera.h>

static ghostGLManager *glManager = NULL; // the main graphics object
static gstScene *myScene = NULL;	 // a pointer to the GHOST scene

//=============================================================================
// Function : init_graphics
// ------------------------
// Performs the initialization on the ghostGLManager object. This routine is
// called once by the framework when the view is created and then again by the
// start_program routine, once the haptic scene has been generated.
//=============================================================================
void init_graphics(gstScene *pScene)
{
    ghostGLSyncCamera *myCamera;
    
    if (!glManager) {
        myCamera = new ghostGLSyncCamera();
        glManager = new ghostGLManager(myCamera);
    }
    
    // only do this if the scene exists
    if (pScene) {
        myScene = pScene; // store the scene for later use
        glManager->loadScene(pScene);
        glManager->setAxesOn(FALSE);
        
        // get the current sync camera and set it up for workspace / camera syncing
        gstPHANToM *myPhantom = (gstPHANToM *) pScene->getRoot()->getByName(PHANTOM_NAME); 
        myCamera = (ghostGLSyncCamera *) glManager->getCamera();
        myCamera->setWorkspaceBoundary((gstBoundaryCube *) myPhantom->getBoundaryObj());
        myCamera->setSyncMode(ghostGLSyncCamera::SYNC_CAMERA_TO_WORKSPACE);
        myCamera->update(); // initialize the camera transform
        set_workspace_color();
        set_robot_color();

        // lock all of the geometry display lists in the scene except the workspace
        glManager->setGeometryLock(pScene->getRoot(), TRUE);
        glManager->setGeometryLock(myPhantom->getBoundaryObj(), FALSE);
    }
}

//=============================================================================
// Function : term_graphics
// ------------------------
// Deletes the instance of ghostGLManager
//=============================================================================
void term_graphics(void)
{
    if (glManager)
        delete glManager;
}

//=============================================================================
// Function : reshape
// ------------------
// Instructs ghostGL to resize the viewport based on the new view dimensions
//=============================================================================
void reshape(GLint width, GLint height)
{
    glManager->reshape(width, height);  // reshape the viewport
}

//=============================================================================
// update_graphics
// ---------------
// Tells the ghostGLManager object to perform a redraw of the scene
//=============================================================================
void update_graphics(void)
{
    glManager->redraw();
}

//=============================================================================
// Function : set_workspace_color
// ------------------------------
// Uses the pre and post display wrappers within ghostGL to change the rendering
// settings for the workspace node. By default, these display lists are not
// defined, but you can use the display lists to add additional geometry, setup
// texturing, material properties, etc.
//=============================================================================
void set_workspace_color()
{
    GLfloat workspace_ambient_diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
    gstTransform *myNode;
    gfxDisplaySettings *displaySettings;
    
    // obtain a pointer to the workspace and then get its display settings
    myNode = (gstTransform *) myScene->getRoot()->getByName(WORKSPACE_NAME);
    displaySettings = glManager->getDisplaySettings(myNode);
    
    // set the color to red
    displaySettings->preDisplayList = glGenLists(1);
    glNewList(displaySettings->preDisplayList, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, workspace_ambient_diffuse);
    glEndList();
    
    // unset the color
    displaySettings->postDisplayList = glGenLists(1);
    glNewList(displaySettings->postDisplayList, GL_COMPILE);
    glPopAttrib();
    glEndList();
}

//=============================================================================
// Function : set_workspace_color
// ------------------------------
// Uses the pre and post display wrappers within ghostGL to change the rendering
// settings for the robot's left and right arms. Any pre / post settings applied
// to a separator will affect all of the child nodes
//=============================================================================
void set_robot_color()
{
    GLfloat leftarm_ambient_diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat rightarm_ambient_diffuse[] = {0.0f, 0.0f, 1.0f, 1.0f};
    gstTransform *myNode;
    gfxDisplaySettings *displaySettings;
    
    // obtain a pointer to the left arm and then get its display settings
    myNode = (gstTransform *) myScene->getRoot()->getByName("leftarm");
    displaySettings = glManager->getDisplaySettings(myNode);
    
    // set the color on the left arm
    displaySettings->preDisplayList = glGenLists(1);
    glNewList(displaySettings->preDisplayList, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, leftarm_ambient_diffuse);
    glEndList();
    
    // unset the left arm color
    displaySettings->postDisplayList = glGenLists(1);
    glNewList(displaySettings->postDisplayList, GL_COMPILE);
    glPopAttrib();
    glEndList();
    
    // obtain a pointer to the right arm and then get its display settings
    myNode = (gstTransform *) myScene->getRoot()->getByName("rightarm");
    displaySettings = glManager->getDisplaySettings(myNode);
    
    // set the color on the right arm
    displaySettings->preDisplayList = glGenLists(1);
    glNewList(displaySettings->preDisplayList, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rightarm_ambient_diffuse);
    glEndList();
    
    // unset the right arm color
    displaySettings->postDisplayList = glGenLists(1);
    glNewList(displaySettings->postDisplayList, GL_COMPILE);
    glPopAttrib();
    glEndList();
}

