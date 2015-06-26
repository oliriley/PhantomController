// ParEdit.h: C++ derived edit control for numbers/letters etc

/////////////////////////////////////////////////////////////////////////////
// CParsedEdit is a specialized CEdit control that only allows characters
//  of a given type.
// This class is used in 3 different ways in the samples

#ifndef PARSEDEDIT_H
#define PARSEDEDIT_H

class CParsedEdit : public CEdit
{
protected:
    WORD    m_wParseStyle;      // C++ member data
    BOOL changed;
    double number;
public:
    BOOL HasFocus()
    {
        HWND hWnd = ::GetFocus();
        return (hWnd == m_hWnd || ::IsChild(m_hWnd, hWnd));
    }
    void setNumber(double value);
    double getNumber();
// Construction
    CParsedEdit();

    // explicit construction (see dertest.cpp)
    BOOL Create(DWORD dwStyle, DWORD dwParseStyle /* includes PES_ style*/,
        const RECT& rect, CWnd* pParentWnd, UINT nID);

    // subclassed construction (see subtest.cpp)
    BOOL SubclassEdit(UINT nID, CWnd* pParent, WORD wParseStyle);

    // for WNDCLASS Registered window
    static BOOL RegisterControlClass();

// Overridables
    virtual void OnBadInput();

// Implementation
protected:
    //{{AFX_MSG(CParsedEdit)
    afx_msg void OnChar(UINT, UINT, UINT); // for character validation
    afx_msg void OnVScroll(UINT, UINT, CScrollBar*); // for spin buttons
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    BOOL decimalPoint;
};

/////////////////////////////////
// Parsed edit control sub-styles

#define PES_NUMBERS         0x0001
#define PES_LETTERS         0x0002
#define PES_DECIMALPOINT    0x0003
#define PES_OTHERCHARS      0x0004
#define PES_ALL             0xFFFF

/////////////////////////////////////////////////////////////////////////////
// Extra control notifications

// above the range for normal EN_ messages
#define PEN_ILLEGALCHAR     0x8000
            // sent to parent when illegal character hit
            // return 0 if you want parsed edit to beep

#endif  // PARSEDEDIT_H
