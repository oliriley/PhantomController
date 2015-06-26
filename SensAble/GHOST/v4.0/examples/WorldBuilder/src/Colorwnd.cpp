// ColorWnd.cpp : implementation file
//

#include "gstAfx.h"
#include "resource.h"
#include "WorldBuilderApp.h"
#include "ColorWnd.h"
#include "MaterialBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorWnd

CColorWnd::CColorWnd()
{
}

CColorWnd::~CColorWnd()
{
}


BEGIN_MESSAGE_MAP(CColorWnd, CStatic)
    //{{AFX_MSG_MAP(CColorWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorWnd message handlers
float CColorWnd::toRGB(float q1, float q2, float hue)
{
    if (hue > 360)
        hue = hue - 360;
    if (hue < 0)
        hue = hue + 360;
    if (hue < 60)
        return q1 + (q2 - q1) * hue / 60;
    else if (hue < 180)
        return q2;
    else if (hue < 240)
        return q1 + (q2 - q1) * (240 - hue) / 60;
    else
        return q1;
}

COLORREF CColorWnd::HLS_To_RGB(float H, float L, float S)
{
    float p1, p2, R, G, B;

    if (L <= 0.5)
        p2 = L * (1 + S);
    else
        p2 = L + S - (L * S);
    p1 = 2 * L - p2;
    if (S == 0) {
        R = L;
        G = L;
        B = L;
    } else {
        R = toRGB(p1, p2, H + 120);
        G = toRGB(p1, p2, H);
        B = toRGB(p1, p2, H - 120);
    }
    return RGB(R * 255, G * 255, B * 255);
}


void CColorWnd::Init()
{
    int x, y;
    float H, L;
    CDC dcMem;
    CPaintDC dc(this);
    CBrush brush;
    CRect rect;
    if (colorGradiant.m_hObject != NULL)
        colorGradiant.DeleteObject();        // get rid of old bitmap

    dcMem.CreateCompatibleDC(&dc);
    GetClientRect(clientRect);
    clientSize = clientRect.Size();
    colorGradiant.CreateCompatibleBitmap(&dc, clientSize.cx, clientSize.cy);
    ASSERT(colorGradiant.m_hObject != NULL);

    CBitmap* pOldBitmap = dcMem.SelectObject(&colorGradiant);

    for (y = 0; y < clientSize.cy; y++)
        for (x = 0; x < clientSize.cx; x++) {
            H = ((float)x) * 360 / (clientSize.cx - 1);
            L = ((float)(clientSize.cy - 1) - y) / (clientSize.cy - 1);
            rect.SetRect(x, y, x + 1, y + 1);
            brush.CreateSolidBrush(HLS_To_RGB(H, L, 1));
            dcMem.FillRect(&rect, &brush);
            brush.DeleteObject();
        }
    dcMem.SelectObject(pOldBitmap);
    dcMem.DeleteDC();
}

void CColorWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CBitmap* pbmOld = NULL;
    CDC dcMem;

    if (colorGradiant.m_hObject == NULL)
        return;     // no bitmap for the ball

    dcMem.CreateCompatibleDC(&dc);
    pbmOld = dcMem.SelectObject(&colorGradiant);
    dc.BitBlt(0, 0, clientSize.cx, clientSize.cy, &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pbmOld);
    dcMem.DeleteDC();
    // Do not call CStatic::OnPaint() for painting messages
}

BOOL CColorWnd::SubclassStatic(UINT nID, CWnd * pParent)
{
    return SubclassDlgItem(nID, pParent);
}

void CColorWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    float H, L;
    if (clientRect.PtInRect(point)) {
        SetCapture();
        H = ((float)point.x) * 360 / (clientSize.cx - 1);
        L = ((float)(clientSize.cy - 1) - point.y) / (clientSize.cy - 1);
        color = HLS_To_RGB(H, L, 1);
        ((CMaterialBar*)GetOwner())->DrawColorButton(color);
    }
    // CStatic::OnLButtonDown(nFlags, point);
}

void CColorWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    if (GetCapture() != this)
        return;

    float H, L;
    if (clientRect.PtInRect(point)) {
        H = ((float)point.x) * 360 / (clientSize.cx - 1);
        L = ((float)(clientSize.cy - 1) - point.y) / (clientSize.cy - 1);
        color = HLS_To_RGB(H, L, 1);
        ((CMaterialBar*)GetOwner())->DrawColorButton(color);
    }
    // CStatic::OnMouseMove(nFlags, point);
}

void CColorWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (GetCapture() == this)
        ReleaseCapture();
    ((CMaterialBar*)GetOwner())->setColor(color);
    GetOwner()->SendMessage(WM_COMMAND, GetDlgCtrlID());    
    // CStatic::OnLButtonUp(nFlags, point);
}
