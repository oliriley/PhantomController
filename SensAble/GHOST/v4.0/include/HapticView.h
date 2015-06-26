// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#ifndef __HAPTIC_VIEW_H__
#define __HAPTIC_VIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HapticMouse.h"

class CHapticView : public CHapticMouse
{
protected: // create from serialization only

    CHapticView();
    DECLARE_DYNCREATE(CHapticView)

// Attributes
public:

    CPalette *GetPalette() {
        return &m_GLPalette;
    }

// Operations
public:

    // Client interface functions
    virtual void   StartProgram(BOOL bPHANToMMouseEnabled);
    virtual void   EndProgram();
    virtual BOOL   ProgramDone();
    virtual void   InitGraphics();
    virtual void   UpdateGraphics();
    virtual void   ResizeGraphics(int cx, int cy);
    virtual void   TermGraphics();
    virtual LPCSTR *QueryPHANToMNames();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticView)
    public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CHapticView();

    static void HapticErrorHandler(int errorId, char *message, void *userdata);

protected:
    UCHAR ComponentFromIndex(int i, UINT nbits, UINT shift);
    void CreateRGBPalette();

// Generated message map functions
protected:

    //{{AFX_MSG(CHapticView)
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:

    BOOL m_bNeedResetScreen;

    static BOOL m_bRecoverableError;  // determines whether servoloop can be restarted
    static const int kMaxNumPHANToMs; // maximum number of serviceable PHANToMs

    HGLRC       m_hRC;  // Rendering Context
    HDC         m_hDC;  // Device Context

    int m_windowWidth, m_windowHeight;

    CPalette    m_GLPalette; // Logical Palette

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif  // __HAPTIC_VIEW_H__
