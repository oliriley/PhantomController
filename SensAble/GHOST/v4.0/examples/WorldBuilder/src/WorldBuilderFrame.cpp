// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// WorldBuilderFrame.cpp
//

#include "gstAfx.h"
#include "WorldBuilderApp.h"

#include "WorldBuilderApp.h"
#include "WorldBuilderDoc.h"
#include "WorldBuilderView.h"
#include "WorldBuilderFrame.h"

#include <HapticSplash.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderFrame

// Change: derive from CHapticFrame, not CFrameWnd
IMPLEMENT_DYNCREATE(CWorldBuilderFrame, CHapticFrame)

// Change: derive from CHapticFrame, not CFrameWnd
BEGIN_MESSAGE_MAP(CWorldBuilderFrame, CHapticFrame)
    //{{AFX_MSG_MAP(CWorldBuilderFrame)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    ON_WM_CREATE()
    ON_COMMAND(ID_MAIN_PRIMITIVES, OnMainPrimitives)
    ON_COMMAND(ID_MAIN_TRANSFORM, OnMainTransform)
    ON_COMMAND(ID_MAIN_MATERIAL, OnMainMaterial)
    ON_COMMAND(ID_MATERIAL_COLOR, OnMaterialColor)
    ON_CONTROL_RANGE(EN_KILLFOCUS, ID_MATERIAL_DAMPING, ID_MATERIAL_DFRICTION, OnMaterialInput)
    ON_COMMAND(ID_MATERIAL_COLORPALETTE, OnMaterialColorPalette)
    ON_COMMAND_RANGE(ID_PRIMITIVE_CUBE, ID_PRIMITIVE_TORUS, OnPrimitives)
    ON_UPDATE_COMMAND_UI(ID_MAIN_MATERIAL, OnUpdateMainMaterial)
    ON_UPDATE_COMMAND_UI(ID_MAIN_TRANSFORM, OnUpdateMainTransform)
    ON_UPDATE_COMMAND_UI(ID_MAIN_PRIMITIVES, OnUpdateMainPrimitives)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR            // Status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderFrame construction/destruction

CWorldBuilderFrame::CWorldBuilderFrame()
{
}

CWorldBuilderFrame::~CWorldBuilderFrame()
{
}

