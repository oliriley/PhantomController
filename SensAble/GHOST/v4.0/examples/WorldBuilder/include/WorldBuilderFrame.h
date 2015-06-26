// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// WorldBuilderFrame.h : derived from CHapticFrame
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDBUILDERFRAME_H__C950C8F8_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_WORLDBUILDERFRAME_H__C950C8F8_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change: Include HapticFrame.h
#include "HapticFrame.h"

#include "TransformBar.h"
#include "MaterialBar.h"
#include "ColorWnd.h"
#include "ObjectState.h"

// Change: derive from CHapticFrame, not CFrameWnd
class CWorldBuilderFrame : public CHapticFrame
{
protected: // create from serialization only
    CWorldBuilderFrame();
    DECLARE_DYNCREATE(CWorldBuilderFrame)

// Attributes
public:
    void CheckControlToFitInScreen(CControlBar* pControlBar);
    void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
    void updateAllProperties();
    void getProperties();
      // if (id == 0) then it applies all properties to the selected
      // objects, otherwise it just applies the property associated
      // with the given control id
    void applyProperties(UINT id = 0);

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWorldBuilderFrame)
    public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CWorldBuilderFrame();
    void updateToolbars();
    void toggleToolbars();

private:  // control bar embedded members
    CFlatToolBar        mainToolBar;
    CTransformBar       transformToolBar;
    CMaterialBar        materialToolBar;
    CFlatToolBar        primitivesToolBar;
    CStatusBar          myStatusBar;
    CObjectProperties   properties;

// Generated message map functions
protected:
    //{{AFX_MSG(CWorldBuilderFrame)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnMainPrimitives();
    afx_msg void OnMainTransform();
    afx_msg void OnMainMaterial();
    afx_msg void OnMaterialColor();
    afx_msg void OnMaterialInput(UINT nID);
    afx_msg void OnMaterialColorPalette();
    afx_msg void OnPrimitives(UINT nID);
    afx_msg void OnUpdateMainMaterial(CCmdUI* pCmdUI);
    afx_msg void OnUpdateMainTransform(CCmdUI* pCmdUI);
    afx_msg void OnUpdateMainPrimitives(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDBUILDERFRAME_H__C950C8F8_B9EC_11D1_B8C5_000000000000__INCLUDED_)
