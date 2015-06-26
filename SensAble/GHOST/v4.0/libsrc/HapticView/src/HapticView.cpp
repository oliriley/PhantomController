// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#include "stdafx.h"
#include "HapticApp.h"
#include "HapticView.h"

#include <reset_screen.h>

#include <gstPHANToM.h>
#include <gstErrorHandler.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4390) // empty controlled statement found; is this the intent?

BOOL CHapticView::m_bRecoverableError = FALSE;
const int CHapticView::kMaxNumPHANToMs = 2;

#define UPDATE_RATE     33  // maintain an update rate of 30 Hz
#define TIMER_ID        42  // the answer to life, the universe and everything

/////////////////////////////////////////////////////////////////////////////
// CHapticView

IMPLEMENT_DYNCREATE(CHapticView, CHapticMouse)

BEGIN_MESSAGE_MAP(CHapticView, CHapticMouse)
    //{{AFX_MSG_MAP(CHapticView)
    ON_WM_TIMER()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticView construction/destruction

CHapticView::CHapticView()
{
    m_hRC = NULL;
    m_hDC = NULL;
    m_windowWidth = m_windowHeight = 0;

    m_bNeedResetScreen = FALSE; // don't draw it until you have to
}

CHapticView::~CHapticView()
{
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers

void CHapticView::OnTimer(UINT nIDEvent)
{        
    if (m_bHapticsOn && nIDEvent == m_timerID) {
        Invalidate();

        // Quit the application if the haptics process has quit
        if (ProgramDone())
            if(!m_bRecoverableError) {
                // Drop out of haptics mode
                LeaveHaptics();
                
                // Destroy the window in case the message is not received
                OnDestroy();
                
                AfxPostQuitMessage(0);
            } else {
                // force a cleanup of the servoloop before reenabling
                EnableServoLoop(FALSE);
                EnableServoLoop(TRUE);
                m_bRecoverableError = FALSE; // only try to recover once per error
            }
    }

    if (m_bStartProgram && !m_bInitialized) {
        m_bStartProgram = FALSE;

        // allow mouse mode transitions by default
        EnablePHANToMMouse(TRUE);

        // pass in status of mouse driver, StartProgram can call
        // EnablePHANToMMouse() to disable mouse mode
        StartProgram(IsMouseDriverEnabled());
        
        // haptics have been initialized
        m_bInitialized = TRUE;
        
        // Setup the error handler callback for GHOST
        setErrorCallback(HapticErrorHandler, NULL);
        
        // Load any files on the command line
        ((CHapticApp *) AfxGetApp())->LoadFiles();

        if (IsTopParentActive())
            OnPhantomEntry();

        // enable left mouse button emulation by default
        SetWhichButton(MOUSE_LEFT_BUTTON_MASK);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CHapticView drawing

BOOL CHapticView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;
    
    return CHapticMouseBase::PreCreateWindow(cs);
}

void CHapticView::OnDraw(CDC* pDC)
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_bInitialized && m_bNeedResetScreen) {
        // Put up image that tells user to hold PHANToM in reset position
        RECT viewRect;
        GetClientRect(&viewRect);
        draw_reset_image(viewRect.right, viewRect.bottom);
    } else if (m_bInitialized)
        UpdateGraphics();

    glFlush();
    
    // Swap the front and back buffers
    SwapBuffers(m_hDC);
}

