// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// TemplateDoc.cpp : implementation of the CTemplateDoc class
//

#include "gstAfx.h"

#include "TemplateApp.h"
#include "TemplateDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTemplateDoc

// Change: derive from CHapticDoc, not CDocument
IMPLEMENT_DYNCREATE(CTemplateDoc, CHapticDoc)

// Change: derive from CHapticDoc, not CDocument
BEGIN_MESSAGE_MAP(CTemplateDoc, CHapticDoc)
	//{{AFX_MSG_MAP(CTemplateDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateDoc construction/destruction

CTemplateDoc::CTemplateDoc()
{
}

CTemplateDoc::~CTemplateDoc()
{
}

BOOL CTemplateDoc::OnNewDocument()
{
	if (! CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTemplateDoc serialization

void CTemplateDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTemplateDoc diagnostics

#ifdef _DEBUG
void CTemplateDoc::AssertValid() const
{
    // Change: assert CHapticDoc, not CDocument
	CHapticDoc::AssertValid();
}

void CTemplateDoc::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticDoc, not CDocument
	CHapticDoc::Dump(dc);
}
#endif //_DEBUG
