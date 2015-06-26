// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "gstAfx.h"
#include "RobotFrameworkApp.h"

#include "MainFrm.h"
#include "HapticSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

// Change: derive from CHapticFrame, not CFrameWnd
IMPLEMENT_DYNCREATE(CMainFrame, CHapticFrame)

// Change: derive from CHapticFrame, not CFrameWnd
BEGIN_MESSAGE_MAP(CMainFrame, CHapticFrame)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticFrame, not CFrameWnd
	return CHapticFrame::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    // Change: assert CHapticFrame, not CFrameWnd
	CHapticFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticFrame, not CFrameWnd
	CHapticFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHapticFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	return 0;
}