//
// GLachape, November 10th, 1995, BUG FIX: The MFC LoadBarState and SaveBarState functions
// do not check the screen resolution.  If a CControlBar is undocked and positioned to the
// right of the screen, changing the video resolution for a lower one will make this control
// appear outside the monitor screen and it is impossible to recover it without destroying
// the registry database.
// This simple function is intended to prevent this problem.  It simply checks if the window
// is visible and undocked.  If so, it verifies that the control origin is not outside the
// screen resolution right and bottom sides.
//
void CWorldBuilderFrame::CheckControlToFitInScreen(CControlBar* pControlBar)
{
    int     ScreenResX = GetSystemMetrics(SM_CXSCREEN);
    int     ScreenResY = GetSystemMetrics(SM_CYSCREEN);
    int     nOffsetFactor = 15;

    BOOL    bVisibilityCheck = (pControlBar->GetStyle() & WS_VISIBLE) != 0;
    if (! bVisibilityCheck)
        return;     // Toolbar not visible

    if (! pControlBar->IsFloating()) {
        // GLachape, November 14th, 1995, Second bug fix.
        // A toolbar is moved when we dock another toolbar over it.
        // This moved toolbar can be outside the screen.
        CRect   ControlBarRect;
        pControlBar->GetWindowRect(&ControlBarRect);

        BOOL    bControlBarMoved = FALSE;
        DWORD   dwStyle = 0;

        if (ControlBarRect.left >= (ScreenResX - nOffsetFactor)) {
            bControlBarMoved = TRUE;
            dwStyle = CBRS_ALIGN_TOP;
            ControlBarRect.left = ScreenResX- ControlBarRect.Width();
        }
        if (ControlBarRect.top >= (ScreenResY - nOffsetFactor)) {
            bControlBarMoved = TRUE;
            dwStyle = CBRS_ALIGN_LEFT;
            ControlBarRect.top = ScreenResY- ControlBarRect.Height();
        }
        if (bControlBarMoved) {
            CPoint  ControlBarOrigin(ControlBarRect.TopLeft());
            FloatControlBar(pControlBar, ControlBarOrigin, dwStyle);
        }
        return;     // Toolbar is docked
    }

    // Not sure it is neccessary
    RecalcLayout();

    // OK, criteria met: visible and undocked
    CFrameWnd* pControlFrame = pControlBar->GetDockingFrame();
    if (pControlFrame == NULL)
        return;

    CRect   ControlRect;
    pControlFrame->GetWindowRect(ControlRect);

    BOOL bControlMoved = FALSE;

    if (ControlRect.left >= (ScreenResX - nOffsetFactor)) {
        bControlMoved = TRUE;
        ControlRect.left = ScreenResX- ControlRect.Width();
    }

    if (ControlRect.top >= (ScreenResY - nOffsetFactor)) {
        bControlMoved = TRUE;
        ControlRect.top = ScreenResY- ControlRect.Height();
    }

    // Move the control bar if needed
    if (bControlMoved)
        pControlFrame->SetWindowPos(&wndTop, ControlRect.left, ControlRect.top,
                                    0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CWorldBuilderFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
    CRect   winRect, rect;
    DWORD   dw;
    UINT    n;

    // Get MFC to adjust the dimensions of all docked toolbars
    // so that GetWindowRect will be accurate
    RecalcLayout();
    GetClientRect(&winRect);

    LeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1, 0);
    dw = LeftOf->GetBarStyle();
    n = 0;
    n = (dw&CBRS_ALIGN_TOP)              ? AFX_IDW_DOCKBAR_TOP    : n;
    n = (dw&CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
    n = (dw&CBRS_ALIGN_LEFT && n == 0)   ? AFX_IDW_DOCKBAR_LEFT   : n;
    n = (dw&CBRS_ALIGN_RIGHT && n == 0)  ? AFX_IDW_DOCKBAR_RIGHT  : n;

    // When we take the default parameters on rect, DockControlBar will dock
    // each toolbar on a separate line.  By calculating a rectangle, we in effect
    // are simulating a toolbar being dragged to that location and docked.
    DockControlBar(Bar, n, &rect);
}

void CWorldBuilderFrame::updateAllProperties()
{
    materialToolBar.setProperty(ID_MATERIAL_DAMPING, properties.damping);
    materialToolBar.setProperty(ID_MATERIAL_STIFFNESS, properties.stiffness);
    materialToolBar.setProperty(ID_MATERIAL_SFRICTION, properties.staticFriction);
    materialToolBar.setProperty(ID_MATERIAL_DFRICTION, properties.dynamicFriction);
    materialToolBar.setColor(properties.color);
}

BOOL CWorldBuilderFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Change: pass on to CHapticFrame, not CFrameWnd
    return CHapticFrame::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderFrame message handlers

int CWorldBuilderFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CHapticFrame::OnCreate(lpCreateStruct) == -1)
        return -1;

    // CG: The following line was added by the Splash Screen component.
    CSplashWnd::ShowSplashScreen(this);

    if (! myStatusBar.Create(this) ||
        ! myStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
    
    if (! mainToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
                                   CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI) ||
        ! mainToolBar.LoadToolBar(IDB_MAIN)) {
        TRACE0("Failed to create main tool bar\n");
        return -1;      // fail to create
    }

    if (! transformToolBar.Init(this)) {
        TRACE0("Failed to create transform toolbar\n");
        return -1;      // fail to create
    }

    if (! materialToolBar.Init(this)) {
        TRACE0("Failed to create material toolbar\n");
        return -1;      // fail to create
    }

    if (! primitivesToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
                                         CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI) ||
        ! primitivesToolBar.LoadToolBar(IDB_PRIMITIVES)) {
        TRACE0("Failed to create primitives toolbar\n");
        return -1;      // fail to create
    }

    EnableDocking(CBRS_ALIGN_ANY);
    mainToolBar.SetWindowText(_T("Main Panel"));
    mainToolBar.EnableDocking(CBRS_ALIGN_ANY);
    mainToolBar.SetFlatLookStyle();
    DockControlBar(&mainToolBar);

    transformToolBar.SetWindowText(_T("Transformations"));
    transformToolBar.EnableDocking(CBRS_ALIGN_ANY); 
    transformToolBar.SetFlatLookStyle();
    //DockControlBarLeftOf(&transformToolBar, &mainToolBar);
    //DockControlBarLeftOf(&mainToolBar, &transformToolBar);
    ShowControlBar(&transformToolBar, FALSE, FALSE);

    materialToolBar.SetWindowText(_T("Material Properties"));
    materialToolBar.EnableDocking(CBRS_ALIGN_ANY);  
    materialToolBar.SetFlatLookStyle();
    //DockControlBarLeftOf(&materialToolBar, &primitivesToolBar);
    ShowControlBar(&materialToolBar, FALSE, FALSE);

    primitivesToolBar.SetWindowText(_T("Primitives"));
    primitivesToolBar.EnableDocking(CBRS_ALIGN_ANY);    
    primitivesToolBar.SetFlatLookStyle();
    //DockControlBarLeftOf(&primitivesToolBar, &materialToolBar);
    ShowControlBar(&primitivesToolBar, FALSE, FALSE);

    materialToolBar.setPropertyMax(ID_MATERIAL_DAMPING, 5.0, TRUE);
    materialToolBar.setPropertyMax(ID_MATERIAL_STIFFNESS, properties.stiffness, TRUE);
    materialToolBar.setPropertyMax(ID_MATERIAL_SFRICTION, 1.0, TRUE);
    materialToolBar.setPropertyMax(ID_MATERIAL_DFRICTION, 1.0, TRUE);
    materialToolBar.setColor(properties.color);

    materialToolBar.setProperty(ID_MATERIAL_DAMPING, properties.damping);
    materialToolBar.setProperty(ID_MATERIAL_SFRICTION, properties.staticFriction);
    materialToolBar.setProperty(ID_MATERIAL_DFRICTION, properties.dynamicFriction);

    return 0;
}

