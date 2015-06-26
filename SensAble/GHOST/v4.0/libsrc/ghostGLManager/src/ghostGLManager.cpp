//=============================================================================
//   Filename : ghostGLManager.h
// Written by : Brandon Itkowitz (bitkowitz@sensable.com)
//              Philip Winston (pwinston@sensable.com)
//              Chris Tarr (ctarr@sensable.com)
//    Project : GhostGL 1.0
//     Module : GhostGL Manager
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include "ghostGLManager.h"

#include <ghostGLCameraBase.h>
#include <ghostGLActionObject.h>

#include <GL/gl.h>
#include <GL/glu.h>

typedef list<ghostGLActionObject *>::iterator actionObjsIter;

// As a safety precaution, don't lock the geometry by default.
gstBoolean gfxDisplaySettings::bDefaultGeometryLock = FALSE;

//=============================================================================
// Constructor : ghostGLManager
// ----------------------------
// Sets up the camera and initializes OpenGL state for the rendering context
//=============================================================================
ghostGLManager::ghostGLManager(ghostGLCameraBase *camera)
{
    m_pCamera = NULL;
    m_pScene = NULL;
    m_bGraphicsOn = FALSE;  // do not draw the graphics by default
    m_bAxesOn = TRUE;
    
    // If camera parameter is NULL create default camera
    if (camera) {
        setCamera(camera);
        m_bUserCamera = TRUE;
    } else {
        setCamera(new ghostGLCameraBase());
        m_bUserCamera = FALSE;
    }
    
    // initEnv() sets up GL stuff (lighting, etc.)
    initEnv();
}

//=============================================================================
// Deconstructor : ~ghostGLManager
// -------------------------------
// Cleans up the objects belonging to this ghostGLManager instance. 
//
// Note : If you created your own camera, you'll need to delete it yourself
//=============================================================================
ghostGLManager::~ghostGLManager()
{
    // Empty the list of action objects
    while (!m_actionObjs.empty()) {
        delete m_actionObjs.front();
        m_actionObjs.pop_front();
    }
 
    // Delete the camera if it is the default camera
    if (!m_bUserCamera) {
        delete m_pCamera;
    }

    // Removes gfxDisplaySetting data from all of the nodes
    clearAll();
}

//=============================================================================
// Member Function : loadScene
// ---------------------------
// Handles setting up the gstScene instance for GhostGL rendering. Each node in
// the scene needs to be setup with a graphics callback and a
// gfxDisplaySettings structure.
//=============================================================================
void ghostGLManager::loadScene(gstScene *scene)
{
    m_bGraphicsOn = FALSE;

    if (!(m_pScene = scene)) {        
        return;
    }
        
    gstTransform *root = m_pScene->getRoot();
    assert(root);
    
    if (!root || !root->isOfType(gstSeparator::getClassTypeId())) {
        // we expect the root to be a separator
        return;
    }

    m_bGraphicsOn = TRUE;

    // start at the top of the model view matrix stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // calls setupNode recursively for all nodes in the scene
    setupSeparator((gstSeparator *)root);

    // force an initial update on the camera
    getCamera()->update();
}

//=============================================================================
// Member Function : redraw
// ------------------------
// The main entry point for having ghostGL draw the scene. It handles calling
// all of the action objects, applying the current camera transform and
// the uses drawScene() to render all of the nodes.
//=============================================================================
void ghostGLManager::redraw()
{
    for (actionObjsIter iter = m_actionObjs.begin(); iter != m_actionObjs.end(); iter++)
        (*iter)->preDisplay();
    
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // draw the camera transform as the first scene transform
    getCamera()->applyTransform();
    
    // draw the scene graph
    drawScene();
    
    for (iter = m_actionObjs.begin(); iter != m_actionObjs.end(); iter++)
        (*iter)->postDisplay();
}

//=============================================================================
// Member Function : reshape
// -------------------------
// Reshape should be called with the current view dimensions everytime the
// rendering context is resized
//=============================================================================
void ghostGLManager::reshape(int width, int height)
{
    getCamera()->reshape(width, height);
}

