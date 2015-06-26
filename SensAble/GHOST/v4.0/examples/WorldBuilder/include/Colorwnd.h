#if !defined(AFX_COLORWND_H__567D9AB1_5521_11D1_8416_0060085B2146__INCLUDED_)
#define AFX_COLORWND_H__567D9AB1_5521_11D1_8416_0060085B2146__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorWnd.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CColorWnd window

class CColorWnd : public CStatic
{
// Construction
public:
    CColorWnd();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    COLORREF HLS_To_RGB(float H, float L, float S);
    float toRGB(float q1, float q2, float hue);
//  BOOL Create(DWORD dwStyle /* includes PES_ style*/, const RECT& rect, CWnd* pParentWnd, UINT nID);
    void Init();
    virtual ~CColorWnd();
    BOOL SubclassStatic(UINT nID, CWnd* pParent);
    // Generated message map functions
protected:
    CBitmap colorGradiant;
    //{{AFX_MSG(CColorWnd)
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    COLORREF color;
    CRect clientRect;
    CSize clientSize;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORWND_H__567D9AB1_5521_11D1_8416_0060085B2146__INCLUDED_)