void CWorldBuilderFrame::OnMainPrimitives()
{
    if ((primitivesToolBar.GetStyle() & WS_VISIBLE) != 0)
        ShowControlBar(&primitivesToolBar, FALSE, FALSE);
    else {
        DockControlBarLeftOf(&primitivesToolBar, &materialToolBar);
        ShowControlBar(&primitivesToolBar, TRUE, FALSE);
    }
}

void CWorldBuilderFrame::OnMainTransform()
{           
    if ((transformToolBar.GetStyle() & WS_VISIBLE) != 0)
        ShowControlBar(&transformToolBar, FALSE, FALSE);
    else {
        DockControlBarLeftOf(&transformToolBar, &mainToolBar);
        ShowControlBar(&transformToolBar, TRUE, FALSE);
    }
}

void CWorldBuilderFrame::OnMainMaterial()
{
    if ((materialToolBar.GetStyle() & WS_VISIBLE) != 0)
        ShowControlBar(&materialToolBar, FALSE, FALSE);
    else {
        DockControlBar(&materialToolBar);
        ShowControlBar(&materialToolBar, TRUE, FALSE);
    }
}

void CWorldBuilderFrame::OnMaterialColor()
{
    CColorDialog dlg(properties.color, CC_FULLOPEN, this);
    if (dlg.DoModal() == IDOK) {
        properties.color = dlg.GetColor();
        materialToolBar.setColor(properties.color);
    }
      // Apply the new properties immediately
    applyProperties(ID_MATERIAL_COLOR);
}


