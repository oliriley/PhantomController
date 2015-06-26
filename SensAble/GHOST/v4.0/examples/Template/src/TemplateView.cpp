// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// TemplateView.cpp : implementation of the CTemplateView class
//

#include "gstAfx.h"

#include "TemplateApp.h"
#include "TemplateDoc.h"
#include "TemplateView.h"
#include "resource.h"

#include "Template.h"
#include "Template_graphics.h"
#include "Template_haptics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTemplateView

// Change: derive from CHapticView, not CView
IMPLEMENT_DYNCREATE(CTemplateView, CHapticView)

// Change: derive from CHapticView, not CView
BEGIN_MESSAGE_MAP(CTemplateView, CHapticView)
	//{{AFX_MSG_MAP(CTemplateView)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateView construction/destruction

CTemplateView::CTemplateView()
{
}

CTemplateView::~CTemplateView()
{
}

BOOL CTemplateView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticView, not CView
	return CHapticView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTemplateView diagnostics

#ifdef _DEBUG
void CTemplateView::AssertValid() const
{
    // Change: assert CHapticView, not CView
	CHapticView::AssertValid();
}

void CTemplateView::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticView, not CView
	CHapticView::Dump(dc);
}

CTemplateDoc* CTemplateView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTemplateDoc)));
	return (CTemplateDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTemplateView message handlers

void CTemplateView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Change: pass on to CHapticView, not CView
    CHapticView::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CTemplateView interface functions

void CTemplateView::StartProgram(BOOL bPHANToMMouseActive)
{
    // Reset the PHANToM only if the PHANToM Mouse simulation is not in effect
    BOOL bResetPHANToM = !bPHANToMMouseActive;

    start_program(bResetPHANToM);
}

void CTemplateView::EndProgram()
{
    end_program();
}

BOOL CTemplateView::ProgramDone()
{
    return scene_done();
}

void CTemplateView::InitGraphics()
{
    init_graphics();
}

void CTemplateView::UpdateGraphics()
{
    update_graphics();
}

void CTemplateView::ResizeGraphics(int cx, int cy)
{
    reshape(cx, cy);
}

void CTemplateView::TermGraphics()
{
    term_graphics();
}

void CTemplateView::EnableServoLoop(BOOL bEnable)
{
    enable_servo_loop(bEnable);
}

LPCSTR* CTemplateView::QueryPHANToMNames()
{
    static LPCSTR   apszNames[] = {
                        PHANTOM_NAME,
                        NULL
                        };
    return apszNames;
}

BOOL CTemplateView::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    return query_phantom_pos(pX, pY, pZ);
}

