// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#include "stdafx.h"
#include "HapticApp.h"
#include "HapticMouse.h"

#include <winioctl.h>
#include "PHANToMMouseIoCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4244) // conversion from 'double' to 'float', possible loss of data

/////////////////////////////////////////////////////////////////////////////
// CHapticMouse

IMPLEMENT_DYNCREATE(CHapticMouse, CHapticMouseBase)

BEGIN_MESSAGE_MAP(CHapticMouse, CHapticMouseBase)
    //{{AFX_MSG_MAP(CHapticMouse)
    ON_WM_KEYUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_MESSAGE(WM_PHANTOMMOUSEPUSHTHRU, OnPHANToMMousePushThru)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticMouse construction/destruction

CHapticMouse::CHapticMouse()
{
    m_bAllowFallThru = FALSE;
}

CHapticMouse::~CHapticMouse()
{
}

void CHapticMouse::ForceDisable()
{
    // Drop out of haptics mode
    LeaveHaptics();

    // Show the cursor
    ShowMouseCursor(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Interface function stubs

void CHapticMouse::EnableServoLoop(BOOL bEnable)
{
    // Client must implement
    ASSERT(FALSE);
}

BOOL CHapticMouse::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    // Client must implement
    ASSERT(FALSE);
    return FALSE;
}
    
/////////////////////////////////////////////////////////////////////////////
// Message handlers

void CHapticMouse::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CView::OnKeyUp(nChar, nRepCnt, nFlags);

    switch (nChar) {
    case VK_RETURN:
        if (!m_bInitialized)
            m_bStartProgram = TRUE;
        break;

    case VK_ESCAPE:
        OnPhantomLeave();
        break;
    }
}

void CHapticMouse::OnMouseMove(UINT nFlags, CPoint point)
{
    CView::OnMouseMove(nFlags, point);

    // If the mouse was not in view previously, then this is mouse entry
    if (!m_bMouseInView && !m_bHapticsOn) {
        // enable the push-thru event when the mouse enters the view
        if (m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU)
            EnablePushThruMode();

        // Disable forces in the PHANToM Mouse driver and enable the
        // servoloop in the application if fall thru is enabled
        if (m_bAllowFallThru && m_hapticEntryMode == PHANTOM_MOUSE_FALL_THRU)
            OnPhantomEntry();

        // Only allow fall thru when the mouse has re-entered the view
        m_bAllowFallThru = FALSE;
    }
    
    m_bMouseInView = TRUE;

    // Sign up for notification when the mouse leaves the window
    RequestLeaveNotify();
}

void CHapticMouse::OnMouseLeave(UINT wParam, ULONG dwParam)
{
    m_bMouseInView = FALSE;

    // Disable the push-thru event while the mouse is not in view
    if (!m_bHapticsOn && m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU)
        EnablePushThruMode(FALSE);

    // Allow the mouse to fall thru when it re-enters the view
    if (IsMouseDriverEnabled())
        m_bAllowFallThru = TRUE;
}

void CHapticMouse::OnLButtonDown(UINT nFlags, CPoint point)
{
    CView::OnLButtonDown(nFlags, point);

    m_bMouseInView = TRUE;
}

void CHapticMouse::OnLButtonUp(UINT nFlags, CPoint point)
{
    CView::OnLButtonUp(nFlags, point);

    m_bMouseInView = TRUE;

    if (!m_bInitialized)
        m_bStartProgram = TRUE;

    if (!m_bHapticsOn && m_hapticEntryMode == PHANTOM_MOUSE_CLICK_THRU)
        OnPhantomEntry();

    // Hide the mouse pointer
    ShowMouseCursor(FALSE);
}

void CHapticMouse::OnMButtonDown(UINT nFlags, CPoint point)
{
    CView::OnMButtonDown(nFlags, point);

    m_bMouseInView = TRUE;
}


void CHapticMouse::OnMButtonUp(UINT nFlags, CPoint point)
{
    CView::OnMButtonUp(nFlags, point);

    m_bMouseInView = TRUE;
}

void CHapticMouse::OnRButtonDown(UINT nFlags, CPoint point)
{
    CView::OnRButtonDown(nFlags, point);

    m_bMouseInView = TRUE;
}


void CHapticMouse::OnRButtonUp(UINT nFlags, CPoint point)
{
    CView::OnRButtonUp(nFlags, point);

    m_bMouseInView = TRUE;
}

