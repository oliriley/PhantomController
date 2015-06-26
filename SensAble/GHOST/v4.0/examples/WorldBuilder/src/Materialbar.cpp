#include "gstAfx.h"
#include "resource.h"
#include "MaterialBar.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define InputWidth              39
#define SliderWidth             40
#define StaticWidth             45
#define ControlWidth            (InputWidth + SliderWidth + StaticWidth)

#define ColorPaletteIndex       0
#define ColorPaletteWidth       120
#define ColorButtonIndex        1
#define ColorButtonBitmap       0       // Index of bitmap for color button
#define DampingSliderIndex      2
#define FrictionSliderIndex     4

/////////////////////////////////////////////////////////////////////////////
// CMaterialBar

CMaterialBar::CMaterialBar()
{
}

CMaterialBar::~CMaterialBar()
{
}

BOOL CMaterialBar::Init(CWnd* pParentWnd)
{
    CRect rect[2], rect2;
    CString staticText;
    CSize fontSize, size;
    CFont *oldFont;
    HFONT hFont;

    m_bVertical = FALSE;

    DWORD style = WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY;

    if (! Create(pParentWnd, style) || !LoadToolBar(IDB_MATERIAL))
    {
        return FALSE;
    }

    SetButtonInfo(ColorPaletteIndex, ID_MATERIAL_COLOR, TBBS_SEPARATOR, ColorPaletteWidth);
    SetButtonInfo(DampingSliderIndex, ID_MATERIAL_DAMPING, TBBS_SEPARATOR, ControlWidth);
    SetButtonInfo(FrictionSliderIndex, ID_STATIC_SFRICTION, TBBS_SEPARATOR, ControlWidth);
    
    GetItemRect(ColorPaletteIndex, &rect[0]);
    colorPalette.Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_SUNKEN|SS_WHITEFRAME,
                        rect[0], this, ID_MATERIAL_COLORPALETTE);
    colorPalette.Init();

    hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    if (hFont == NULL)
        hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
    CClientDC dc(this);
    oldFont = dc.SelectObject(CFont::FromHandle(hFont));
    fontSize = dc.GetTextExtent(_T("jF"));
    dc.SelectObject(oldFont);

    CClientDC dcScreen(GetDesktopWindow());
    long logical_pixels = dcScreen.GetDeviceCaps(LOGPIXELSY);
    smallFont.CreatePointFont(((rect[0].bottom - rect[0].top) * 72 /
                               (logical_pixels * 3) - 1) * 10, "DEFAULT", NULL);

    GetItemRect(DampingSliderIndex, &rect[0]);
    GetItemRect(FrictionSliderIndex, &rect[1]);

    int idCounter = 0;
    for (int i = 0; i < 2; i++) {
        size = rect[i].Size();
        rect2 = rect[i];
        size.cy = size.cy / 2;
        for (int j = 0; j < 2; j++) {
            rect2.bottom = rect2.top + size.cy;
            rect2.right = rect2.left + SliderWidth;
            slider[idCounter].Create(WS_CHILD| WS_VISIBLE|
                TBS_HORZ|TBS_NOTICKS, rect2, this, ID_MATERIAL_DAMPING + idCounter);
            slider[idCounter].SetRange(0, 100);
            rect2.left = rect2.right;
            rect2.right = rect2.left + StaticWidth;

            staticText.LoadString(ID_STATIC_DAMPING + idCounter);
            if (! staticField[idCounter].Create(staticText, WS_VISIBLE|WS_CHILD|
                SS_NOTIFY|SS_CENTER, rect2, this, ID_STATIC_DAMPING + idCounter))
            {
                return FALSE;
            }
            staticField[idCounter].SendMessage(WM_SETFONT, (WPARAM)HFONT(smallFont));
            rect2.left = rect2.right;
            rect2.right = rect2.left + InputWidth;
            rect2.bottom = rect2.top + fontSize.cy;
            if (! inputField[idCounter].Create(WS_VISIBLE|WS_CHILD|WS_TABSTOP|ES_LEFT|
                ES_AUTOHSCROLL, PES_NUMBERS, rect2, this, ID_MATERIAL_DAMPING + idCounter))
            {
                return FALSE;
            }
            inputField[idCounter].SendMessage(WM_SETFONT, (WPARAM)hFont);
            rect2.left = rect[i].left; // put the input fields side by side
            rect2.right = rect[i].right;
            rect2.top = rect2.top + size.cy + 1;
            idCounter++;
        }
    }

    /*
    GetItemRect(MassInputFieldIndex, &rect2);
    rect2.right = rect2.left + InputWidth;
    rect2.bottom = rect2.top + fontSize.cy;
    if (! inputField[idCounter].Create(WS_VISIBLE|WS_CHILD|WS_TABSTOP|ES_LEFT|
        ES_AUTOHSCROLL, PES_NUMBERS, rect2, this, ID_MATERIAL_MASS))
    {
        return FALSE;
    }
    inputField[idCounter].SendMessage(WM_SETFONT, (WPARAM)hFont);
    rect2.top = rect2.bottom;
    rect2.bottom = rect2.top + fontSize.cy;
    staticText.LoadString(ID_STATIC_MASS);
    if (! staticField[idCounter].Create(staticText, WS_VISIBLE|WS_CHILD|
        SS_NOTIFY|SS_CENTER, rect2, this, ID_STATIC_MASS))
    {
        return FALSE;
    }
    staticField[idCounter].SendMessage(WM_SETFONT, (WPARAM)HFONT(smallFont));
    */

    if (! SetHorizontal())
        return FALSE;

    return TRUE;
}

