/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  HapticViewer.h

Description: 

  MFC CWinApp derived application class for haptic viewer.

******************************************************************************/

#if !defined(AFX_HAPTICVIEWER_H__AF5EEA05_A057_11D4_A8D5_000086513E90__INCLUDED_)
#define AFX_HAPTICVIEWER_H__AF5EEA05_A057_11D4_A8D5_000086513E90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerApp:
//

class CHapticViewerApp : public CWinApp
{
public:
    CHapticViewerApp();
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticViewerApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CHapticViewerApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        };


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HAPTICVIEWER_H__AF5EEA05_A057_11D4_A8D5_000086513E90__INCLUDED_)