//=============================================================================
// Member Function : setCamera
// ---------------------------
// Specify your own camera for viewing the scene
//=============================================================================
void ghostGLManager::setCamera(ghostGLCameraBase *camera)
{
    if (!camera)
        return;
    
    if (m_pCamera && !m_bUserCamera)
        delete m_pCamera;
    
    m_pCamera = camera;
    camera->setGLManager(this);
}

//=============================================================================
// Member Function : addActionObject
// ---------------------------------
// Adds an action object to be pinged every redraw. This allows you to have
// dynamic operations occuring at the graphics rate.
//=============================================================================
void ghostGLManager::addActionObject(ghostGLActionObject *obj)
{
    if (!obj)
        return;
    
    m_actionObjs.push_front(obj);
    obj->setGLManager(this);
}

//=============================================================================
// Member Function : removeActionObject
// ------------------------------------
// Removes the specified action object instance from the list of action objects
//=============================================================================
void ghostGLManager::removeActionObject(ghostGLActionObject *obj)
{
    if (!obj)
        return;
    
    obj->remove();
    obj->setGLManager(NULL);
    m_actionObjs.remove(obj);    
}

//=============================================================================
// Member Function : setTouchableByPHANToM
// ---------------------------------------
// Allows you to easily set the touchable property on the entire scene
//=============================================================================
void ghostGLManager::setTouchableByPHANToM(gstBoolean bTouchable)
{
    assert(m_pScene);
    gstTransform *root = m_pScene->getRoot();
    assert(root);
    root->setTouchableByPHANToM(bTouchable);
}    

//=============================================================================
// Member Function : setVisible
// ----------------------------
// Allows you to recursively specify visiblity for one or several nodes. If
// a separator node is passed in, all of its descendants will be affected.
// When a node is made invisible, all of its display list entries are deleted.
// When the node is made visible again, it will automatically be setup again
// for rendering. 
//=============================================================================
void ghostGLManager::setVisible(gstTransform *node, gstBoolean bVisible)
{
    if (bVisible) {
        setupNode(node, NULL);
    } else {        
        gfxDisplaySettings *displayData = getDisplaySettings(node);
        glDeleteLists(displayData->preDisplayList, 1);
        glDeleteLists(displayData->xformDisplayList, 1);
        glDeleteLists(displayData->geomDisplayList, 1);
        glDeleteLists(displayData->postDisplayList, 1);
        displayData->preDisplayList = 0;
        displayData->xformDisplayList = 0;
        displayData->geomDisplayList = 0;
        displayData->postDisplayList = 0;
        
        // unset the callback, leave our gfxDisplaySetting intact
        node->setGraphicsCallback(NULL, displayData);
    }
        
    // if the node is a separator, propogate the setting to all of its children
    if (node->isOfType(gstSeparator::getClassTypeId())) {
        gstTransform *tempNode;
        int numChildren = ((gstSeparator *) node)->getNumChildren();

        for (int i = 0; i < numChildren; i++) {
            tempNode = ((gstSeparator *) node)->getChild(i);
            setVisible(tempNode, bVisible);
        }
    }
}

//=============================================================================
// Member Function : setGeometryLock
// ---------------------------------
// If you know that a nonstatic node is not going to be changing its geometry
// frequently, you can lock the geometry and get better rendering performance
//
// This routine will work recursively off of a separator, or you can just
// perform this on an individual node
//=============================================================================
void ghostGLManager::setGeometryLock(gstTransform *node, gstBoolean bLock)
{
    gfxDisplaySettings *displayData = getDisplaySettings(node);

    // apply the geometry lock setting to this node
    displayData->bGeometryLocked = bLock;
    
    // if the node is a separator, propogate the setting to all of its children
    if (node->isOfType(gstSeparator::getClassTypeId())) {
        gstTransform *tempNode;
        int numChildren = ((gstSeparator *) node)->getNumChildren();

        for (int i = 0; i < numChildren; i++) {
            tempNode = ((gstSeparator *) node)->getChild(i);
            setGeometryLock(tempNode, bLock);
        }
    }
}

//=============================================================================
// Member Function : clearAll
// --------------------------
// Calls clearNode() on all the nodes in the scene
//=============================================================================
void ghostGLManager::clearAll()
{
    if (m_pScene) {
        clearSeparator((gstSeparator *) m_pScene->getRoot());
    }
}

