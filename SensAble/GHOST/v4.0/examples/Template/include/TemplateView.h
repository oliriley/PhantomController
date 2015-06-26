// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// TemplateView.h : interface of the CTemplateView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPLATEVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_TEMPLATEVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticView.h
#include "HapticView.h"

// Change: derive from CHapticView, not CView
class CTemplateView : public CHapticView
{
protected: // create from serialization only
	CTemplateView();
    DECLARE_DYNCREATE(CTemplateView)

// Attributes
public:
	CTemplateDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemplateView)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTemplateView();
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
	//{{AFX_MSG(CTemplateView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TemplateView.cpp
inline CTemplateDoc* CTemplateView::GetDocument()
   { return (CTemplateDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPLATEVIEW_H__C950C8FC_B9EC_11D1_B8C5_000000000000__INCLUDED_)