BOOL CMaterialBar::SetHorizontal()
{
    CRect rect, rect2;

    m_bVertical = FALSE;

    SetButtonInfo(ColorPaletteIndex, ID_MATERIAL_COLOR, TBBS_SEPARATOR, ColorPaletteWidth);
    SetButtonInfo(DampingSliderIndex, ID_STATIC_DAMPING, TBBS_SEPARATOR, ControlWidth);
    SetButtonInfo(FrictionSliderIndex, ID_STATIC_SFRICTION, TBBS_SEPARATOR, ControlWidth);
    return TRUE;
}

BOOL CMaterialBar::SetVertical()
{
    m_bVertical = TRUE;

    SetButtonInfo(ColorPaletteIndex, 0, TBBS_SEPARATOR, 0);
    SetButtonInfo(DampingSliderIndex, 0, TBBS_SEPARATOR, 0);
    SetButtonInfo(FrictionSliderIndex, 0, TBBS_SEPARATOR, 0);
    return TRUE;
}

CSize CMaterialBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    // if we're committing set the buttons appropriately
    if (dwMode & LM_COMMIT) {
        if (dwMode & LM_VERTDOCK) {
            if (! m_bVertical)
                SetVertical();
        } else
            if (m_bVertical)
                SetHorizontal();

        return CToolBar::CalcDynamicLayout(nLength, dwMode);
    } else {
        BOOL bOld = m_bVertical;
        BOOL bSwitch = (dwMode & LM_HORZ) ? bOld : ! bOld;

        if (bSwitch)
            if (bOld)
                SetHorizontal();
            else
                SetVertical();

        CSize sizeResult = CToolBar::CalcDynamicLayout(nLength, dwMode);
        
        if (bSwitch)
            if (bOld)
                SetHorizontal();
            else
                SetVertical();

        return sizeResult;
    }
}

void CMaterialBar::DrawColorButton(COLORREF colorvalue)
{
    CClientDC dc(this);
    CBrush brush;
    CRect rect;

    brush.CreateSolidBrush(colorvalue);
    GetItemRect(ColorButtonIndex, &rect);
    dc.FillRect(&rect, &brush);
    brush.DeleteObject();
}

void CMaterialBar::UpdateColorBitmap()
{
        TBREPLACEBITMAP replaceBitmap;
    replaceBitmap.hInstOld = NULL;
    replaceBitmap.nIDOld = (UINT)m_hbmImageWell;
    replaceBitmap.hInstNew = NULL;
    replaceBitmap.nIDNew = (UINT)m_hbmImageWell;
    replaceBitmap.nButtons = 3;
    BOOL bResult = (BOOL)DefWindowProc(TB_REPLACEBITMAP, 0, (LPARAM)&replaceBitmap);
    ASSERT(bResult != 0);
    
// or:   OnSysColorChange();
    CClientDC dc(this);
    CDC dcMem;
    CBrush brush;
    CRect rect;
        
    dcMem.CreateCompatibleDC(&dc);
    CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hbmImageWell));
    
    brush.CreateSolidBrush(color);
    rect.SetRect(ColorButtonBitmap * m_sizeImage.cx, 0,
                 (ColorButtonBitmap + 1) * m_sizeImage.cx, m_sizeImage.cy);
    dcMem.FillRect(&rect, &brush);
    brush.DeleteObject();    
    dcMem.SelectObject(pOldBitmap);
    dcMem.DeleteDC();        
    
    GetItemRect(ColorButtonIndex, &rect);
    InvalidateRect(&rect);
}

COLORREF CMaterialBar::getColor()
{
    return color;
}

void CMaterialBar::setColor(COLORREF colorvalue)
{
    color = colorvalue;
    UpdateColorBitmap();
}

double CMaterialBar::getProperty(UINT nID)
{
    int n = nID - ID_MATERIAL_DAMPING;
    ASSERT(n >=0 && n < NUMPROPERTIES);
    return inputField[n].getNumber();
}

void CMaterialBar::setProperty(UINT nID, double value)
{
    int n = nID - ID_MATERIAL_DAMPING;
    ASSERT(n >= 0 && n < NUMPROPERTIES);
    inputField[n].setNumber(value);
    if (n < NUMSLIDERS)
        slider[n].SetPos((int)floor(100.0 * value / propertyMax[n]));
}

void CMaterialBar::setPropertyMax(UINT nID, double value, BOOL adjust)
{
    int n = nID - ID_MATERIAL_DAMPING;
    ASSERT(n >= 0 && n < NUMSLIDERS);
    propertyMax[n] = value;
    if (adjust)
        setProperty(nID, value);
}

BEGIN_MESSAGE_MAP(CMaterialBar, CFlatToolBar)
    //{{AFX_MSG_MAP(CMaterialBar)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMaterialBar message handlers

void CMaterialBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Message from the slider
    if (pScrollBar != NULL) {
        int nID = pScrollBar->GetDlgCtrlID();
        int n = nID - ID_MATERIAL_DAMPING;
        ASSERT(n >= 0 && n < NUMSLIDERS);
        int pos = ((CSliderCtrl*)pScrollBar)->GetPos();
        double value = pos * propertyMax[n] / 100;
        inputField[n].setNumber(value);
        if (nSBCode == SB_ENDSCROLL)
            SendMessage(WM_COMMAND, MAKELONG(nID, EN_KILLFOCUS), (LPARAM)m_hWnd);
    }
    CFlatToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}
