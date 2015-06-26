// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

//
// HapticFrame.cpp : implementation of the CHapticFrame class
//                   (adaptation of MFC CFrameWnds to the PHANToM Mouse driver)
//

#include "stdafx.h"
#include "HapticFrame.h"
#include "HapticView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHapticFrame

IMPLEMENT_DYNCREATE(CHapticFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CHapticFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CHapticFrame)
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticFrame construction/destruction

CHapticFrame::CHapticFrame()
{
}

CHapticFrame::~CHapticFrame()
{
}

BOOL CHapticFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style = WS_OVERLAPPED | WS_CAPTION /* | FWS_ADDTOTITLE */
        | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

    // Center a standard size window on the desktop
    CWnd* desktop = GetDesktopWindow();
    CRect rctDesktop, rctWindow;
    desktop->GetClientRect(rctDesktop);

#define WINDOW_WIDTH    700         // Client area dimensions in pixels
#define WINDOW_HEIGHT   525

    rctWindow.left   = (rctDesktop.Width() - WINDOW_WIDTH) / 2;
    rctWindow.top    = (rctDesktop.Height() - WINDOW_HEIGHT) / 2;
    rctWindow.right  = rctWindow.left + WINDOW_WIDTH;
    rctWindow.bottom = rctWindow.top  + WINDOW_HEIGHT;

    // Compute position and dimensions of top level window
    if (! AdjustWindowRectEx(&rctWindow, cs.style, TRUE, cs.dwExStyle))
        return FALSE;

    cs.x = rctWindow.left;
    cs.y = rctWindow.top;
    cs.cx = rctWindow.right  - rctWindow.left;
    cs.cy = rctWindow.bottom - rctWindow.top;

    return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHapticFrame message handlers

// Route message to CView derived class
void CHapticFrame::OnPaletteChanged(CWnd* pFocusWnd) {
    CView* pView = GetActiveView();
    if (pView) {
        // OnPaletteChanged is not public, so send a message.
        pView->SendMessage(WM_PALETTECHANGED,
                           (WPARAM)(pFocusWnd->GetSafeHwnd()),
                           (LPARAM)0);
    }
}

// Route message to CView derived class
BOOL CHapticFrame::OnQueryNewPalette() {
	CView* pView = GetActiveView();
    if (pView) {
        // OnQueryNewPalette is not public, so send a message.
        return pView->SendMessage(WM_QUERYNEWPALETTE,
                                  (WPARAM)0,
                                  (LPARAM)0);
    }

    return FALSE; 
}
