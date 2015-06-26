// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// PhantomContextView.cpp : implementation of the CPhantomContextView class
//

#include "gstAfx.h"

#include "PhantomContextApp.h"
#include "PhantomContextDoc.h"
#include "PhantomContextView.h"
#include "resource.h"

#include "PhantomContext.h"
#include "PhantomContext_graphics.h"
#include "PhantomContext_haptics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextView

// Change: derive from CHapticView, not CView
IMPLEMENT_DYNCREATE(CPhantomContextView, CHapticView)

// Change: derive from CHapticView, not CView
BEGIN_MESSAGE_MAP(CPhantomContextView, CHapticView)
	//{{AFX_MSG_MAP(CPhantomContextView)
	ON_WM_CHAR()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONUP()
    ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPTIONS_PHANTOMMOUSE, OnMouseToggle)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_PHANTOMMOUSE, OnUpdateMouse)
	ON_COMMAND(ID_FALLTHRU, OnEnableFallThru)
	ON_UPDATE_COMMAND_UI(ID_FALLTHRU, OnUpdateFallThru)
	ON_COMMAND(ID_CLICKTHRU, OnEnableClickThru)
	ON_UPDATE_COMMAND_UI(ID_CLICKTHRU, OnUpdateClickThru)
	ON_COMMAND(ID_PUSHTHRU, OnEnablePushThru)
	ON_UPDATE_COMMAND_UI(ID_PUSHTHRU, OnUpdatePushThru)
	ON_COMMAND(ID_LEFTBUTTON, OnSetLeftButton)
	ON_UPDATE_COMMAND_UI(ID_LEFTBUTTON, OnUpdateLeftButton)
	ON_COMMAND(ID_MIDDLEBUTTON, OnSetMiddleButton)
	ON_UPDATE_COMMAND_UI(ID_MIDDLEBUTTON, OnUpdateMiddleButton)
	ON_COMMAND(ID_RIGHTBUTTON, OnSetRightButton)
	ON_UPDATE_COMMAND_UI(ID_RIGHTBUTTON, OnUpdateRightButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextView construction/destruction

CPhantomContextView::CPhantomContextView()
{
}

CPhantomContextView::~CPhantomContextView()
{
}

BOOL CPhantomContextView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticView, not CView
	return CHapticView::PreCreateWindow(cs);
}

void CPhantomContextView::OnPhantomLeave()
{
#if 0
    double frustumHeight;
    PHANToMMouseConfig config;

    // determine the appropriate scale for the mouse movement, given the depth
    // of the PHANToM cursor
    frustumHeight = get_frustum_height();

    // Update the mouse configuration with the new scale factor
    config = GetPHANToMMouseConfig();
    config.nMouseScale = m_windowHeight / frustumHeight;
    SetPHANToMMouseConfig(config);
#endif
    
    CHapticView::OnPhantomLeave();
}

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextView diagnostics

#ifdef _DEBUG
void CPhantomContextView::AssertValid() const
{
    // Change: assert CHapticView, not CView
	CHapticView::AssertValid();
}

void CPhantomContextView::Dump(CDumpContext& dc) const
{
    // Change: dump CHapticView, not CView
	CHapticView::Dump(dc);
}

CPhantomContextDoc* CPhantomContextView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhantomContextDoc)));
	return (CPhantomContextDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextView message handlers

void CPhantomContextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar) {
    case 'M':
    case 'm':
        OnMouseToggle();
        break;
    case 'F':
    case 'f':
	    SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_FALL_THRU);
	    break;
    case 'C':
    case 'c':
        SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_CLICK_THRU);
        break;
    case 'P':
    case 'p':
        SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_PUSH_THRU);
        break;
    case 'A':
    case 'a':
        toggle_axes(); // make a call into the PhantomContext_graphics layer
        break;

    case '1':
        m_buttonMask ^= MOUSE_LEFT_BUTTON_MASK;
        SetWhichButton(m_buttonMask);
        break;
    case '2':
        m_buttonMask ^= MOUSE_MIDDLE_BUTTON_MASK;
        SetWhichButton(m_buttonMask);
        break;
    case '3':
        m_buttonMask ^= MOUSE_RIGHT_BUTTON_MASK;
        SetWhichButton(m_buttonMask);
        break;
    }

    // Change: pass on to CHapticView, not CView
    CHapticView::OnChar(nChar, nRepCnt, nFlags);
}

