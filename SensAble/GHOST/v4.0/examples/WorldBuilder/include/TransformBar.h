// TransformBar.h : header file

#include "FlatToolBar.h"
#include "Paredit.h"

/////////////////////////////////////////////////////////////////////////////
// CEditBar window

class CTransformBar : public CFlatToolBar
{
// Construction
public:
    CTransformBar();
    BOOL Init(CWnd* pParentWnd);
    BOOL SetHorizontal();
    BOOL SetVertical(); 
// Attributes
protected:
    BOOL m_bVertical;
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTransformBar)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CTransformBar();
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

    // Generated message map functions
protected:
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    //{{AFX_MSG(CTransformBar)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
