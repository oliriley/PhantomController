//*****************************************************************************
//      Filename : PhantomContext_graphics.cpp
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include "gstAfx.h"
#include "PhantomContext_graphics.h"
#include "PhantomContext_haptics.h"

#include <ghostGLManager.h>
#include <ghostGLSyncCamera.h>
#include <ghostGLPinchXForm.h>

//*****************************************************************************
//                      S t a t i c   V a r i a b l e s 
//*****************************************************************************
static ghostGLManager *glManager = NULL;       // the main graphics object

static gstBoundaryCube *workspaceBounds = NULL;
static HapticFrustum *hapticFrustum = NULL;
static gstBoolean bAxesEnabled = FALSE;

static gstScene *myScene = NULL;
static gstPHANToM *myPhantom = NULL;

//*****************************************************************************
// Function : init_graphics
// ------------------------
// Description : Performs the initialization on the ghostGLManager object. 
//               This routine is called once by the framework API and then again
//               by the start_program routine, once the haptic scene has been
//               generated
// Return type : void 
// Argument #1 : gstBoolean initScene
//*****************************************************************************
void init_graphics(gstScene *pScene)
{
    ghostGLSyncCamera *myCamera;
    
    if (!glManager) {
        myCamera = new ghostGLSyncCamera();
        glManager = new ghostGLManager(myCamera);
    }
    
    // only do this if the initScene flag was explicitly set
    if (pScene) {
        myScene = pScene;
        
        glManager->loadScene(pScene);
        glManager->setAxesOn(bAxesEnabled);
        
        // Specify the PHANToM cursor to be a sphere instead of the default cone
        myPhantom = (gstPHANToM *) myScene->getRoot()->getByName(PHANTOM_NAME);
        setup_cursor(myPhantom);
        
        // Get the workspace boundary from the Phantom and then remove the store
        // boundary we need the boundary to remain a sibling of the phantom for
        // ghostGL to keep it in sync'd
        workspaceBounds = (gstBoundaryCube *) myPhantom->getBoundaryObj();
        myPhantom->setBoundaryObj(NULL);
        
        // get the current sync camera and set it up for workspace / camera syncing
        myCamera = (ghostGLSyncCamera *) glManager->getCamera();
        myCamera->setWorkspaceBoundary(workspaceBounds);
        myCamera->setSyncMode(ghostGLSyncCamera::SYNC_CAMERA_TO_WORKSPACE);
        myCamera->setSyncToWindow(TRUE);         // sync to window is true by default
        myCamera->setViewEntireWorkspace(FALSE); // view the back plane only
        myCamera->setClipToWorkspace(TRUE);      // turn on graphic z-clipping for the workspace
        myCamera->update();                      // initialize the camera transform
        
        // setup the pinch transform
        myCamera->setSyncMode(ghostGLSyncCamera::SYNC_WORKSPACE_TO_CAMERA);
        ghostGLPinchXForm *pinchXFormObj = new ghostGLPinchXForm();
        glManager->addActionObject(pinchXFormObj);    
        
        // now update the workspace based on the camera
        myCamera->update();
        
        init_frustum(); // init the frustum based on the current camera and workspace
        
        // recenter the scene geometry in the workspace
        gstNode *geomSep = ((gstSeparator *) pScene->getRoot())->getByName(GEOM_SEP_NAME);
        ((gstSeparator *) geomSep)->setTranslate(workspaceBounds->getPosition());   
    }
}

//*****************************************************************************
// Function : term_graphics
// ------------------------
// Description : Deletes the instance of ghostGLManager
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void term_graphics(void)
{
    if (glManager)
        delete glManager;
}

//*****************************************************************************
// Function : reshape
// ------------------
// Description : Called by window manager when window is resized.
// Return type : void 
// Argument #1 : GLsizei w
// Argument #2 : GLsizei h
//*****************************************************************************
void reshape(int width, int height)
{
    glManager->reshape(width, height);  // reshape the viewport
    
    if (hapticFrustum)
        update_frustum();           // resize the haptic frustum
}

//*****************************************************************************
// Function : update_graphics
// --------------------------
// Description : Tells the ghostGLManager object to perform a redraw of the
//               scene
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void update_graphics(void)
{    
    glManager->redraw();
}