//=============================================================================
// Member Function : clearSeparator
// --------------------------------
// Calls clearNode() on all descendents of the separator
//=============================================================================
void ghostGLManager::clearSeparator(gstSeparator *sep)
{
    if (!sep) {
        return;
    }

    int numChildren = sep->getNumChildren();
    
    for (int i = 0; i < numChildren; i++) {
        gstTransform *node = sep->getChild(i);
        
        if (node->isOfType(gstSeparator::getClassTypeId())) {
            // recursion
            clearSeparator((gstSeparator *)node);
        } else {
            clearNode(node);
        }
    }
}

//=============================================================================
// Member Function : clearNode
// ---------------------------
// Frees the display lists and other GhostGL data associated with this node
// 
// Note : If you have your own user-defined callback and data, clearing the node
//        will still preserve your callback and data
//=============================================================================
void ghostGLManager::clearNode(gstTransform *node)
{
    gfxDisplaySettings *displayData = getDisplaySettings(node);
    if (displayData) {
        glDeleteLists(displayData->preDisplayList, 1);
        glDeleteLists(displayData->xformDisplayList, 1);
        glDeleteLists(displayData->geomDisplayList, 1);
        glDeleteLists(displayData->postDisplayList, 1);

        // restore user callback/data as only callback/data
        node->setGraphicsCallback(displayData->callback, displayData->userdata);
        delete displayData;
    }
}

//=============================================================================
// Member Function : setGraphicsCallback
// -------------------------------------
// Since ghostGLManager uses GHOST's own graphics callback fields to store
// information, it provides its own callback/userdata field. This method sets
// a callback and userdata pair for a given node. The graphics callback will be
// called from ghostGLManager's own graphics callback.  NULL for either argument
// will set that field to NULL. There is no "keep previous" if you only want
// to change one argument.
//=============================================================================
void ghostGLManager::setGraphicsCallback(gstTransform *node,
                                         gstGraphicsCallback *callback,
                                         void *userdata)
{
    gfxDisplaySettings *displayData = getDisplaySettings(node);
    
    displayData->callback = callback;
    displayData->userdata = userdata;
}

//=============================================================================
// Member Function : getUserData
// -----------------------------
// Returns a previously set userdata element
//=============================================================================
void *ghostGLManager::getUserData(gstTransform *node)
{
    gfxDisplaySettings *displayData = getDisplaySettings(node);
    
    return displayData->userdata;
}

//=============================================================================
// Member Function : getDisplaySettings
// ------------------------------------
// Returns the gfxDisplaySettings instance for a particular node
//=============================================================================
gfxDisplaySettings *ghostGLManager::getDisplaySettings(gstTransform *node)
{
    assert(node);
    gfxDisplaySettings *displayData;
    displayData = (gfxDisplaySettings *)node->getGraphicsCBUserData();
    
    if (!displayData) {
        setupNode(node, NULL);
        displayData = (gfxDisplaySettings *)node->getGraphicsCBUserData();
        assert(displayData);
    }
    
    return displayData;
}

//=============================================================================
// Member Function : getPhantomDisplaySettings
// -------------------------------------------
// Returns the gfxPhantomDisplaySettings node for a gstPHANToM node
//=============================================================================
gfxPhantomDisplaySettings *ghostGLManager::getPhantomDisplaySettings(gstPHANToM *phantom)
{
    assert(phantom);
    gfxPhantomDisplaySettings *displayData;
    displayData = (gfxPhantomDisplaySettings *)phantom->getGraphicsCBUserData();
    
    if (!displayData) {
        setupPhantom(phantom, NULL);
        displayData = (gfxPhantomDisplaySettings *)phantom->getGraphicsCBUserData();
        assert(displayData);
    }
    
    return displayData;
}

