//=============================================================================
//   Filename : ghostGLActionObject.h
// Written by : Chris Tarr (ctarr@sensable.com)
//              Philip Winston (pwinston@sensable.com)
//    Project : GhostGL 1.0
//     Module : GhostGL Action Object
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GHOST_GL_ACTION_OBJECT_H
#define GHOST_GL_ACTION_OBJECT_H

class ghostGLManager;

// Base class for action objects such as the pinch transform.
class ghostGLActionObject
{
    friend class ghostGLManager;
    
public:

    // remove is called when ghostGLManager::removeActionObject()
    // is called on this object
    virtual void remove() {};

    virtual ~ghostGLActionObject() {};
    
protected:

    // constructor is protected because it is not meaningful to
    // instantiate a ghostGLActionObject, only objects derived from it
    ghostGLActionObject(){};
    
    // accessor
    ghostGLManager *getGLManager() {
        return m_glManager;
    }
    
    // assign new manager
    void setGLManager(ghostGLManager *glManager) {
        m_glManager = glManager;
    }
    
    // called before each display iteration
    virtual void preDisplay(){};
    // called after each display iteration
    virtual void postDisplay(){};
    
    // pointer to our manager
    ghostGLManager *m_glManager;
};

#endif // GHOST_GL_ACTION_OBJECT_H


