// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// RobotFrameworkView.h : interface of the CRobotFrameworkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RobotFrameworkView_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_RobotFrameworkView_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticView.h
#include "HapticView.h"

// Change: derive from CHapticView, not CView
class CRobotFrameworkView : public CHapticView
{
protected: // create from serialization only
	CRobotFrameworkView();
    DECLARE_DYNCREATE(CRobotFrameworkView)

// Attributes
public:
	CRobotFrameworkDoc* GetDocument();

// Operations
public:
    void ToggleGraphics();
    void ToggleHaptics();
    void ToggleMouse();
    BOOL GetGraphics();
    BOOL GetHaptics();
    BOOL GetMouse();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotFrameworkView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotFrameworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    // Client interface functions
    void    StartProgram(BOOL bPHANToMMouseActive);
    void    EndProgram();
    BOOL    ProgramDone();
    void    InitGraphics();
    void    UpdateGraphics();
    void    ResizeGraphics(int cx, int cy);
    void    TermGraphics();
    void    EnableServoLoop(BOOL bEnable);
    LPCSTR* QueryPHANToMNames();
    BOOL    QueryCursorPos(double* pX, double* pY, double* pZ);

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CRobotFrameworkView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RobotFrameworkView.cpp
inline CRobotFrameworkDoc* CRobotFrameworkView::GetDocument()
   { return (CRobotFrameworkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RobotFrameworkView_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
