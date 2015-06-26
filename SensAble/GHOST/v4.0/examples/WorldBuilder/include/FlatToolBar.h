// FlatToolBar.h
// (c) 1997, Roger Onslow
// Roger.Onslow@comsys.com.au
// posted to: http://www.codeguru.com/toolbar/devstudio_like.shtml

#ifndef FLATTOOLBAR_H
#define FLATTOOLBAR_H

class CFlatToolBar : public CToolBar
{
        DECLARE_DYNAMIC(CFlatToolBar);
public:
        void SetFlatLookStyle();
        void RepaintBackground();
        void DrawSeparators();
        void DrawSeparators(CClientDC* pDC);
        void EraseNonClient();
        void DrawGripper(CWindowDC *pDC, CRect& rectWindow);
        void Update(CFrameWnd *frame) { OnUpdateCmdUI(frame, FALSE); }
protected:
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFlatToolBar)
        virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
        //}}AFX_VIRTUAL
        // Message Handlers

        //{{AFX_MSG(CFlatToolBar)
        afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
        afx_msg void OnPaint();
        afx_msg void OnNcPaint();
        afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS*     lpncsp );
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP();
};

#endif  // FLATTOOLBAR_H
