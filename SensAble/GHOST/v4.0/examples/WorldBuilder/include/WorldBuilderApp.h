// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// WorldBuilderApp.h
//

#if !defined(AFX_WORLDBUILDERAPP_H__C950C8F4_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_WORLDBUILDERAPP_H__C950C8F4_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderApp:
// See WorldBuilderApp.cpp for the implementation of this class
//

// Change: Include HapticApp.h
#include "HapticApp.h"

// Change: derive from CHapticApp, not CWinApp
class CWorldBuilderApp : public CHapticApp
{
public:
    CWorldBuilderApp();
    CSingleDocTemplate* GetAppTemplate() { return m_pWorldBuilderAppTemplate; }

    virtual void LoadFiles();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWorldBuilderApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CWorldBuilderApp)
    afx_msg void OnAppAbout();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    CSingleDocTemplate *m_pWorldBuilderAppTemplate;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDBUILDERAPP_H__C950C8F4_B9EC_11D1_B8C5_000000000000__INCLUDED_)
