// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// WorldBuilderApp.cpp : Defines the class behaviors for the application.
//

#include "gstAfx.h"
#include "WorldBuilderApp.h"

#include "WorldBuilderFrame.h"
#include "WorldBuilderDoc.h"
#include "WorldBuilderView.h"
#include <dos.h>
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderApp

// Change: derive from CHapticApp, not CWinApp
BEGIN_MESSAGE_MAP(CWorldBuilderApp, CHapticApp)
    //{{AFX_MSG_MAP(CWorldBuilderApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderApp construction

CWorldBuilderApp::CWorldBuilderApp()
{
    m_pWorldBuilderAppTemplate = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWorldBuilderApp object

CWorldBuilderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderApp initialization

BOOL CWorldBuilderApp::InitInstance()
{
    if (! CHapticApp::InitInstance())
        return FALSE;

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    m_pWorldBuilderAppTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CWorldBuilderDoc),
        RUNTIME_CLASS(CWorldBuilderFrame),       // main SDI frame window
        RUNTIME_CLASS(CWorldBuilderView));
    AddDocTemplate(m_pWorldBuilderAppTemplate);

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Create an empty document and view, and the main frame
    OnFileNew();
    if (m_pMainWnd == NULL)
        // Error
        return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

    return TRUE;
}

  // we overide LoadFiles so we can detect if files were loaded
  // when the program was launched, we enable GO mode if so
void CWorldBuilderApp::LoadFiles()
{
    CHapticApp::LoadFiles();

    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    CWorldBuilderFrame *frame = (CWorldBuilderFrame *)m_pMainWnd;
    ASSERT(frame);

      // go into GO mode if we opened a file
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) {
        CWorldBuilderDoc *doc = (CWorldBuilderDoc *)frame->GetActiveDocument();
        ASSERT(doc);
        doc->OnGo();
    } else {
        // open the toolbars since we're editing
        frame->toggleToolbars();
    }
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
    //{{AFX_MSG(CAboutDlg)
        // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    SetDlgItemText(IDC_VERSION, gstGetVersion());

	return TRUE;
}

// App command to run the dialog
void CWorldBuilderApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