//=============================================================================
// Static Member Function : updateNode
// -----------------------------------
// This is the graphics callback routine used for all nodes except the PHANToM
//=============================================================================
void ghostGLManager::updateNode(gstTransform *node, void *cbData, void *userData)
{
    gfxDisplaySettings *displayData = (gfxDisplaySettings *) userData;
    assert(displayData);
    
    if (displayData) {
        if (displayData->bGeometryLocked)
            glDeleteLists(displayData->xformDisplayList, 1);
        else {
            glDeleteLists(displayData->geomDisplayList, 1);
            glDeleteLists(displayData->xformDisplayList, 1);        
        }
    }
    
    assert(displayData->gfxManager);
    displayData->gfxManager->setupNode(node, (gstTransformGraphicsCBData *) cbData);
    
    // if the user has provided a callback, call it
    if (displayData->callback)
        (*displayData->callback)(node, cbData, userData);
}

//=============================================================================
// Static Member Function : updateNode
// -----------------------------------
// This is the graphics callback routine used for just PHANToM nodes
//=============================================================================
void ghostGLManager::updatePhantom(gstTransform *node, void *cbData, void *userData)
{
    gstPHANToMGraphicsCBData *phantomdata = (gstPHANToMGraphicsCBData *)cbData;
    gfxPhantomDisplaySettings *displayData = (gfxPhantomDisplaySettings *)userData;
    
    if (displayData) {
        if (displayData->bGeometryLocked)
            glDeleteLists(displayData->xformDisplayList, 1);
        else {
            glDeleteLists(displayData->geomDisplayList, 1);
            glDeleteLists(displayData->xformDisplayList, 1);        
        }
    }
    
    assert(displayData->gfxManager);
    displayData->gfxManager->setupPhantom((gstPHANToM *)node, phantomdata);
    
    // if the user has provided a callback, call it
    if (displayData->callback)
        (*displayData->callback)(node, cbData, userData);
}

//=============================================================================
// Member Function : initEnv
// -------------------------
// Initializes the OpenGL rendering state for backface culling, depth
// buffering, lighting, and smooth shading.
//=============================================================================
void ghostGLManager::initEnv()
{
    GLfloat light_model_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light0_diffuse[] = {0.9f, 0.9f, 0.9f, 0.9f};
    
    GLfloat default_mat_color[] = {0.8f, 0.8f, 0.8f, 1.0f};
    
    // Enable depth buffering for hidden surface removal
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    // Cull back faces
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    // Other misc features
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHT0);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, default_mat_color);
}

//=============================================================================
// Member Function : setupSeparator
// --------------------------------
// Performs a recursive setup on all descendants of this separator.
//=============================================================================
void ghostGLManager::setupSeparator(gstSeparator *node)
{
    int numChildren = node->getNumChildren();
    gstTransform *tempNode;
   
    // setup this node before doing the children
    setupNode(node, NULL);
    
    for (int i = 0; i < numChildren; i++) {
        tempNode = node->getChild(i);
        
        if (tempNode->isOfType(gstPHANToM::getClassTypeId())) {
            setupPhantom((gstPHANToM *) tempNode, NULL);
        } else if (tempNode->isOfType(gstSeparator::getClassTypeId())) {
            setupSeparator((gstSeparator *) tempNode);
        } else {
            setupNode(tempNode, NULL);
        }
    }
}

//=============================================================================
// Member Function : setupNode
// ---------------------------
// This routine is used for generating the display list entries for a node.
// If a node hasn't been setup for ghostGL rendering, a gfxDisplaySettings
// instance will be created for the node. Each time afterwards, the 
// xformDisplayList entry will be updated and even the geomDisplayList if the
// geometry hasn't been locked.
//=============================================================================
void ghostGLManager::setupNode(gstTransform *transformNode,
                               gstTransformGraphicsCBData *cbData)
{
    gstBoolean bFirstTime = FALSE;

    gfxDisplaySettings *displayData =
        (gfxDisplaySettings *)transformNode->getGraphicsCBUserData();
    
    if (displayData == NULL) {
        displayData = new gfxDisplaySettings(this);
        bFirstTime = TRUE;
    }
    
    // get a display list index for the geometry
    if (displayData->xformDisplayList == 0) {
        displayData->xformDisplayList = glGenLists(1);
        assert(displayData->xformDisplayList);
    }

    // get a display list index for the geometry
    if (displayData->geomDisplayList == 0) {
        displayData->geomDisplayList = glGenLists(1);
        assert(displayData->geomDisplayList);
    }

    // always regenerate the display list for the transform

    // preserve the state of the matrix stack
    glPushMatrix();
    glNewList(displayData->xformDisplayList, GL_COMPILE);

    // transform the node
    if (cbData)
        drawTransformMatrix(cbData->transform);
    else {
        gstTransformMatrix matrix;
        transformNode->getTransformMatrix(matrix);
        
        drawTransformMatrix(matrix);
    }
    
    glEndList();

    // restore the state of the matrix stack
    glPopMatrix();

    // only create the display list for the geometry if this is the first time
    // or if the geometry isn't locked
    if (bFirstTime || !displayData->bGeometryLocked) {
        // preserve the state of the matrix stack
        glPushMatrix();
        glNewList(displayData->geomDisplayList, GL_COMPILE);

        ghostGLDraw::drawNode(transformNode, FALSE);

        glEndList();

        // restore the state of the matrix stack
        glPopMatrix();
    }
        
    // set the callback routine
    if (bFirstTime)
        transformNode->setGraphicsCallback(updateNode, displayData);
}

