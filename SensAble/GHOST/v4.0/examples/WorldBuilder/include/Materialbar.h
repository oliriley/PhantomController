// MaterialBar.h : header file

#include "ParEdit.h"
#include "ColorWnd.h"
#include "resource.h"
#include "FlatToolBar.h"

/////////////////////////////////////////////////////////////////////////////
// CMaterialBarBar window
#define NUMSLIDERS 4
// defines, how many input fiels get a slider (starting by input field 0)
#define NUMPROPERTIES 4
// Number of material properties that can be expressed by 1 double value (so, no color)

class CMaterialBar : public CFlatToolBar
{
// Construction
public:
    CMaterialBar();
    BOOL Init(CWnd* pParentWnd);
    BOOL SetHorizontal();
    BOOL SetVertical(); 
// Attributes
protected:
    double propertyMax[NUMSLIDERS];
    CSliderCtrl slider[NUMSLIDERS];
    CParsedEdit inputField[NUMPROPERTIES];
    CStatic staticField[NUMPROPERTIES];
    CFont smallFont;
    CColorWnd colorPalette;
    COLORREF color;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMaterialBar)
    protected:
//  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Implementation
public:
    void setPropertyMax(UINT nID, double value, BOOL adjust);
    void setProperty(UINT nID, double value);
    double getProperty(UINT nID);
    void setColor(COLORREF colorvalue);
    COLORREF getColor();
    void UpdateColorBitmap();
    void DrawColorButton(COLORREF colorvalue);
    BOOL m_bVertical;

    virtual ~CMaterialBar();
//  virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

    // Generated message map functions
protected:
    //{{AFX_MSG(CMaterialBar)
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
