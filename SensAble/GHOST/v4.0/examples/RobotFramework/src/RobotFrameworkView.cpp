// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// RobotFrameworkView.cpp : implementation of the CRobotFrameworkView class
//

#include "gstAfx.h"
#include "RobotFrameworkApp.h"

#include "RobotFrameworkDoc.h"
#include "RobotFrameworkView.h"
#include "resource.h"

#include <RobotFramework.h>
#include <RobotFramework_graphics.h>
#include <RobotFramework_haptics.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkView

// Change: derive from CHapticView, not CView
IMPLEMENT_DYNCREATE(CRobotFrameworkView, CHapticView)

// Change: derive from CHapticView, not CView
BEGIN_MESSAGE_MAP(CRobotFrameworkView, CHapticView)
	//{{AFX_MSG_MAP(CRobotFrameworkView)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkView construction/destruction

CRobotFrameworkView::CRobotFrameworkView()
{
}

CRobotFrameworkView::~CRobotFrameworkView()
{
}

BOOL CRobotFrameworkView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticView, not CView
	return CHapticView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkView diagnostics

#ifdef _DEBUG
void CRobotFrameworkView::AssertValid() const
{
    // Change: assert CHapticView, not CView
	CHapticView::AssertValid();
}

void CRobotFrameworkView::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticView, not CView
	CHapticView::Dump(dc);
}

CRobotFrameworkDoc* CRobotFrameworkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotFrameworkDoc)));
	return (CRobotFrameworkDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkView message handlers

void CRobotFrameworkView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Change: pass on to CHapticView, not CView
    CHapticView::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CRobotFrameworkView interface functions

void CRobotFrameworkView::StartProgram(BOOL bPHANToMMouseActive)
{
    // Reset the PHANToM only if the PHANToM Mouse simulation is not in effect
    int bResetPHANToM = !bPHANToMMouseActive;

    start_program(bResetPHANToM);
}

void CRobotFrameworkView::EndProgram()
{
    end_program();
}

BOOL CRobotFrameworkView::ProgramDone()
{
    return scene_done();
}

void CRobotFrameworkView::InitGraphics()
{
    init_graphics();
}

void CRobotFrameworkView::UpdateGraphics()
{
    update_graphics();
}

void CRobotFrameworkView::ResizeGraphics(int cx, int cy)
{
    reshape(cx, cy);
}

void CRobotFrameworkView::TermGraphics()
{
    term_graphics();
}

void CRobotFrameworkView::EnableServoLoop(BOOL bEnable)
{
    enable_servo_loop(bEnable);
}

LPCSTR* CRobotFrameworkView::QueryPHANToMNames()
{
    static LPCSTR   apszNames[] = {
                        PHANTOM_NAME,
                        NULL
                        };
    return apszNames;
}

BOOL CRobotFrameworkView::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    return query_phantom_pos(pX, pY, pZ);
}