//=============================================================================
// Member Function : setupPhantom
// ------------------------------
// This routine is used for generating the display list entries for a gstPHANToM
// node. If the node hasn't been setup for ghostGL rendering, an instance of
// gfxPhantomDisplaySettings will be created for the node. Each time afterwards,
// the xformDisplayList entry will be updated. The geometry for a PHANToM node
// is locked by default.
//=============================================================================
void ghostGLManager::setupPhantom(gstPHANToM *phantomNode, gstPHANToMGraphicsCBData *cbData)
{
    gstBoolean bFirstTime = FALSE;
    
    gfxPhantomDisplaySettings *displayData =
        (gfxPhantomDisplaySettings *)phantomNode->getGraphicsCBUserData();
    
    if (displayData == NULL) {
        displayData = new gfxPhantomDisplaySettings(this);
        displayData->bDrawSCP = TRUE;
        displayData->bGeometryLocked = TRUE;
        bFirstTime = TRUE;
    }
    
    // get a display list index for the geometry
    if (displayData->xformDisplayList == 0) {
        displayData->xformDisplayList = glGenLists(1);
        assert(displayData->xformDisplayList);
    }

    // get a display list index for the geometry
    if (displayData->geomDisplayList == 0) {
        displayData->geomDisplayList = glGenLists(1);
        assert(displayData->geomDisplayList);
    }
        
    // always regenerate the display list for the transform

    // preserve the state of the matrix stack
    glPushMatrix();
    glNewList(displayData->xformDisplayList, GL_COMPILE);

    // transform the node
    gstTransformMatrix matrix;
    gstPoint SCP_LC;
    if (cbData) {
        matrix = cbData->transform;
        SCP_LC = phantomNode->getParent()->fromWorld(cbData->SCP_WC);
    } else {
        phantomNode->getTransformMatrix(matrix);
        phantomNode->getSCP_WC(SCP_LC);
        SCP_LC = phantomNode->getParent()->fromWorld(SCP_LC);
    }
    
    if (displayData->bDrawSCP) {
        matrix[3][0] = SCP_LC[0];
        matrix[3][1] = SCP_LC[1];
        matrix[3][2] = SCP_LC[2];
    }
    
    drawTransformMatrix(matrix);

    glEndList();
    
    // restore the state of the matrix stack
    glPopMatrix();

    // only create the display list for the geometry if this is the first time
    // or if the geometry isn't locked
    if (bFirstTime || !displayData->bGeometryLocked) {
        // preserve the state of the matrix stack
        glPushMatrix();
        glNewList(displayData->geomDisplayList, GL_COMPILE);

        ghostGLDraw::drawNode(phantomNode, FALSE);
    
        glEndList();
    
        // restore the state of the matrix stack
        glPopMatrix();
    }

    // store the current phantom callback data for safe-keeping
    if (cbData)
        m_phantomCBData = *cbData;
    
    // set the callback routine
    if (bFirstTime)
        phantomNode->setGraphicsCallback(updatePhantom, displayData);
}

