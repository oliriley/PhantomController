// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// PhantomContextView.h : interface of the CPhantomContextView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GHOSTAPPVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_GHOSTAPPVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticView.h
#include "HapticView.h"
#include "PhantomContextDoc.h"

// Change: derive from CHapticView, not CView
class CPhantomContextView : public CHapticView
{
protected: // create from serialization only
	CPhantomContextView();
    DECLARE_DYNCREATE(CPhantomContextView)

// Attributes
public:
	CPhantomContextDoc* GetDocument();

// Operations
public:
    virtual void OnPhantomLeave();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhantomContextView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhantomContextView();
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
	//{{AFX_MSG(CPhantomContextView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseToggle();
	afx_msg void OnUpdateMouse(CCmdUI* pCmdUI);
	afx_msg void OnEnableFallThru();
	afx_msg void OnUpdateFallThru(CCmdUI* pCmdUI);
	afx_msg void OnEnableClickThru();
	afx_msg void OnUpdateClickThru(CCmdUI* pCmdUI);
	afx_msg void OnEnablePushThru();
	afx_msg void OnUpdatePushThru(CCmdUI* pCmdUI);
	afx_msg void OnSetLeftButton();
	afx_msg void OnUpdateLeftButton(CCmdUI* pCmdUI);
	afx_msg void OnSetMiddleButton();
	afx_msg void OnUpdateMiddleButton(CCmdUI* pCmdUI);
	afx_msg void OnSetRightButton();
	afx_msg void OnUpdateRightButton(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PhantomContextView.cpp
inline CPhantomContextDoc* CPhantomContextView::GetDocument()
   { return (CPhantomContextDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GHOSTAPPVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
