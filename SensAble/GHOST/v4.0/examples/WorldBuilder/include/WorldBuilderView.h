// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// WorldBuilderView.h : interface of the CWorldBuilderView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDBUILDERVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_WORLDBUILDERVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticView.h
#include "HapticView.h"
class CWorldBuilderDoc;

// Change: derive from CHapticView, not CView
class CWorldBuilderView : public CHapticView
{
protected: // create from serialization only
    CWorldBuilderView();
    DECLARE_DYNCREATE(CWorldBuilderView)

// Attributes
public:
    CWorldBuilderDoc* GetDocument();

// Operations
public:
    gstBoolean isSuspended() { return m_Suspended; }
    void       setSuspended(gstBoolean state) { m_Suspended = state; }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWorldBuilderView)
    public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    protected:
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CWorldBuilderView();
    void    EnableServoLoop(BOOL bEnable);  // was protected
protected:
    // Client interface functions
    void    StartProgram(BOOL bPHANToMMouseActive);
    void    EndProgram();
    BOOL    ProgramDone();
    void    InitGraphics();
    void    UpdateGraphics();
    void    ResizeGraphics(int cx, int cy);
    void    TermGraphics();    
    LPCSTR* QueryPHANToMNames();
    BOOL    QueryCursorPos(double* pX, double* pY, double* pZ);

    gstBoolean m_Suspended;

// Generated message map functions
protected:
    //{{AFX_MSG(CWorldBuilderView)
    afx_msg void OnMouseMode();
    afx_msg void OnUpdateMouseMode(CCmdUI* pCmdUI);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);            
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG 
// debug version in WorldBuilderView.cpp
inline CWorldBuilderDoc* CWorldBuilderView::GetDocument()
   { return (CWorldBuilderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDBUILDERVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
