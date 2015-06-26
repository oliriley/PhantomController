// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// RobotFrameworkDoc.cpp : implementation of the CRobotFrameworkDoc class
//

#include "gstAfx.h"
#include "RobotFrameworkApp.h"

#include "RobotFrameworkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkDoc

// Change: derive from CHapticDoc, not CDocument
IMPLEMENT_DYNCREATE(CRobotFrameworkDoc, CHapticDoc)

// Change: derive from CHapticDoc, not CDocument
BEGIN_MESSAGE_MAP(CRobotFrameworkDoc, CHapticDoc)
	//{{AFX_MSG_MAP(CRobotFrameworkDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkDoc construction/destruction

CRobotFrameworkDoc::CRobotFrameworkDoc()
{
}

CRobotFrameworkDoc::~CRobotFrameworkDoc()
{
}

BOOL CRobotFrameworkDoc::OnNewDocument()
{
	if (! CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkDoc serialization

void CRobotFrameworkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkDoc diagnostics

#ifdef _DEBUG
void CRobotFrameworkDoc::AssertValid() const
{
    // Change: assert CHapticDoc, not CDocument
	CHapticDoc::AssertValid();
}

void CRobotFrameworkDoc::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticDoc, not CDocument
	CHapticDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkDoc commands
