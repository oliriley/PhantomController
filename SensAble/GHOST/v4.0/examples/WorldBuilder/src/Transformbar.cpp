#include "gstAfx.h"
#include "resource.h"
#include "WorldBuilderApp.h"
#include "WorldBuilderDoc.h"
#include "WorldBuilderFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MoveDialogIndex         1
#define RotateDialogIndex       3
#define ScaleDialogIndex        5

#define InputWidth              39
#define SpaceNeeded             (InputWidth * 3) + 3

/////////////////////////////////////////////////////////////////////////////
// CTransformBar

CTransformBar::CTransformBar()
{
}

CTransformBar::~CTransformBar()
{
}

BOOL CTransformBar::Init(CWnd* pParentWnd)
{
    m_bVertical = FALSE;
    if (! CToolBar::Create(pParentWnd, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY) ||
            ! LoadToolBar(IDB_TRANSFORM))
    {
        return FALSE;
    }

    if (! SetHorizontal())
        return FALSE;
    return TRUE;
}

BOOL CTransformBar::SetHorizontal()
{
    m_bVertical = FALSE;
    return TRUE;
}

BOOL CTransformBar::SetVertical()
{
    m_bVertical = TRUE;
    return TRUE;
}

CSize CTransformBar::CalcDynamicLayout(int nLength, DWORD dwMode)
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

BEGIN_MESSAGE_MAP(CTransformBar, CFlatToolBar)
    //{{AFX_MSG_MAP(CTransformBar)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CTransformBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    CFlatToolBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
}
