// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// RobotFrameworkDoc.h : interface of the CRobotFrameworkDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RobotFrameworkDoc_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_RobotFrameworkDoc_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticDoc.h
#include "HapticDoc.h"

// Change: derive from CHapticDoc, not CDocument
class CRobotFrameworkDoc : public CHapticDoc
{
protected: // create from serialization only
	CRobotFrameworkDoc();
	DECLARE_DYNCREATE(CRobotFrameworkDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotFrameworkDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotFrameworkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotFrameworkDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RobotFrameworkDoc_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_)