void CWorldBuilderFrame::OnMaterialInput(UINT nID)
{
    double  value = materialToolBar.getProperty(nID);

    if (nID == ID_MATERIAL_DAMPING) {
        if ((value < 0) || (value > 5)) {
            value = properties.damping;
            SetMessageText(ID_ERROR_BETWEEN0AND5);
            MessageBeep((UINT)-1);
        } else
            properties.damping = value;
    }   
    else if (nID == ID_MATERIAL_STIFFNESS) {
        if ((value < 0) || (value > 1)) {
            value = properties.stiffness;
            SetMessageText(ID_ERROR_BETWEEN0AND1);
            MessageBeep((UINT)-1);
        } else
            properties.stiffness = value;
    }
    else if (nID == ID_MATERIAL_SFRICTION) {
        if ((value < 0) || (value > 1)) {
            value = properties.staticFriction;
            SetMessageText(ID_ERROR_BETWEEN0AND1);
            MessageBeep((UINT)-1);
        } else {
            properties.staticFriction = value;
              // We won't allow the staticFriction to be set lower than 
              // dynamicFriction, it would have no physical meaning
            if (value < properties.dynamicFriction) {
                properties.dynamicFriction = value;
                materialToolBar.setProperty(ID_MATERIAL_DFRICTION, value);
                applyProperties(ID_MATERIAL_DFRICTION);
                SetMessageText(ID_ERROR_FRICTION);
            }
        }
    }
    else if (nID == ID_MATERIAL_DFRICTION) {
        if ((value < 0) || (value > 1)) {
            value = properties.dynamicFriction;
            SetMessageText(ID_ERROR_BETWEEN0AND1);
            MessageBeep((UINT)-1);
        } else {
            properties.dynamicFriction = value;
              // We won't allow the dynamicFriction to be set higher than 
              // staticFriction, it would have no physical meaning
            if (value > properties.staticFriction) {
                properties.staticFriction = value;
                materialToolBar.setProperty(ID_MATERIAL_SFRICTION, value);
                applyProperties(ID_MATERIAL_SFRICTION);
                SetMessageText(ID_ERROR_FRICTION);                
            }
        }            
    }

    materialToolBar.setProperty(nID, value);
      // Apply the new properties immediately
    applyProperties(nID);
}

void CWorldBuilderFrame::getProperties()
{
    CObjectProperties *prop = ((CWorldBuilderDoc *)GetActiveDocument())->GetProperties();
    if (prop != NULL) {
        properties.CopyFrom(prop);
        updateAllProperties();
    }
}

  // if (id == 0) then it applies all properties to the selected
  // objects, otherwise it just applies the property associated
  // with the given control id
void CWorldBuilderFrame::applyProperties(UINT id)
{
    GetNextDlgTabItem(this)->SetFocus();
    ((CWorldBuilderDoc *)GetActiveDocument())->SetProperties(&properties, id);
}

void CWorldBuilderFrame::OnMaterialColorPalette()
{
    properties.color = materialToolBar.getColor();
      // Apply the new properties immediately
    applyProperties(ID_MATERIAL_COLORPALETTE);
}

void CWorldBuilderFrame::OnPrimitives(UINT nID)
{
    SendMessage(WM_COMMAND, nID - ID_PRIMITIVE_CUBE + ID_NEW_CUBE);
      // We call SetProperties() to copy all the current 
      // properties to the new object
    ((CWorldBuilderDoc *)GetActiveDocument())->SetProperties(&properties);
}

void CWorldBuilderFrame::OnUpdateMainMaterial(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((materialToolBar.GetStyle() & WS_VISIBLE) != 0);
}

void CWorldBuilderFrame::OnUpdateMainTransform(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((transformToolBar.GetStyle()& WS_VISIBLE) != 0);   
}

void CWorldBuilderFrame::OnUpdateMainPrimitives(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((primitivesToolBar.GetStyle()& WS_VISIBLE) != 0);  
}

void CWorldBuilderFrame::updateToolbars()
{
    mainToolBar.Update(this);
    transformToolBar.Update(this);
    materialToolBar.Update(this);
    primitivesToolBar.Update(this);
}

void CWorldBuilderFrame::toggleToolbars()
{      
    OnMainTransform();
    OnMainMaterial();
    OnMainPrimitives();
}
