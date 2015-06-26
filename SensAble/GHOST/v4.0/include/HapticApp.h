// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticApp.h : interface of the CHapticApp class
//               (adaptation of MFC CWinApps to the PHANToM Mouse driver)
//

#ifndef __HapticApp_h__
#define __HapticApp_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

// #include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHapticApp:
// See HapticApp.cpp for the implementation of this class
//

class CHapticApp : public CWinApp
{
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    CHapticApp();
//  virtual BOOL    CalibrateDialog();
    virtual void    LoadFiles();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CHapticApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __HapticApp_h__
