/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  HapticViewerView.h

Description: 

  MFC CView derived view class for haptic viewer.

******************************************************************************/

#if !defined(AFX_HAPTICVIEWERVIEW_H__AF5EEA0D_A057_11D4_A8D5_000086513E90__INCLUDED_)
#define AFX_HAPTICVIEWERVIEWVIEW_H__AF5EEA0D_A057_11D4_A8D5_000086513E90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HD/hd.h>
#include <HL/hl.h>

class Mesh;

class CHapticViewerView : public CView
{
protected: // create from serialization only
    CHapticViewerView();
    DECLARE_DYNCREATE(CHapticViewerView)

    virtual ~CHapticViewerView();

// Attributes
public:
    CHapticViewerDoc* GetDocument()
    { 
        return (CHapticViewerDoc*)m_pDocument; 
    }

    HDC     m_hDC;         // graphic device context handle
    HGLRC   m_hRC;         // graphic rendering context handle

    HHD    m_hHD;          // haptic device context handle
    HHLRC  m_hHLRC;        // haptic rendering context handle

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticViewerView)
public:
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void OnPerformance();
    virtual void OnWireframe();
    virtual void OnHapticCameraVisual();

    virtual void OnShapeDepthBuffer();
    virtual void OnShapeFeedbackBuffer();

    virtual void OnHapticCameraView();
    virtual void OnHapticAdaptiveViewport();
    //}}AFX_VIRTUAL


// Implementation
public:

private:
    // initialization routines
    void initGL();              // intialize graphic (GL)
    void initHL();              // initial haptics

    // draw routines
    int drawSceneGL(Mesh* pObj, bool isHapticView);
    int drawSceneHL(Mesh* pObj);
    void drawCursorGL();

    void drawPerformance(double hlElapsed, 
                         double glElapsed, 
                         double totalElapsed,
                         int hlTriCount, int glTriCount);
    void drawTextGL(int x, int y, char* text);
    void drawFrustum(hduVector3Dd* frustum);

    void updateGLCamera();
    void updateGLView(Mesh* pMesh);
    void updateWorkspace();

    void initCameraTransform();

    BOOL SetupPixelFormat();

private:
    // window dimensions
    int m_windowHeight;
    int m_windowWidth;

    // view frustum data
    hduVector3Dd m_hapticCameraFrustum[8];  // haptic camera frustum
    hduVector3Dd m_graphicFrustum[8];       // view frustum

    // cursor
    double m_cursorScale;
    unsigned int m_cursorDisplayList;

    // font display list
    GLuint m_fontListBase;

    bool m_bPerformance;
    bool m_bWireframe;
    bool m_bHapticCameraVisual;

    bool m_bShapeDepthBuffer;
    bool m_bShapeFeedbackBuffer;

    bool m_bHapticCameraView;
    bool m_bHapticAdaptiveViewport;

    bool m_bSinglePassRender;

    hduVector3Dd m_cameraPosWC;

    hduMatrix m_cameraRotation;
    double m_cameraScale;
    double m_cameraTranslationX;
    double m_cameraTranslationY;
    bool m_isRotatingCamera;
    bool m_isScalingCamera;
    bool m_isTranslatingCamera;

    int m_lastMouseX;
    int m_lastMouseY;

protected:
    hduVector3Dd getProxyPositionInModelCoord(Mesh* pObj);
    void getViewVolumeInModelCoord(hduVector3Dd* frustum);

// Generated message map functions
protected:
    //{{AFX_MSG(CHapticViewerView)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnUpdatePerformance(CCmdUI* pCmdUI);
    afx_msg void OnUpdateWireframe(CCmdUI* pCmdUI);
    afx_msg void OnUpdateHapticCameraVisual(CCmdUI* pCmdUI);
    afx_msg void OnUpdateShapeDepthBuffer(CCmdUI* pCmdUI);
    afx_msg void OnUpdateShapeFeedbackBuffer(CCmdUI* pCmdUI);
    afx_msg void OnUpdateHapticCameraView(CCmdUI* pCmdUI);
    afx_msg void OnUpdateHapticAdaptiveViewport(CCmdUI* pCmdUI);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HAPTICVIEWERVIEWVIEW_H__AF5EEA0D_A057_11D4_A8D5_000086513E90__INCLUDED_)
