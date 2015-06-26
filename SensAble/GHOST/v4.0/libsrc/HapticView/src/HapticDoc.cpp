// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticDoc.cpp : implementation of the CHapticDoc class
//                 (adaptation of MFC CDocs to the PHANToM Mouse driver)
//

#include "stdafx.h"
#include "HapticApp.h"
#include "HapticDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHapticDoc

IMPLEMENT_DYNCREATE(CHapticDoc, CDocument)

BEGIN_MESSAGE_MAP(CHapticDoc, CDocument)
	//{{AFX_MSG_MAP(CHapticDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticDoc construction/destruction

CHapticDoc::CHapticDoc()
{
}

CHapticDoc::~CHapticDoc()
{
}

BOOL CHapticDoc::OnNewDocument()
{
	if (! CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHapticDoc serialization

void CHapticDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHapticDoc commands