void CPhantomContextView::OnLButtonUp(UINT nFlags, CPoint point)
{
    CHapticView::OnLButtonUp(nFlags, point);

    cout << "Left Mouse Button Pressed" << endl;
}

void CPhantomContextView::OnMButtonUp(UINT nFlags, CPoint point)
{
    CHapticView::OnMButtonUp(nFlags, point);

    cout << "Middle Mouse Button Pressed" << endl;
}

void CPhantomContextView::OnRButtonUp(UINT nFlags, CPoint point)
{
    CHapticView::OnRButtonUp(nFlags, point);

    cout << "Right Mouse Button Pressed" << endl;
}

void CPhantomContextView::OnMouseToggle() 
{
    EnablePHANToMMouse(!IsPHANToMMouseEnabled());    
}

void CPhantomContextView::OnUpdateMouse(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(IsPHANToMMouseEnabled());
}
void CPhantomContextView::OnEnableFallThru() 
{
    SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_FALL_THRU);    	
}

void CPhantomContextView::OnUpdateFallThru(CCmdUI* pCmdUI) 
{
    if (GetHapticEntryMode() == CHapticView::PHANTOM_MOUSE_FALL_THRU)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CPhantomContextView::OnEnableClickThru() 
{
    SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_CLICK_THRU);	
}

void CPhantomContextView::OnUpdateClickThru(CCmdUI* pCmdUI) 
{
    if (GetHapticEntryMode() == CHapticView::PHANTOM_MOUSE_CLICK_THRU)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);	
}

void CPhantomContextView::OnEnablePushThru() 
{
    SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_PUSH_THRU);
}

void CPhantomContextView::OnUpdatePushThru(CCmdUI* pCmdUI) 
{
    if (GetHapticEntryMode() == CHapticView::PHANTOM_MOUSE_PUSH_THRU)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);	
}

void CPhantomContextView::OnSetLeftButton() 
{
    m_buttonMask ^= MOUSE_LEFT_BUTTON_MASK;
    SetWhichButton(m_buttonMask);
}

void CPhantomContextView::OnUpdateLeftButton(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck((m_buttonMask & MOUSE_LEFT_BUTTON_MASK) != 0);
}

void CPhantomContextView::OnSetMiddleButton() 
{
    m_buttonMask ^= MOUSE_MIDDLE_BUTTON_MASK;
    SetWhichButton(m_buttonMask);	
}

void CPhantomContextView::OnUpdateMiddleButton(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck((m_buttonMask & MOUSE_MIDDLE_BUTTON_MASK) != 0);
}

void CPhantomContextView::OnSetRightButton() 
{
    m_buttonMask ^= MOUSE_RIGHT_BUTTON_MASK;
    SetWhichButton(m_buttonMask);	
}

void CPhantomContextView::OnUpdateRightButton(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck((m_buttonMask & MOUSE_RIGHT_BUTTON_MASK) != 0);
}

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextView interface functions

void CPhantomContextView::StartProgram(BOOL bPHANToMMouseActive)
{
    // Reset the PHANToM only if the PHANToM Mouse simulation is not in effect
    int bResetPHANToM = !bPHANToMMouseActive;

    start_program(bResetPHANToM);

    // turn on push thru mode by default
    SetHapticEntryMode(CHapticView::PHANTOM_MOUSE_PUSH_THRU);
}

void CPhantomContextView::EndProgram()
{
    end_program();
}

BOOL CPhantomContextView::ProgramDone()
{
    return scene_done();
}

void CPhantomContextView::InitGraphics()
{
    init_graphics();
}

void CPhantomContextView::UpdateGraphics()
{
    update_graphics();
}

void CPhantomContextView::ResizeGraphics(int cx, int cy)
{
    reshape(cx, cy);
}

void CPhantomContextView::TermGraphics()
{
    term_graphics();
}

void CPhantomContextView::EnableServoLoop(BOOL bEnable)
{
    enable_servo_loop(bEnable);
}

LPCSTR* CPhantomContextView::QueryPHANToMNames()
{
    static LPCSTR   apszNames[] = {
                        PHANTOM_NAME,
                        NULL
                        };
    return apszNames;
}

BOOL CPhantomContextView::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    return query_phantom_pos(pX, pY, pZ);
}