//=============================================================================
// Member Function : drawScene
// ---------------------------
// If graphics is enabled, this routine will handle invoking the drawing of
// the scene. First, the events and graphics callbacks are polled. Then
// each of the nodes in the scene is drawn using callNode recursively.
//
// Note: updateEvents() is called before updateGraphics() since event callbacks
//       might introduce a change in the nodes which generates more graphics
//       callbacks that need to be called. This way we can call those callbacks
//       on this redraw as opposed to the next redraw.
//=============================================================================
void ghostGLManager::drawScene()
{
    if (m_bGraphicsOn) {
        
        // Update (dynamic) objects with event callbacks first
        m_pScene->updateEvents();
        
        // Update objects with graphics callbacks
        m_pScene->updateGraphics();
        
        if (m_bAxesOn)
            drawCoordAxes();
        
        // traverse the scene graph and draw each node
        // using its gfxDisplaySettings display lists
        callNode((gstSeparator *) m_pScene->getRoot());
    }
}

//=============================================================================
// Member Function : callDisplayLists
// ----------------------------------
// Handles calling the display lists for the passed in node. If the draw
// mode is ALL_DISPLAY_LISTS, then the display lists are called in this order:
// preDisplayList, xformDisplayList, geomDisplayList, postDisplayList
//
// If the draw mode is PRE_DISPLAY, then only the first three display lists 
// are called. The POST_DISPLAY mode will only call the postDisplayList.
//=============================================================================
void ghostGLManager::callDisplayLists(gstTransform *node, ghostGLDrawMode drawMode)
{
    //fprintf(stderr, "callDisplayLists(%s)\n", (char*)(node->getName()));
    // getDisplaySettings() will call setupNode() to setup the
    // callback and gfxDisplaySettings structure if one doesn't exist
    gfxDisplaySettings *displayData = getDisplaySettings(node);
    
    switch (drawMode) {
    case ALL_DISPLAY_LISTS:
        // call the pre OpenGL display settings
        if (displayData->preDisplayList > 0)
            glCallList(displayData->preDisplayList);
        
        // load the node's transform
        if (displayData->xformDisplayList > 0)
            glCallList(displayData->xformDisplayList);
        
        // load the node's geometry
        if (displayData->geomDisplayList > 0)
            glCallList(displayData->geomDisplayList);
        
        // call the post OpenGL display settings
        if (displayData->postDisplayList > 0)
            glCallList(displayData->postDisplayList);
        break;
        
    case PRE_DISPLAY:
        // call the pre OpenGL display settings
        if (displayData->preDisplayList > 0)
            glCallList(displayData->preDisplayList);
        
        // load the node's transform
        if (displayData->xformDisplayList > 0)
            glCallList(displayData->xformDisplayList);
        
        // load the node's geometry
        if (displayData->geomDisplayList > 0)
            glCallList(displayData->geomDisplayList);
        break;
        
    case POST_DISPLAY:
        // call the post OpenGL display settings
        if (displayData->postDisplayList > 0)
            glCallList(displayData->postDisplayList);
        break;
    }
}

//=============================================================================
// Member Function : callNode
// --------------------------
// Recursively calls the display lists for all descendants of a separator.
// If the node to be display is not a separator, then all of the display lists
// are called one after the other. If the node is a separator, then the 
// PRE_DISPLAY mode is used before calling all of the descendants. After all
// descendant have been called, the POST_DISPLAY will be called for the
// separator. This allows you to set a rendering property in the pre/post
// display list entries for a separator and affect a whole branch in the scene
//=============================================================================
void ghostGLManager::callNode(gstSeparator *node)
{
    int numChildren = node->getNumChildren();
    gstTransform *tempNode;
    
    // load the transform for this reference frame before drawing the children
    // draw the pre display lists
    callDisplayLists((gstTransform *) node, PRE_DISPLAY);
    
    for (int i = 0; i < numChildren; i++) {
        tempNode = node->getChild(i);
        
        if (tempNode->isOfType(gstSeparator::getClassTypeId())) {
            glPushMatrix();
            callNode((gstSeparator *) tempNode);
            glPopMatrix();
        } else {
            glPushMatrix();
            callDisplayLists(tempNode);
            glPopMatrix();
        }
    }

    // load the transform for this reference frame before drawing the children
    // draw the post display list
    callDisplayLists((gstTransform *) node, POST_DISPLAY);

}
