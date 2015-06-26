// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticDoc.h : interface of the CHapticDoc class
//               (adaptation of MFC CDocs to the PHANToM Mouse driver)
//

#ifndef __HapticDoc_h__
#define __HapticDoc_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CHapticDoc : public CDocument
{
protected: // create from serialization only
	CHapticDoc();
	DECLARE_DYNCREATE(CHapticDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHapticDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHapticDoc();

// Generated message map functions
protected:
	//{{AFX_MSG(CHapticDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __HapticDoc_h__
