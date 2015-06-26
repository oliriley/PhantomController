// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// PhantomContextApp.cpp : Defines the class behaviors for the application.
//

#include "gstAfx.h"
#include "PhantomContextApp.h"

#include "MainFrm.h"
#include "PhantomContextDoc.h"
#include "PhantomContextView.h"
#include <dos.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextApp

// Change: derive from CHapticApp, not CWinApp
BEGIN_MESSAGE_MAP(CPhantomContextApp, CHapticApp)
    //{{AFX_MSG_MAP(CPhantomContextApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextApp construction

CPhantomContextApp::CPhantomContextApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPhantomContextApp object

CPhantomContextApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPhantomContextApp initialization

BOOL CPhantomContextApp::InitInstance()
{
    if (! CHapticApp::InitInstance())
        return FALSE;

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CPhantomContextDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CPhantomContextView));
    AddDocTemplate(pDocTemplate);

    // Create an empty document and view, and the main frame
    OnFileNew();
    if (m_pMainWnd == NULL)
        // Error
        return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
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
void CPhantomContextApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}
