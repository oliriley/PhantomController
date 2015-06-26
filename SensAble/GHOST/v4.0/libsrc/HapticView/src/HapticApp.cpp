// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticApp.cpp : implementation of the CHapticApp class
//                 (adaptation of MFC CWinApps to the PHANToM Mouse driver)
//

#include "stdafx.h"
#include "HapticApp.h"
#include "HapticFrame.h"
#include "HapticDoc.h"
#include "HapticView.h"
#include "HapticSplash.h"
#include "HapticConsoleIO.h"
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4305) // 'initializing' : truncation from 'const double' to 'float'

/////////////////////////////////////////////////////////////////////////////
// CHapticApp

BEGIN_MESSAGE_MAP(CHapticApp, CWinApp)
    //{{AFX_MSG_MAP(CHapticApp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticApp construction

CHapticApp::CHapticApp()
{
}

void CHapticApp::LoadFiles()
{
    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (! ProcessShellCommand(cmdInfo))
        // Clean up to default state
        OnFileNew();
}

/////////////////////////////////////////////////////////////////////////////
// CHapticApp initialization

BOOL CHapticApp::InitInstance()
{
    if (ALLOW_CONSOLE_OUTPUT)
    {
        // Create console window and enable stdout/stdin/cout/cerr on it
        RedirectIOToConsole();

        // Tell the user that console output has been enabled
        cout << "Console output has been enabled. To disable this output, remove the" << endl;
        cout << "CONSOLE_OUTPUT environment variable by using the system control panel." << endl << endl;
    }

    // CG: The following block was added by the Splash Screen component.
    {
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);
        CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
    }

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif

    // Change the registry key under which our settings are stored.
    // You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("SensAble Technologies"));

    LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

    return TRUE;
}

int CHapticApp::ExitInstance() 
{
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CHapticApp commands

BOOL CHapticApp::PreTranslateMessage(MSG* pMsg)
{
    // CG: The following lines were added by the Splash Screen component.
    if (CSplashWnd::PreTranslateAppMessage(pMsg))
        return TRUE;

    return CWinApp::PreTranslateMessage(pMsg);
}
