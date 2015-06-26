//*****************************************************************************
//      Filename : ghostGLUTManager.h
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

#ifndef GHOST_GLUT_MANAGER_H
#define GHOST_GLUT_MANAGER_H

#include <ghostGLManager.h>

#include <glut.h>

#define DEFAULT_WINDOW_TITLE    "GhostGLUTManager Application"

// Adds windowing with GLUT to ghostGLManager.  With ghostGLUTManager only a few
// lines of code can create a fully functional graphical-haptic simulation.
class ghostGLUTManager: public ghostGLManager
{
public:
    
    //
    // Creates the instance of the singleton class.  If a singleton factory object has
    // been registered (see SetFactory()), This function calls the factory object to
    // create the singleton instance.  If no factory object is registered, this
    // function directly creates a singleton instance.
    //
    static ghostGLUTManager *CreateInstance(int argc, char *argv[], char *title = DEFAULT_WINDOW_TITLE);
    
    //
    // Returns a pointer to the singleton instance if one has been created.  Otherwise
    // NULL is returned.
    //
    static ghostGLUTManager *GetInstance();
    
    //
    // Call this function to determine if a singleton instance currently exists.  This
    // function returns TRUE (non-zero) a singleton instance does exist.
    //
    inline static bool HasInstance();
    
    //
    // Call this function to directly obtain a pointer to the singleton instance.  If
    // no such instance exists, this function returns zero without creating a new
    // instance.
    //
    // CAUTION:
    //
    // This is a low-level function.  In general you should use GetInstance() instead
    // of this function.  This function is provided for effeciency and should be use
    // with caution.
    //
    inline static ghostGLUTManager *PeekInstance();
    
    //
    // Releases a pointer to the singleton object previously obtained by calling
    // GetInstance().
    //
    // The function returns TRUE if a singleton instance still exists upon return from
    // this function.
    //
    static bool ReleaseInstance(bool bAutoFree = FALSE);    // Automatically free unused resources
    
    static void glutReshape(int width, int height);
    static void glutDisplay();
    static void glutIdle();
    
    virtual void redraw();
    
    void startMainloop();
    
protected:
    
    //
    // Protected default constructor prevents direct creation of instances of this
    // class.  Actual instance creation is peformed by the CreateInstance() member
    // function.
    //
    ghostGLUTManager(){}
    
    //
    // Protected destructor prevents direct deletion of the singleton instance.  Actual
    // instance deletion is performed by the DestroyInstance() member function.
    //
    virtual ~ghostGLUTManager(){}
    
    
    void init(int argc, char *argv[], char *title = DEFAULT_WINDOW_TITLE);
    
    //
    // Destroys the singleton instance (if such an instance exists).
    //
    static void DestroyInstance();
    
private:
    
    //
    // Pointer to the singleton instance
    //
    static ghostGLUTManager *m_pInstance;	// Pointer to the singleton instance
    
    //
    // Disabled copy constructor.  This constructor is declared private but not defined
    // to keep the compiler from generating a copy constructor and to make it
    // impossible create an instance of this class using the copy constructor.
    //
    // Any attempt by code outside of this class to access (either explicitly or
    // implicitly) the copy constructor for this class will result in a compiler error
    // because the constructor is declared private. An attempt to access the copy
    // constructor from within this class will compile but will fail at link time
    // because the constructor is not defined.
    //
    ghostGLUTManager(const ghostGLUTManager& rhs_);		// Existing object being copied
};

inline bool ghostGLUTManager::HasInstance()
{
    return m_pInstance != 0;
}

inline ghostGLUTManager *ghostGLUTManager::PeekInstance()
{
    return m_pInstance;
}

#endif // GHOST_GLUT_MANAGER_H