void CHapticMouse::OnPHANToMMousePushThru(UINT wParam, ULONG dwParam)
{
    if (!m_bHapticsOn && m_bMouseInView && m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU) {
        OnPhantomEntry();
        EnablePushThruMode(FALSE);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Worker functions

BOOL CHapticMouse::RequestLeaveNotify()
{
    TRACKMOUSEEVENT tmev;
    tmev.cbSize = sizeof tmev;
    tmev.dwFlags = TME_LEAVE;
    tmev.hwndTrack = GetSafeHwnd();
    tmev.dwHoverTime = HOVER_DEFAULT;

    return _TrackMouseEvent(&tmev);
}

BOOL CHapticMouse::CancelLeaveNotify()
{
    TRACKMOUSEEVENT tmev;
    tmev.cbSize = sizeof tmev;
    tmev.dwFlags = TME_CANCEL | TME_LEAVE;
    tmev.hwndTrack = GetSafeHwnd();
    tmev.dwHoverTime = HOVER_DEFAULT;

    return _TrackMouseEvent(&tmev);
}

void CHapticMouse::ShowMouseCursor(BOOL bShowCursor)
{
    // Set or hide the mouse pointer
    if (IsTopParentActive())
        if (bShowCursor)
            ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        else
            ::SetCursor(NULL);
}

void CHapticMouse::EnterHaptics()
{
    if (!m_bHapticsOn && !m_bPHANToMMouseError) {
        m_bHapticsOn = TRUE;

		// Disable forces in the PHANToM Mouse driver
        if (IsMouseDriverEnabled())
            EnableMouseDriver(FALSE);

        // Enable the servoloop in the application
        EnableServoLoop(TRUE);

        // Hide the mouse cursor
        ShowMouseCursor(FALSE);
    }
}

void CHapticMouse::LeaveHaptics()
{
    if (m_bHapticsOn && !m_bPHANToMMouseError) {
        m_bHapticsOn = FALSE;

        // Force one last draw while the servoloop is active
        Invalidate();
        OnPaint();

        // Disable the servo loop in the application
        EnableServoLoop(FALSE);

        // Enable forces in the PHANToM Mouse driver
        // Make sure the latest configuration settings are passed through
        if (IsPHANToMMouseEnabled()) {
            UpdateMouseConfig();
            EnableMouseDriver(TRUE);            
        }

        // Perform one last redraw to flush the graphics queues
        Invalidate();
        UpdateWindow();

        // always show the cursor when in mouse mode
        ShowMouseCursor(TRUE);
    }

}

void CHapticMouse::OnPhantomEntry()
{
    if (m_bInitialized)
        EnterHaptics();

    // bring this application to the foreground and give it the focus
    if (!IsTopParentActive()) {
        SetForegroundWindow();
        SetActiveWindow();
    }
}

void CHapticMouse::OnPhantomLeave()
{
    if (m_bInitialized && IsPHANToMMouseEnabled()) {
	    // Ask the application where its 3D cursor is, so the mouse pointer
	    // can be shown at the corresponding 2D position on the screen
	    Sync3DTo2DCursor();
	
	    // Leave the application PHANToM context
	    LeaveHaptics();

        if (m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU)
            EnablePushThruMode(TRUE);
    }
}

void CHapticMouse::Sync3DTo2DCursor()
{
    double X, Y, Z;
    CRect viewSize;
    CPoint point;

    GetClientRect(&viewSize);

    if (QueryCursorPos(&X, &Y, &Z)) {
        GLdouble matModel[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, matModel);
        GLdouble matProj[16];
        glGetDoublev(GL_PROJECTION_MATRIX, matProj);
        GLint viewport[16];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLdouble x, y, z;
        gluProject(X, Y, Z, matModel, matProj, viewport, &x, &y, &z);
        
        point.x = min(max(0, (int) x), viewSize.Width());
        point.y = min(max(0, viewSize.Height() - (int) y), viewSize.Height());
    } else {
        // Application does not have a current 3D cursor, center the mouse
        // pointer in the window
        point.x = viewSize.Width() / 2;
        point.y = viewSize.Height() / 2;
    }
    
    // Place the mouse cursor at the computed position
    ClientToScreen(&point);
    SetCursorPos(point.x, point.y);
}