int CHapticView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // create the parent CView
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    int nPixelFormat;                           // pixel format index
    m_hDC = ::GetDC(m_hWnd);                    // get the device context

    static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),      // size of thie structure
            1,                                  // version of this structure
            PFD_DRAW_TO_WINDOW |                // draw to window (not a bitmap)
            PFD_SUPPORT_OPENGL |                // support OpenGL in the window
            PFD_DOUBLEBUFFER,                   // double-buffered mode enabled
            PFD_TYPE_RGBA,                      // RGBA color mode
            24,                                 // attempt to get 24-bit color
            0, 0, 0, 0, 0, 0,                   // unused mode selectors
            0, 0,                               // unused mode selectors
            0, 0, 0, 0, 0,                      // unused mode selectors
            32,                                 // size of depth buffer
            4,                                  // size of stencil buffer
            0,                                  // used mode selector
            PFD_MAIN_PLANE,                     // draw in main plane
            0,                                  // unused mode selector
            0, 0, 0,                            // unused mode selectors
    };
    
    // Choose a pixel format that best matches the options in pfd
    if (!(nPixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
        return -1;
    
    // Set the pixel format for the device context
    if (!SetPixelFormat(m_hDC, nPixelFormat, &pfd))
        return -1;
    
    // Create the rendering context
    if (!(m_hRC = wglCreateContext(m_hDC)))
        return -1;
    
    // Make the rendering context current
    if (!wglMakeCurrent(m_hDC, m_hRC))
        return -1;
    
    // Perform initialization of the rendering context
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    // Create palette
    CreateRGBPalette();
    
    // Initialize graphics modes
    InitGraphics();
    
    // Post a redraw request on the view
    Invalidate();

    // Open a connection to the PHANToM mouse driver
    if (OpenMouseDriver())
        // Query whether the PHANToM Mouse simulation is running
        if (m_bPHANToMMouseEnabled = IsMouseDriverEnabled())
            m_bStartProgram = TRUE;
        
    LPCSTR *apszNames = QueryPHANToMNames();
    m_bNeedResetScreen = gstPHANToM::IsResetNeeded(apszNames, kMaxNumPHANToMs);
    
    // If all devices are Desktop models, no reset screen is necessary
    if (!m_bNeedResetScreen)
        m_bStartProgram = TRUE;
            
    return 0; // must return 0 to continue creation of CWnd
}

void CHapticView::OnDestroy()
{
    // disable mouse mode flag
    EnablePHANToMMouse(FALSE);

    // Drop out of haptics mode
    LeaveHaptics();
    
    // Disable the mouse driver
    if (IsMouseDriverEnabled())
        EnableMouseDriver(FALSE);

    // Close the connection to the PHANToM mouse driver
    CloseMouseDriver();
    
    // Terminate graphics modes and destroy display lists
    TermGraphics();

    // Cleanup whatever gstScene elements are left
    EndProgram();             

    // Unset the initialized flag
    m_bInitialized = FALSE;

    // Delete the rendering context and release the device context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hRC);    
    ::ReleaseDC(m_hWnd, m_hDC);

    CView::OnDestroy();
}

BOOL CHapticView::OnEraseBkgnd(CDC* pDC)
{
    return FALSE; // prevent resize flicker
}

void CHapticView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    
    ResizeGraphics(cx, cy);
    m_windowWidth = cx;
    m_windowHeight = cy;
}

void CHapticView::OnActivateView(BOOL bActivate, CView* pActivateView,
                                 CView* pDeactiveView)
{
    static BOOL bPrevMouseMode = FALSE;
    static BOOL bViewState = FALSE;

    CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

    // Enable/Disable the graphics loop timer
    if (bActivate)
        m_timerID = SetTimer(TIMER_ID, UPDATE_RATE, NULL);
    else
        KillTimer(m_timerID);

    // Protection against multiple non-toggling calls
    if (bViewState != bActivate)
        bViewState = bActivate;
    else
        return;

    // Always restore the state of the PHANToM mouse enable flag
    if (bPrevMouseMode) {
        EnablePHANToMMouse(TRUE);
        bPrevMouseMode = FALSE;
    }

    // Haptics should only be on when the application is in the foreground
    // Don't allow the mouse to enable when leaving
    if (m_bInitialized && !IsMouseDriverEnabled())
        if (bActivate)
            EnterHaptics();
        else {
            // Save the state of the PHANToM mouse enable flag
            if (bPrevMouseMode = IsPHANToMMouseEnabled())
                EnablePHANToMMouse(FALSE);
            LeaveHaptics();
        }
}

BOOL CHapticView::OnQueryNewPalette()
{
    // If the palette was created.
	if((HPALETTE) m_GLPalette) {
		int nRet;

		// Selects the palette into the current device context
		SelectPalette(m_hDC, (HPALETTE) m_GLPalette, FALSE);

		// Map entries from the currently selected palette to
		// the system palette.  The return value is the number 
		// of palette entries modified.
		nRet = RealizePalette(m_hDC);

		// Repaint, forces remap of palette in current window
		InvalidateRect(NULL, FALSE);

		return nRet;
	}

	return CView::OnQueryNewPalette();
}

