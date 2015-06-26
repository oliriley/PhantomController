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

#ifndef GHOSTGL_MANAGER_H
#define GHOSTGL_MANAGER_H

#include <ghostGLDraw.h>

#include <gstBasic.h>
#include <gstTransform.h>
#include <gstPHANToM.h>
#include <gstScene.h>

class ghostGLManager;
class ghostGLCameraBase;
class ghostGLActionObject;

class gfxDisplaySettings;
class gfxPhantomDisplaySettings;

// The GhostGL Manager class makes up the core of the GhostGL library. It is
// primarily responsible for managing a scene graph that has been populated with
// GHOST primitives. In doing so, it sets up the necessary graphics callbacks
// and manages OpenGL display lists for drawing each node in a scene.
//
// When the glManager loads a scene, it creates a gfxDisplaySettings structure
// for each node in the scene. This structure holds a series of display lists
// that get called whenever a node is drawn. By default, all gstShape primitives
// in the scene will have geometry display lists generated for them by using
// the drawing routines in ghostGLDraw. After a scene has been loaded, you can
// access the gfxDisplaySettings for any node in the scene to change rendering
// properties, or even swap in your own geometry display list for rendering.
//
// This implementation has not been tested for use with multiple views of a scene
//
class ghostGLManager : public ghostGLDraw
{
    typedef enum {
        ALL_DISPLAY_LISTS = 0,
        PRE_DISPLAY,
        POST_DISPLAY
    } ghostGLDrawMode;
    
public:
    
    ghostGLManager(ghostGLCameraBase *camera = NULL);
    virtual ~ghostGLManager();
	
    // Accessor methods for enabling/disabling drawing
    void setGraphicsOn(gstBoolean bEnable) {
        m_bGraphicsOn = bEnable;
    }
	
    gstBoolean isGraphicsOn() const {
        return m_bGraphicsOn;
    }
    
    // Accessor methods for turning on/off the coordinate origin 
    void setAxesOn(gstBoolean bEnable) {
        m_bAxesOn = bEnable;
    }
	
    gstBoolean isAxesOn() const {
        return m_bAxesOn;
    }
    
    // Returns the first PHANToM instance in the pool of current PHANToMs
    gstPHANToM *getPhantomNode() const {
        return gstPHANToM::getPHANToMsInScene();
    }
    
    gstScene *getScene() {
        return m_pScene;
    }
	
    ghostGLCameraBase *getCamera() const {
        return (ghostGLCameraBase *) m_pCamera;
    }
    
    // Get data about the PHANToM from the last graphics callback
    gstTransformMatrix getPhantomTransform() const {
        return m_phantomCBData.transform;
    }
    
    gstTransformMatrix getPhantomCumulativeTransform() const {
        return m_phantomCBData.cumulativeTransform;
    }
	
    virtual void loadScene(gstScene *scene);
    virtual void redraw();
    virtual void reshape(int width, int height);
	
    virtual void setCamera(ghostGLCameraBase *camera);
    
    virtual void addActionObject(ghostGLActionObject *obj);
    virtual void removeActionObject(ghostGLActionObject *obj);
	
    // Change properties about the scene in a local or global manner
    void setTouchableByPHANToM(gstBoolean bTouchable);    
    void setVisible(gstTransform *node, gstBoolean bVisible);
    void setGeometryLock(gstTransform *node, gstBoolean bLock);
	
    // Cleanup methods that remove the gfxDisplaySettings and callback related data
    void clearAll();
    void clearSeparator(gstSeparator *sep);
    void clearNode(gstTransform *node);
	
    void setGraphicsCallback(gstTransform *node,
        gstGraphicsCallback *callback,
        void *userdata = NULL);
	
    void *getUserData(gstTransform *node);
    
    gfxDisplaySettings *getDisplaySettings(gstTransform *node);
    gfxPhantomDisplaySettings *getPhantomDisplaySettings(gstPHANToM *phantom);
	
    static void updateNode(gstTransform *node, void *cbData, void *userData);
    static void updatePhantom(gstTransform *node, void *cbData, void *userData);
	
protected:
	
    virtual void initEnv();
	
    // Methods used for setting up nodes for GhostGL rendering
    virtual void setupSeparator(gstSeparator *node);
    virtual void setupNode(gstTransform *transformNode,
        gstTransformGraphicsCBData *cbData);
    virtual void setupPhantom(gstPHANToM *phantomNode,
        gstPHANToMGraphicsCBData *cbData);
	
    // Method used for rendering the scene during each redraw
    virtual void drawScene();
    void callDisplayLists(gstTransform *node,
        ghostGLDrawMode drawMode = ALL_DISPLAY_LISTS);
    void callNode(gstSeparator *node);
	
    // The list of action objects
    list<ghostGLActionObject *> m_actionObjs;
	
private:
	
    gstBoolean m_bGraphicsOn; 
    gstBoolean m_bAxesOn;
    gstBoolean m_bUserCamera;
    
    gstScene *m_pScene;
    ghostGLCameraBase *m_pCamera;
    
    // Storage for the PHANTOM's last callback data
    gstPHANToMGraphicsCBData m_phantomCBData;
};

// Per node display lists and settings
class gfxDisplaySettings
{
public:
    
    gfxDisplaySettings(ghostGLManager *gfxManager_ = NULL) :
	  gfxManager(gfxManager_),
		  preDisplayList(0),
		  xformDisplayList(0),
		  geomDisplayList(0),
		  postDisplayList(0),
		  bGeometryLocked(bDefaultGeometryLock),
		  callback(NULL),
		  userdata(NULL) {}
      
	  ghostGLManager *gfxManager; // ghostGLManager for this node
	  
	  int preDisplayList;         // set material properties, texture mapping, etc.
	  int xformDisplayList;       // compiled OpenGL display list that manipulates the ModelView matrix stack
	  int geomDisplayList;        // compiled OpenGL display list that renders this node's geometry
	  int postDisplayList;        // unset material properties, texture mapping, etc.
	  
	  gstBoolean bGeometryLocked; // when enabled, this can lead to significant speed increase
	  static gstBoolean bDefaultGeometryLock;
	  
	  gstGraphicsCallback *callback; // used for storing userdefined callback
	  void *userdata;                // used for storing userdata for callback
	  
	  static void setDefaultGeometryLock(gstBoolean bLock) {
		  bDefaultGeometryLock = bLock;
	  }
};

// Display lists and settings for the gstPHANToM node
class gfxPhantomDisplaySettings : public gfxDisplaySettings
{
public:
	
    gfxPhantomDisplaySettings(ghostGLManager *m) : gfxDisplaySettings(m) {}
	
    gstBoolean bDrawSCP;
};


#endif // GHOSTGL_MANAGER_H
