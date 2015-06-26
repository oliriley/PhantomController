// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// WorldBuilderView.cpp : implementation of the CWorldBuilderView class
//

#include "gstAfx.h"
#include "WorldBuilderApp.h"
#include "WorldBuilderDoc.h"
#include "WorldBuilderView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderView

// Change: derive from CHapticView, not CView
IMPLEMENT_DYNCREATE(CWorldBuilderView, CHapticView)

// Change: derive from CHapticView, not CView
BEGIN_MESSAGE_MAP(CWorldBuilderView, CHapticView)
    ON_WM_CHAR()
    ON_WM_KEYUP()
    ON_WM_KEYDOWN()
    ON_WM_CONTEXTMENU()
    //{{AFX_MSG_MAP(CWorldBuilderView)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    ON_COMMAND(IDM_MOUSEMODE, OnMouseMode)
    ON_UPDATE_COMMAND_UI(IDM_MOUSEMODE, OnUpdateMouseMode)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderView construction/destruction

CWorldBuilderView::CWorldBuilderView()
{
    // servo loop not running at first
    m_Suspended = TRUE;
}

CWorldBuilderView::~CWorldBuilderView()
{
}

BOOL CWorldBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticView, not CView
    return CHapticView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderView message handlers


void CWorldBuilderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CHapticView::OnKeyDown(nChar, nRepCnt, nFlags);

    switch (nChar) {
        case 'G':
            bool previouslyUp = ((nFlags >> 14) & 1) == 0;
            if (previouslyUp) {
                GetDocument()->startGrab();
            }
            break;
    }
}

void CWorldBuilderView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CHapticView::OnKeyUp(nChar, nRepCnt, nFlags);

    switch (nChar) {
        case 'G':
            GetDocument()->endGrab();
            break;
    }
}


/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderView interface functions

void CWorldBuilderView::StartProgram(BOOL bPHANToMMouseActive)
{
    // Reset the PHANToM only if the PHANToM Mouse simulation is not in effect
    gstBoolean PHANToMReset = ! bPHANToMMouseActive;

    GetDocument()->InitHaptics(PHANToMReset);

      // must call reshape now that ghostGL is up and running so that
      // it can resize the camera to the current window
    RECT rect;
    GetClientRect(&rect);
    ResizeGraphics(rect.right, rect.bottom);  

    // start with mouse mode off since it might be confusing
    // to novice users
    EnablePHANToMMouse(FALSE);
}

void CWorldBuilderView::EndProgram()
{

}

BOOL CWorldBuilderView::ProgramDone()
{
    return !m_Suspended && !GetDocument()->isSceneRunning();
}

void CWorldBuilderView::InitGraphics()
{
}

void CWorldBuilderView::UpdateGraphics()
{
    GetDocument()->Redraw();    
}

void CWorldBuilderView::ResizeGraphics(int w, int h)
{
    /* Correct for zero height/width. */
    h = h ? h : 1;
    w = w ? w : 1;

    GetDocument()->Reshape(w, h);
}

void CWorldBuilderView::TermGraphics()
{
    
}

void CWorldBuilderView::EnableServoLoop(BOOL bEnable)
{
    GetDocument()->EnableServoLoop(bEnable);

}

LPCSTR* CWorldBuilderView::QueryPHANToMNames()
{
    static LPCSTR   apszNames[] = {
                        PHANTOM_NAME,
                        NULL
                        };
    return apszNames;
}

BOOL CWorldBuilderView::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    return GetDocument()->QueryCursorPos(pX, pY, pZ);
}

void CWorldBuilderView::OnMouseMode()
{
    EnablePHANToMMouse(!IsPHANToMMouseEnabled());
}

void CWorldBuilderView::OnUpdateMouseMode(CCmdUI* pCmdUI)
{
    ASSERT(pCmdUI);
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(IsPHANToMMouseEnabled());
}


#ifdef _DEBUG
    // non-debug version is inline
    CWorldBuilderDoc* CWorldBuilderView::GetDocument() 
    {
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorldBuilderDoc)));
        return (CWorldBuilderDoc*)m_pDocument;
    }
#endif //_DEBUG