void CHapticView::OnPaletteChanged(CWnd* pFocusWnd)
{
    if (((HPALETTE) m_GLPalette != NULL) && (pFocusWnd != this)) {
        // Select the palette into the device context
        SelectPalette(m_hDC, (HPALETTE) m_GLPalette, FALSE);

        // Map entries to system palette
        RealizePalette(m_hDC);

        // Remap the current colors to the newly realized palette
        UpdateColors(m_hDC);
        return;
    }

    CView::OnPaletteChanged(pFocusWnd);
}

UCHAR CHapticView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    static UCHAR oneto8[2] = {
        0, 255
    };
    static UCHAR twoto8[4] = {
        0, 0x55, 0xaa, 0xff
    };
    static UCHAR threeto8[8] = {
        0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
    };
    UCHAR   val = (UCHAR) (i >> shift);
    
    switch (nbits) {
    case 1:
        val &= 0x1;
        return oneto8[val];
    case 2:
        val &= 0x3;
        return twoto8[val];
    case 3:
        val &= 0x7;
        return threeto8[val];
    default:
        return 0;
    }
}

void CHapticView::CreateRGBPalette()
{
    static PALETTEENTRY defaultPalEntry[20] = {
        { 0,    0,    0,    0 },
        { 0x80, 0,    0,    0 },
        { 0,    0x80, 0,    0 },
        { 0x80, 0x80, 0,    0 },
        { 0,    0,    0x80, 0 },
        { 0x80, 0,    0x80, 0 },
        { 0,    0x80, 0x80, 0 },
        { 0xC0, 0xC0, 0xC0, 0 },
        
        { 192, 220, 192,  0 },
        { 166, 202, 240,  0 },
        { 255, 251, 240,  0 },
        { 160, 160, 164,  0 },
        
        { 0x80, 0x80, 0x80, 0 },
        { 0xFF, 0,    0,    0 },
        { 0,    0xFF, 0,    0 },
        { 0xFF, 0xFF, 0,    0 },
        { 0,    0,    0xFF, 0 },
        { 0xFF, 0,    0xFF, 0 },
        { 0,    0xFF, 0xFF, 0 },
        { 0xFF, 0xFF, 0xFF, 0 }
    };
    static int defaultOverride[13] = {
        0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
    };
    PIXELFORMATDESCRIPTOR   pfd;
    LOGPALETTE             *pPal;
    
    int n = GetPixelFormat(m_hDC);
    DescribePixelFormat(m_hDC, n, sizeof(pfd), &pfd);
    
    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        n = 1 << pfd.cColorBits;
        pPal = (PLOGPALETTE) new char[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];
        
        ASSERT(pPal != NULL);
        
        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;
        for (int i = 0; i < n; i++) {
            pPal->palPalEntry[i].peRed   = ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen = ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue  = ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }
        
        // Fix up the palette to include the default GDI palette
        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6))
            for (i = 1; i <= 12; i++)
                pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
            
            m_GLPalette.CreatePalette(pPal);
            delete pPal;
            
            SelectPalette(m_hDC, (HPALETTE) m_GLPalette, FALSE);
            RealizePalette(m_hDC);
    }
}

void CHapticView::HapticErrorHandler(int errorId, char *message, void *userdata)
{
    if (IsRecoverableError(errorId))
        m_bRecoverableError = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Interface function stubs

void CHapticView::StartProgram(BOOL bPHANToMMouseActive)
{
    // Client must implement
    ASSERT(FALSE);
}

void CHapticView::EndProgram()
{
    // Client must implement
    ASSERT(FALSE);
}

BOOL CHapticView::ProgramDone()
{
    // Client must implement
    ASSERT(FALSE);
    return FALSE;
}

void CHapticView::InitGraphics()
{
    // Client must implement
    ASSERT(FALSE);
}

void CHapticView::UpdateGraphics()
{
    // Client must implement
    ASSERT(FALSE);
}

void CHapticView::ResizeGraphics(int cx, int cy)
{
    // Client must implement
    ASSERT(FALSE);
}

void CHapticView::TermGraphics()
{
    // Client must implement
    ASSERT(FALSE);
}

LPCSTR *CHapticView::QueryPHANToMNames()
{
    // Client must implement
    ASSERT(FALSE);
    return NULL;
}
