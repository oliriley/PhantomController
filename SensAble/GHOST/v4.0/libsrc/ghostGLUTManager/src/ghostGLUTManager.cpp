//*****************************************************************************
//      Filename : ghostGLUTManager.cpp
//   Authored by : Chris Tarr (ctarr@sensable.com)
//    Created on : 08/05/99
// ----------------------------------------------------------------------------
//   Modified by : Brandon Itkowitz (bitkowitz@sensable.com)
//                 Philip Winston (pwinston@sensable.com)
// Last Modified : 09/08/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright (c) 1999
// All rights reserved.
//*****************************************************************************

#include "ghostGLUTManager.h"

//:Include
#include <gstScene.h>
#include <assert.h>						// for assert() macro

// Pointer to the singleton instance
ghostGLUTManager * ghostGLUTManager::m_pInstance = NULL;

ghostGLUTManager * ghostGLUTManager::CreateInstance(int argc, char *argv[], char *title)
{
    //assert(!HasInstance());			// Create only one singleton!
    if (HasInstance())
        return(NULL);
    
    m_pInstance = new ghostGLUTManager;
    m_pInstance->init(argc, argv, title);
    
    return m_pInstance;
}

void ghostGLUTManager::DestroyInstance()
{
    if (HasInstance())
    {
        delete m_pInstance;
        
        m_pInstance = 0;
    }
}

void ghostGLUTManager::glutDisplay()
{
    if (!m_pInstance)
        return;
    
    m_pInstance->redraw();
}

ghostGLUTManager * ghostGLUTManager::GetInstance()
{
    // Return a pointer to the singleton instance.
    return PeekInstance();
}

void ghostGLUTManager::init(int argc, char *argv[], char *title)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(title);
    glutDisplayFunc(glutDisplay);
    glutReshapeFunc(glutReshape);
    glutIdleFunc(glutIdle);
    
    initEnv();
}

void ghostGLUTManager::redraw()
{
    ghostGLManager::redraw();
    glutSwapBuffers();
    
    // XXX: we need to replace this with error specific handling
    //      (at least restart non-fatal, exit on fatal)
    gstScene *scene = getScene();
    if (scene && getScene()->getDoneServoLoop()) {
        // Restart servo loop
        getScene()->startServoLoop();
    }
}

bool ghostGLUTManager::ReleaseInstance(bool bAutoFree)	    // Automatically free unused resources
{
    if (HasInstance())
    {
        if (bAutoFree)
            DestroyInstance();
    }
    
    return HasInstance();
}

void ghostGLUTManager::glutReshape(int width, int height)
{
    if (!m_pInstance)
        return;
    
    m_pInstance->ghostGLManager::reshape(width, height);
}

void ghostGLUTManager::startMainloop()
{
    glutMainLoop();
}

void ghostGLUTManager::glutIdle()
{
    if (!m_pInstance)
        return;
    
    m_pInstance->redraw();
}
