// ParEdit.cpp: C++ derived edit control for numbers/letters etc
#include "gstAfx.h"
#include "ParEdit.h"

/////////////////////////////////////////////////////////////////////////////
// ParsedEdit

CParsedEdit::CParsedEdit()
{
    m_wParseStyle = 0;
    decimalPoint = FALSE;
}

BEGIN_MESSAGE_MAP(CParsedEdit, CEdit)
    //{{AFX_MSG_MAP(CParsedEdit)
    ON_WM_CHAR()
    ON_WM_VSCROLL()     // for associated spin controls
    ON_WM_KEYUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Creating from C++ code

BOOL CParsedEdit::Create(DWORD dwStyle, DWORD dwParseStyle, const RECT& rect,
        CWnd* pParentWnd, UINT nID)
{
    m_wParseStyle = LOWORD(dwParseStyle);
    // figure out edit control style
    // DWORD dwEditStyle = MAKELONG(ES_LEFT, HIWORD(dwStyle));
    return CWnd::Create(_T("EDIT"), NULL, dwStyle, rect, pParentWnd, nID);
    // return CWnd::Create(_T("EDIT"), NULL, dwEditStyle, rect, pParentWnd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// Aliasing on top of an existing Edit control

BOOL CParsedEdit::SubclassEdit(UINT nID, CWnd* pParent, WORD wParseStyle)
{
    m_wParseStyle = wParseStyle;
    return SubclassDlgItem(nID, pParent);
}

/////////////////////////////////////////////////////////////////////////////
// Input character filter

void CParsedEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    WORD type;
    if (nChar < 0x20)
        type = PES_ALL;                         // always allow control chars
    else if (IsCharAlphaNumeric((TCHAR)nChar) && ! IsCharAlpha((TCHAR)nChar))
        type = PES_NUMBERS;
    else if ((m_wParseStyle == PES_NUMBERS) &&
        ((nChar == '.') || (nChar == '-')) /*&& (! decimalPoint)*/) {
        type = PES_DECIMALPOINT;
        decimalPoint = TRUE;
    }
    else if (IsCharAlpha((TCHAR)nChar))
        type = PES_LETTERS;
    else
        type = PES_OTHERCHARS;

    if (m_wParseStyle & type) {
        SetModify(TRUE);
        CEdit::OnChar(nChar, nRepCnt, nFlags);  // permitted
    } else
        // illegal character - inform parent
        OnBadInput();
}

/////////////////////////////////////////////////////////////////////////////
// Spin controls will send scroll messages

void CParsedEdit::OnVScroll(UINT nSBCode, UINT, CScrollBar*)
{
    int nDelta = 0;
    if (nSBCode == SB_LINEDOWN)
        nDelta = -1;
    else if (nSBCode == SB_LINEUP)
        nDelta = +1;
    else
        return; // nothing special

    // set the focus to this edit item and select it all
    SetFocus();

    // Get the number in the control.
    BOOL bOk;
    int nOld = GetParent()->GetDlgItemInt(GetDlgCtrlID(), &bOk);
    if (bOk) {
        // The MuScrl32 control also supports range checking
        // for this example, we just prevent overflow
        int nNew = nOld + nDelta;
        if (nNew >= 0 && nNew <= 32767)
            GetParent()->SetDlgItemInt(GetDlgCtrlID(), nNew);
        else
            bOk = FALSE;
    }

    if (! bOk)
        OnBadInput();

    SetSel(0, -1);
}

/////////////////////////////////////////////////////////////////////////////
// default bad input handler, beep (unless parent notification
//    returns -1.  Most parent dialogs will return 0 or 1 for command
//    handlers (i.e. Beep is the default)

void CParsedEdit::OnBadInput()
{
    if (GetParent()->SendMessage(WM_COMMAND,
        MAKELONG(GetDlgCtrlID(), PEN_ILLEGALCHAR), (LPARAM)m_hWnd) != -1)
    {
        MessageBeep((UINT)-1);
    }
}

/////////////////////////////////////////////////////////////////////////////

double CParsedEdit::getNumber()
{
    if (GetModify()) {
        CString text;
        SetModify(FALSE);
        GetWindowText(text);
        number = atof(text);
    }
    return number;
}

void CParsedEdit::setNumber(double value)
{
    if (number != value) {
        CString text;
        number = value;
        text.Format("%g", value);
        SetWindowText(text);
    }
}

void CParsedEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_RETURN)
        GetParentOwner()->GetNextDlgTabItem(this)->SetFocus();
    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
