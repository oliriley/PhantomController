// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticFrame.h : interface of the CHapticFrame class
//                 (adaptation of MFC CFrameWnds to the PHANToM Mouse driver)
//

#ifndef __HapticFrame_h__
#define __HapticFrame_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CHapticFrame : public CFrameWnd
{
protected: // create from serialization only
    CHapticFrame();
    DECLARE_DYNCREATE(CHapticFrame)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CHapticFrame();

// Generated message map functions
protected:
    //{{AFX_MSG(CHapticFrame)
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __HapticFrame_h__
