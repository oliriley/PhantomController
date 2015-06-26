// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// PhantomContextDoc.cpp : implementation of the CPhantomContextDoc class
//

#include "gstAfx.h"
#include "PhantomContextApp.h"

#include "PhantomContextDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextDoc

// Change: derive from CHapticDoc, not CDocument
IMPLEMENT_DYNCREATE(CPhantomContextDoc, CHapticDoc)

// Change: derive from CHapticDoc, not CDocument
BEGIN_MESSAGE_MAP(CPhantomContextDoc, CHapticDoc)
	//{{AFX_MSG_MAP(CPhantomContextDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextDoc construction/destruction

CPhantomContextDoc::CPhantomContextDoc()
{
}

CPhantomContextDoc::~CPhantomContextDoc()
{
}

BOOL CPhantomContextDoc::OnNewDocument()
{
	if (! CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextDoc serialization

void CPhantomContextDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextDoc diagnostics

#ifdef _DEBUG
void CPhantomContextDoc::AssertValid() const
{
    // Change: assert CHapticDoc, not CDocument
	CHapticDoc::AssertValid();
}

void CPhantomContextDoc::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticDoc, not CDocument
	CHapticDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextDoc commands
