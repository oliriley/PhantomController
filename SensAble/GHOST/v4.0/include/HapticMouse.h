// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#ifndef __HAPTIC_MOUSE_H__
#define __HAPTIC_MOUSE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HapticMouseBase.h"

class CHapticMouse : public CHapticMouseBase
{
protected: // create from serialization only
 
    CHapticMouse();
    DECLARE_DYNCREATE(CHapticMouse)

// Operations
public:

    void ForceDisable();
    virtual void OnPhantomEntry();
    virtual void OnPhantomLeave();

// Implementation
public:

    virtual ~CHapticMouse();

protected:

    // Client interface functions
    virtual void    EnableServoLoop(BOOL bEnable);
    virtual BOOL    QueryCursorPos(double* pX, double* pY, double* pZ);

protected:
    // Worker functions
    BOOL RequestLeaveNotify();
    BOOL CancelLeaveNotify();
    void ShowMouseCursor(BOOL bShowCursor = TRUE);
    void EnterHaptics();
    void LeaveHaptics();
    void Sync3DTo2DCursor();

// Generated message map functions
protected:
    //{{AFX_MSG(CHapticMouse)
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave(UINT wParam, ULONG dwParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
    afx_msg void OnPHANToMMousePushThru(UINT wParam, ULONG dwParam);

    UINT        m_timerID;            // the ID returned by SetTimer

    BOOL        m_bAllowFallThru;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __HAPTIC_MOUSE_H__