//*****************************************************************************
// Function : init_frustum_dimensions
// ----------------------------------
// Description : Determines the dimensions to set the haptic frustum, given a
//               gstBoundaryCube object and the viewing parameters
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void init_frustum(void)
{
    // create a haptic frustum and set its dimensions so that its bounding planes
    // intersect the back plane edges of the workspace
    hapticFrustum = new HapticFrustum();
    hapticFrustum->setTouchableFrom(gstTriPolyMeshHaptic::RV_FRONT);
    hapticFrustum->setThreshold(FORCE_THRESHOLD);
    hapticFrustum->setEventCallback(frustum_event_callback, NULL);
    
    // add the haptic frustum as a sibling of the phantom
    gstSeparator *phantomNodeBase = (gstSeparator *) myPhantom->getParent();
    phantomNodeBase->addChild(hapticFrustum);
    
    // disable drawing of the haptic frustum and the workspace
    glManager->setVisible(hapticFrustum, FALSE);
    glManager->setVisible(workspaceBounds, FALSE);
    
    // resize the haptic frustum to match the current boundary
    update_frustum();
}

//*****************************************************************************
// Function : update_frustum
// -------------------------
// Description : Resizes the haptic frustum based on the new dimensions of the
//               workspace boundary
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void update_frustum(void)
{
    gstPoint viewerPos;
    float nearPlane, farPlane, aspectRatio, fieldOfView;
    float nearOffset, farOffset, border;
    ghostGLSyncCamera *myCamera;
    
    // get the current sync camera and set it up for workspace / camera syncing
    myCamera = (ghostGLSyncCamera *) glManager->getCamera();
    
    fieldOfView = myCamera->getViewAngle();
    aspectRatio = (float) myCamera->getWindowWidth() / (float) myCamera->getWindowHeight();
    
    // need to calculate the nearPlane and farPlane, since the camera won't always coincide
    viewerPos = myCamera->getWorkspaceOffset();
    nearPlane = viewerPos.z() - workspaceBounds->getLength() / 2.0;
    farPlane = viewerPos.z() + workspaceBounds->getLength() / 2.0;
    
    // the near and far plane offsets allow for a difference in setting between the 
    // graphics clipping planes and the haptic bounding planes
    nearOffset = 0;
    farOffset = 0;
    border = 0;
    
    hapticFrustum->setPerspectiveView(viewerPos, fieldOfView, aspectRatio,
        nearPlane, farPlane, nearOffset, farOffset, border);
    
    // adjust the ghostGL graphic clipping planes so that the cursor doesn't get clipped
    if (myCamera->getClipToWorkspace()) {
        nearPlane = myCamera->getNearClippingPlane();
        farPlane = myCamera->getFarClippingPlane();
        myCamera->setClippingPlanes(nearPlane - NEAR_PLANE_OFFSET,
            farPlane + FAR_PLANE_OFFSET);
    }
}

//*****************************************************************************
// Function : get_frustum_height
// -----------------------------
// Description : Determines the frustum height for a particular Phantom Z depth
// Return type : [float] frustum height
// Argument #1 : [float] z location of the PHANToM
//*****************************************************************************
double get_frustum_height()
{
    ghostGLSyncCamera *myCamera;
    float zOffset, height, length;
    
    // get the current sync camera and set it up for workspace / camera syncing
    myCamera = (ghostGLSyncCamera *) glManager->getCamera();
    
    // get the z offset of the camera
    zOffset = myCamera->getWorkspaceOffset().z();

    // get some dimensions about the workspace
    height = workspaceBounds->getHeight();
    length = workspaceBounds->getLength();

    // local phantom position
    gstPoint phantomPos = myPhantom->getPosition();
    
    return (height / (zOffset + length / 2.0) * (zOffset - phantomPos.z()));
}

//*****************************************************************************
// Function : setup_cursor
// -----------------------
// Description : Force ghostGL to draw the cursor as a sphere instead of a cone
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void setup_cursor(gstTransform *cursorNode)
{
    static const int CURSOR_RADIUS = 3.0;
    
    gfxDisplaySettings *displaySettings = glManager->getDisplaySettings(cursorNode);
    GLfloat cursor_diffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
    
    glDeleteLists(displaySettings->geomDisplayList, 1);
    
    glNewList(displaySettings->geomDisplayList, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cursor_diffuse);
    ghostGLDraw::drawSphere(CURSOR_RADIUS);
    glPopAttrib();
    glEndList();
}

//*****************************************************************************
// Function : toggle_axes
// ----------------------
// Description : Toggle on / off the display of the coordinate axes
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void toggle_axes(void)
{
    bAxesEnabled = !bAxesEnabled;
    glManager->setAxesOn(bAxesEnabled);
}

//*****************************************************************************
// Function : toggle_graphics
// --------------------------
// Description : Toggle on / off the display of graphics
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void toggle_graphics(void)
{
    glManager->setGraphicsOn(!glManager->isGraphicsOn());
}

//*****************************************************************************
// Function : get_graphics
// -----------------------
// Description : Return current state of graphics. By default, graphics will be
//               on if glManager hasn't been instantiated
// Return type : int 
// Argument #1 : void
//*****************************************************************************
int get_graphics(void)
{
    if (glManager)
        return glManager->isGraphicsOn();
    else
        return TRUE;
}
