/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  HapticViewerDoc.cpp

Description: 

  MFC CView derived view class for haptic viewer.

******************************************************************************/

#include "stdafx.h"
#include <assert.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "HapticViewer.h"
#include "HapticViewerDoc.h"
#include "HapticViewerView.h"

#include "Mesh.h"

#include <HD/hd.h>
#include <HL/hl.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FONT_HEIGHT 16

static LARGE_INTEGER g_liFreq = {0,0};
static LARGE_INTEGER g_drawPerfli;
    

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerView

IMPLEMENT_DYNCREATE(CHapticViewerView, CView)

    BEGIN_MESSAGE_MAP(CHapticViewerView, CView)
//{{AFX_MSG_MAP(CHapticViewerView)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
//}}AFX_MSG_MAP
// Standard printing commands
    ON_COMMAND(ID_VIEW_PERFORMANCE, OnPerformance)
    ON_COMMAND(ID_VIEW_WIREFRAME, OnWireframe)
    ON_COMMAND(ID_VIEW_HAPTICCAMERAVISUAL, OnHapticCameraVisual)
    ON_COMMAND(ID_SHAPE_DEPTHBUFFER, OnShapeDepthBuffer)
    ON_COMMAND(ID_SHAPE_FEEDBACKBUFFER, OnShapeFeedbackBuffer)
    ON_COMMAND(ID_HAPTIC_HLHAPTICCAMERAVIEW, OnHapticCameraView)
    ON_COMMAND(ID_HAPTIC_HLHAPTICADAPTIVEVIEWPORT, OnHapticAdaptiveViewport)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PERFORMANCE, OnUpdatePerformance)
    ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, OnUpdateWireframe)
    ON_UPDATE_COMMAND_UI(ID_VIEW_HAPTICCAMERAVISUAL, OnUpdateHapticCameraVisual)
    ON_UPDATE_COMMAND_UI(ID_HAPTIC_HLHAPTICADAPTIVEVIEWPORT, OnUpdateHapticAdaptiveViewport)
    ON_UPDATE_COMMAND_UI(ID_SHAPE_DEPTHBUFFER, OnUpdateShapeDepthBuffer)
    ON_UPDATE_COMMAND_UI(ID_SHAPE_FEEDBACKBUFFER, OnUpdateShapeFeedbackBuffer)
    ON_UPDATE_COMMAND_UI(ID_HAPTIC_HLHAPTICCAMERAVIEW, OnUpdateHapticCameraView)
    ON_UPDATE_COMMAND_UI(ID_HAPTIC_HLHAPTICADAPTIVEVIEWPORT, OnUpdateHapticAdaptiveViewport)
    END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerView construction/destruction

void updateDragObjectTransform(Mesh* pObj);
                         

CHapticViewerView::CHapticViewerView()
    : m_cursorDisplayList(0),
      m_hHD(HD_INVALID_HANDLE),
      m_hHLRC(NULL),
      m_fontListBase(1000),
      m_bPerformance(true),
      m_bWireframe(false),
      m_bHapticCameraVisual(false),
      m_bShapeDepthBuffer(false),
      m_bHapticCameraView(true),
      m_bHapticAdaptiveViewport(false),
      m_bSinglePassRender(false),
      m_cameraScale(1.0),
      m_cameraTranslationX(0),
      m_cameraTranslationY(0),
      m_isRotatingCamera(false),
      m_isScalingCamera(false),
      m_isTranslatingCamera(false)
{
    // TODO: add construction code here
}

CHapticViewerView::~CHapticViewerView()
{
}

/////////////////////////////////////////////////////////////////////////////
// Menu management

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnPerformance()
{
    m_bPerformance = !m_bPerformance;
}

void CHapticViewerView::OnUpdatePerformance(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bPerformance);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnWireframe()
{
    m_bWireframe = !m_bWireframe;
}

void CHapticViewerView::OnUpdateWireframe(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_bSinglePassRender);
    pCmdUI->SetCheck(m_bWireframe);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnHapticCameraVisual()
{
    m_bHapticCameraVisual = !m_bHapticCameraVisual;
}

void CHapticViewerView::OnUpdateHapticCameraVisual(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bHapticCameraView);
    pCmdUI->SetCheck(m_bHapticCameraVisual);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnShapeDepthBuffer()
{
    m_bShapeDepthBuffer = true;
}

void CHapticViewerView::OnUpdateShapeDepthBuffer(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_bShapeDepthBuffer);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnShapeFeedbackBuffer()
{
    m_bShapeDepthBuffer = false;
}

void CHapticViewerView::OnUpdateShapeFeedbackBuffer(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(!m_bShapeDepthBuffer);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnHapticCameraView()
{
    m_bHapticCameraView = !m_bHapticCameraView;
    m_bHapticAdaptiveViewport = false;
}

void CHapticViewerView::OnUpdateHapticCameraView(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bHapticCameraView);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CHapticViewerView::OnHapticAdaptiveViewport()
{
    m_bHapticAdaptiveViewport = !m_bHapticAdaptiveViewport;
    m_bHapticCameraView = false;
    m_bHapticCameraVisual = false;
}

void CHapticViewerView::OnUpdateHapticAdaptiveViewport(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bShapeDepthBuffer);
    pCmdUI->SetCheck(m_bHapticAdaptiveViewport);
}
/////////////////////////////////////////////////////////////////////////////


// Menu management
/////////////////////////////////////////////////////////////////////////////

BOOL CHapticViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
    // required window styles for OpenGL
    cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);

    return CView::PreCreateWindow(cs);
}

hduVector3Dd CHapticViewerView::getProxyPositionInModelCoord(
    Mesh* pObj
    )
{
    hduMatrix mat = pObj->transform.getInverse();
        
    hduVector3Dd proxyPos;
    hlGetDoublev(HL_PROXY_POSITION, proxyPos);

    hduVector3Dd posMC;
    mat.multVecMatrix(proxyPos, posMC);

    return posMC;
}

//////////////////////////////////////////////////////////////////////////////
//  transformFrustum -
//     Transform srcFrustum by mat, result in dstFrustum.
//
//////////////////////////////////////////////////////////////////////////////
void transformFrustum(
    const hduMatrix& mat,
    const hduVector3Dd* srcFrustum,
    hduVector3Dd* dstFrustum
    )
{
    for (int i = 0; i < 8; ++i)
    {
        hduVector3Dd dst;
        mat.multVecMatrix(srcFrustum[i], dst);

        dstFrustum[i][0] = dst[0];
        dstFrustum[i][1] = dst[1];
        dstFrustum[i][2] = dst[2];
    }
}

enum 
{ 
    LEFTBOTTOMNEAR, RIGHTBOTTOMNEAR, RIGHTTOPNEAR, LEFTTOPNEAR,
    LEFTBOTTOMFAR, RIGHTBOTTOMFAR, RIGHTTOPFAR, LEFTTOPFAR
};

void CHapticViewerView::getViewVolumeInModelCoord(
    hduVector3Dd* frustum)
{
    GLdouble projection[16];
    GLdouble modelview[16];

    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    bool bNoError = false;

    hduMatrix modelTeye(modelview);
    hduMatrix eyeTmodel = modelTeye;
    bNoError = eyeTmodel.invert();
    assert(bNoError);

    hduMatrix eyeTclip(projection);
    hduMatrix clipTeye = eyeTclip;
    bNoError = clipTeye.invert();
    assert(bNoError);

    hduMatrix clipTmodel = clipTeye.multRight(eyeTmodel);

    // Compute the edges of the view frustum by transforming canonical
    // coordinates for the corners into eye space.

    hduVector3Dd localFrustum[8];
    localFrustum[LEFTBOTTOMNEAR]  = hduVector3Dd(-1, -1, -1);
    localFrustum[LEFTBOTTOMFAR]   = hduVector3Dd(-1, -1,  1);
    localFrustum[RIGHTBOTTOMNEAR] = hduVector3Dd( 1, -1, -1);
    localFrustum[RIGHTBOTTOMFAR]  = hduVector3Dd( 1, -1,  1); 
    localFrustum[RIGHTTOPNEAR]    = hduVector3Dd( 1,  1, -1);
    localFrustum[RIGHTTOPFAR]     = hduVector3Dd( 1,  1,  1);
    localFrustum[LEFTTOPNEAR]     = hduVector3Dd(-1,  1, -1);
    localFrustum[LEFTTOPFAR]      = hduVector3Dd(-1,  1,  1);
    
    transformFrustum(clipTmodel, localFrustum, frustum);
}

int CHapticViewerView::drawSceneGL(Mesh* pObj, bool isHapticView)
{
    int triCount = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glMultMatrixd(pObj->transform);

    hduVector3Dd* currFrustum = m_graphicFrustum;

    if (isHapticView)
    {
        // haptic render pass
        if (!m_bSinglePassRender)
        {
            getViewVolumeInModelCoord(m_hapticCameraFrustum);
            if (m_bHapticCameraView)
                currFrustum = m_hapticCameraFrustum;
            
            if (!m_bShapeDepthBuffer)
            {
                // FEEDBACK_BUFFER shape, 
                // count number of tris and send hint
                int triCount = pObj->numTri(currFrustum);
                hlHinti(HL_SHAPE_FEEDBACK_BUFFER_VERTICES, triCount*3);
            }
        }
    }

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.5, 0.5, 0.5);

    triCount = pObj->drawGL(currFrustum);

    glPopMatrix();

    return triCount;
}

int CHapticViewerView::drawSceneHL(Mesh* pObj)
{
    int triDrawCount = 0;
 
    hlBeginFrame();
    
    hlCheckEvents();

    HLboolean buttDown;
    hlGetBooleanv(HL_BUTTON1_STATE, &buttDown);

    if (buttDown && pObj->isDragging)
    {
        updateDragObjectTransform(pObj);
    }
    else
    {
    if (m_bHapticCameraView)
        hlEnable(HL_HAPTIC_CAMERA_VIEW);
    else
        hlDisable(HL_HAPTIC_CAMERA_VIEW);
    
    if (m_bHapticAdaptiveViewport)
        hlEnable(HL_ADAPTIVE_VIEWPORT);
    else
        hlDisable(HL_ADAPTIVE_VIEWPORT);
    
    if (m_bShapeDepthBuffer)
        hlBeginShape(HL_SHAPE_DEPTH_BUFFER, pObj->shapeId);
    else
        hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, pObj->shapeId);
    }

    const bool isHapticView = true;
    triDrawCount = drawSceneGL(pObj, isHapticView);
    
    hlEndShape();

    hlEndFrame();
    
    return triDrawCount;
}

void CHapticViewerView::drawFrustum(hduVector3Dd* frustum)
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    glColor3f(0,1,0);

    glBegin(GL_LINE_LOOP);
    glVertex3dv(frustum[LEFTBOTTOMNEAR]);
    glVertex3dv(frustum[LEFTBOTTOMFAR]);
    glVertex3dv(frustum[LEFTTOPFAR]);
    glVertex3dv(frustum[LEFTTOPNEAR]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3dv(frustum[RIGHTBOTTOMNEAR]);
    glVertex3dv(frustum[RIGHTBOTTOMFAR]);
    glVertex3dv(frustum[RIGHTTOPFAR]);
    glVertex3dv(frustum[RIGHTTOPNEAR]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3dv(frustum[LEFTBOTTOMNEAR]);
    glVertex3dv(frustum[RIGHTBOTTOMNEAR]);
    glVertex3dv(frustum[RIGHTTOPNEAR]);
    glVertex3dv(frustum[LEFTTOPNEAR]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3dv(frustum[LEFTBOTTOMFAR]);
    glVertex3dv(frustum[RIGHTBOTTOMFAR]);
    glVertex3dv(frustum[RIGHTTOPFAR]);
    glVertex3dv(frustum[LEFTTOPFAR]);
    glEnd();

    glPopAttrib();
}


////////////////////////////////////////////////////////////////////////////
// CHapticViewerView drawing
//
void CHapticViewerView::OnDraw(CDC* pDC)
{
    CHapticViewerDoc* pDoc = GetDocument();

    // Ceck pauseDraw to see if drawing should be paused to allow the user to 
    // respond to an error message box.
    if (pDoc->m_pauseDraw) return;

    Mesh* pObj = pDoc->getObj();

    wglMakeCurrent(m_hDC,m_hRC);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (pObj)
    {
        static double s_hlElapsed = 0.0;
        static double s_glElapsed = 0.0;
        static double s_totalElapsed = 0.0;
        static int s_hlTriCount = 0;
        static int s_glTriCount = 0;
    
        int hlTriCount = 0;
        int glTriCount = 0;
 
        double hlElapsed = 0.0;
        double glElapsed = 0.0;
        double totalElapsed = 0.0;

        // Manage how often to update performance number to avoid smearing.
        const double showPerfNumSec = 0.25;
    
        LARGE_INTEGER startHapticli, startGraphicli, startTotalli, endli;
        QueryPerformanceCounter(&startTotalli);
        QueryPerformanceCounter(&startHapticli);

        // Single pass render for both haptics and graphics.
        m_bSinglePassRender = m_bShapeDepthBuffer && !m_bHapticCameraView;

        //
        // draw haptic scene
        //
        hlTriCount = drawSceneHL(pObj);
        
        QueryPerformanceCounter(&endli);
        hlElapsed = double(_int64(endli.QuadPart - 
                                  startHapticli.QuadPart)) / g_liFreq.QuadPart;
        
        //
        // draw graphic scene
        //
        QueryPerformanceCounter(&startGraphicli);
        if (!m_bSinglePassRender)
        {
            // Use render produced by drawSceneHL (single pass render).
            glPushAttrib(GL_POLYGON_BIT);
            {
                if (m_bWireframe)
                {
                    glPolygonMode(GL_FRONT, GL_LINE);
                }

                const bool isHapticView = false;
                glTriCount = drawSceneGL(pObj, isHapticView);
            }
            glPopAttrib();

        }

        drawCursorGL();

        // Draw the frustum (if desired).
        if (m_bHapticCameraVisual)
        {
            glPushMatrix();
            glMultMatrixd(pObj->transform);

            drawFrustum(m_hapticCameraFrustum);

            glPopMatrix();
        }

        glFinish();
        QueryPerformanceCounter(&endli);
        glElapsed = double(_int64(endli.QuadPart - 
                                  startGraphicli.QuadPart)) / g_liFreq.QuadPart;

        totalElapsed = double(_int64(endli.QuadPart - 
                                     startTotalli.QuadPart)) / g_liFreq.QuadPart;

        // QueryPerformanceCounter(&endli);
        double perfElapsed = double(_int64(endli.QuadPart - 
                                           g_drawPerfli.QuadPart)) / g_liFreq.QuadPart;
        if ((perfElapsed > showPerfNumSec) && 
            (hlTriCount > 0 || glTriCount > 0))
        {
            QueryPerformanceCounter(&g_drawPerfli);
            s_hlElapsed = hlElapsed;
            s_glElapsed = glElapsed;
            s_totalElapsed = totalElapsed;
            s_hlTriCount = hlTriCount;
            s_glTriCount = glTriCount;
        }
        
        if (m_bPerformance)
        {
            drawPerformance(s_hlElapsed, s_glElapsed, s_totalElapsed, 
                            s_hlTriCount, s_glTriCount);
        }
    

        Invalidate(FALSE);
    }

    SwapBuffers(m_hDC);
}


/////////////////////////////////////////////////////////////////////////////
// CHapticViewerView message handlers

BOOL CHapticViewerView::OnEraseBkgnd(CDC* pDC) 
{
    return FALSE;
}

void CHapticViewerView::OnSize(UINT nType, int cx, int cy) 
{
    CView::OnSize(nType, cx, cy);
        
    m_windowHeight = cy;
    m_windowWidth  = cx;

    CHapticViewerDoc* pDoc = GetDocument();
    Mesh* pObj = pDoc->getObj();

    if (pObj)
    {
        updateGLView(pObj);
    }
}

BOOL CHapticViewerView::SetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,  
            PFD_TYPE_RGBA,
            24,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16,
            0,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
        
        
    m_hDC = ::GetDC(m_hWnd);
    GLuint PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        
    if (!PixelFormat)
    {
        return false ;
    }
        
    if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))
    {
        return false;
    }
        
    m_hRC = wglCreateContext(m_hDC);
    if(!m_hRC)
    {
        return false;
    }
        
    if(!wglMakeCurrent(m_hDC, m_hRC))
    {
        return false;
    }
        
    initGL();
    initHL();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SwapBuffers(m_hDC);

    QueryPerformanceCounter(&g_drawPerfli);
    QueryPerformanceFrequency(&g_liFreq);

    return true;
}

void CHapticViewerView::initGL()
{
    GLfloat direction[4] = { 0.0, 0.0, 1.0, 0.0 };
        
    glEnable(GL_DEPTH_TEST);                                        
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0f);              
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                                                       
        
    glLightfv(GL_LIGHT0, GL_POSITION, direction);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
        
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_NORMALIZE);

    m_fontListBase = glGenLists(256);
    HFONT hFont = CreateFont(FONT_HEIGHT,
                             0,
                             0,
                             0,
                             FW_BOLD,
                             FALSE,
                             FALSE,
                             FALSE,
                             ANSI_CHARSET,
                             OUT_TT_PRECIS,
                             CLIP_DEFAULT_PRECIS,
                             ANTIALIASED_QUALITY,
                             FF_DONTCARE|DEFAULT_PITCH,
                             "Arial");
    
    wglUseFontBitmaps(m_hDC, 0, 255, m_fontListBase);
}

void CHapticViewerView::initHL()
{
    HDErrorInfo error;
    m_hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        char errorMsg[500];
        sprintf(errorMsg,
                "HD error %s\nHHD: %X\nError Code: %X\nInternalErrorCode: %d\n%s\n",
                hdGetErrorString(error.errorCode),
                error.hHD,
                error.errorCode,
                error.internalErrorCode,
                "Failed to initialize haptic device");
        MessageBox(errorMsg);
        getchar();
        exit(1);
    }   
    
    // Create a haptic context for the device. The haptic context maintains 
    // the state that persists between frame intervals and is used for
    // haptic rendering.
    m_hHLRC = hlCreateContext(m_hHD);
    hlMakeCurrent(m_hHLRC); 

    hlTouchableFace(HL_FRONT);
}


int CHapticViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
        
    SetupPixelFormat();
    wglMakeCurrent(NULL,NULL);
        
    return 0;
}


void CHapticViewerView::OnDestroy()
{
    wglDeleteContext(m_hRC);
    hlDeleteContext(m_hHLRC);

    CView::OnDestroy();
}

void CHapticViewerView::updateGLCamera()
{
    CHapticViewerDoc* pDoc = GetDocument();
    Mesh* pMesh = pDoc->getObj();

    hduVector3Df center = pMesh ? pMesh->center() : hduVector3Df(0,0,0);

    wglMakeCurrent(m_hDC,m_hRC);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    gluLookAt(m_cameraPosWC[0], m_cameraPosWC[1], m_cameraPosWC[2],
              center[0], center[1], center[2],
              0, 1, 0);

    glTranslatef(m_cameraTranslationX, m_cameraTranslationY, 0);

    glTranslatef(center[0], center[1], center[2]);
    glMultMatrixd(m_cameraRotation);
    glTranslatef(-center[0], -center[1], -center[2]);

    glScaled(m_cameraScale, m_cameraScale, m_cameraScale);

    getViewVolumeInModelCoord(m_graphicFrustum);
}

void CHapticViewerView::updateGLView(Mesh* pMesh)
{
    wglMakeCurrent(m_hDC,m_hRC);

    static const double kPI = 3.1415926535897932384626433832795;
    static const double kFovY = 50;
    
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Compute the viewing parameters based on a fixed fov and viewing
    // a canonical [-1..1] box centered at the origin.
    const double halfBox = 1.0;

    double nearDist = halfBox/tan((kFovY / 2.0) * kPI / 180.0);
    double farDist = nearDist + 2*halfBox;
    double aspect = (double) m_windowWidth / m_windowHeight;

    // Move near dist back to avoid clipping front face.
    nearDist *= 0.9;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
        
    gluPerspective(kFovY, aspect, nearDist, farDist);

    m_cameraPosWC[0] = 0;
    m_cameraPosWC[1] = 0;
    m_cameraPosWC[2] = nearDist + halfBox;
        
    updateGLCamera();

    updateWorkspace();
}

#define CURSOR_SIZE_PIXELS 20
void CHapticViewerView::updateWorkspace()
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    hlMatrixMode(HL_TOUCHWORKSPACE);
    hlLoadIdentity();
    
    // Fit haptic workspace to view volume.
    hluFitWorkspace(projection);

    // Compute cursor scale.
    m_cursorScale = hluScreenToModelScale(modelview, projection, viewport);
    m_cursorScale *= CURSOR_SIZE_PIXELS;
}

void CHapticViewerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    CView::OnUpdate(pSender, lHint, pHint);

    initCameraTransform();

    CHapticViewerDoc* pDoc = GetDocument();
    Mesh* pObj = pDoc->getObj();

        if (pObj)
    {
        updateGLView(pObj);
    }
}

void elapsedToStringFPS(double elapsed, char* str)
{
    double fps = 1 / elapsed;

    int numDigits = 8;
    int decimal, sign;
    
    char* fpsStr = _ecvt(fps, numDigits, &decimal, &sign);
    
    if (decimal > 0)
    {
        strncpy(str, fpsStr, decimal);
        strncpy(str+decimal, ".", 1);
        strncpy(str+decimal+1, fpsStr+decimal, 2);
        str[decimal+3] = '\0';
    }
}

void CHapticViewerView::drawPerformance(double hlElapsed, 
                                        double glElapsed,
                                        double totalElapsed,
                                        int hlTriCount, 
                                        int glTriCount)
{
    char hlFPS[32] = "HL FPS: 0";
    char glFPS[32] = "GL FPS: 0";
    char totalFPS[32] = "Total FPS: 0";

    if (hlElapsed > 0)
    {
        char hlFPSNum[16];
        elapsedToStringFPS(hlElapsed, hlFPSNum);
        strcpy(hlFPS+strlen(hlFPS)-1, hlFPSNum);
    }

    if (glElapsed > 0)
    {
        char glFPSNum[16];
        elapsedToStringFPS(glElapsed, glFPSNum);
        strcpy(glFPS+strlen(glFPS)-1, glFPSNum);
    }

    if (totalElapsed > 0)
    {
        char totalFPSNum[16];
        elapsedToStringFPS(totalElapsed, totalFPSNum);
        strcpy(totalFPS+strlen(totalFPS)-1, totalFPSNum);
    }
        
    char shapeType[32] = "Shape: ";
    if (m_bShapeDepthBuffer)
    {
        if (m_bSinglePassRender)
            strcpy(shapeType+strlen(shapeType), "DEPTH (SINGLE PASS)");
        else
            strcpy(shapeType+strlen(shapeType), "DEPTH (DUAL PASS)");
    }
    else
        strcpy(shapeType+strlen(shapeType), "FEEDBACK");

    char chGLTriCount[64] = "GL Tri draw count: ";
    _itoa(glTriCount, chGLTriCount + strlen(chGLTriCount), 10);

    char chHLTriCount[64] = "HL Tri draw count: ";
    _itoa(hlTriCount, chHLTriCount + strlen(chHLTriCount), 10);

    // Used for single pass.
    char chTriCount[64] = "Tri draw count: ";
    _itoa(hlTriCount, chTriCount + strlen(chTriCount), 10);

    int fontOffset = FONT_HEIGHT+10;

    // For some reason the first (and only the first)
    // text drawn is drawn in the model color despite
    // the color being explictly set in drawTextGL.
    // Side-step the issue by calling drawTextGL will null text.
    char nullText[] = "";
    drawTextGL(0, m_windowHeight, nullText);

    if (m_bSinglePassRender)
    {
        drawTextGL(0, m_windowHeight-3*fontOffset, shapeType);
        drawTextGL(0, m_windowHeight-2*fontOffset, totalFPS);
        drawTextGL(0, m_windowHeight-1*fontOffset, chTriCount);
    }
    else
    {
        drawTextGL(0, m_windowHeight-6*fontOffset, shapeType);

        drawTextGL(0, m_windowHeight-5*fontOffset, hlFPS);
        drawTextGL(0, m_windowHeight-4*fontOffset, glFPS);
        drawTextGL(0, m_windowHeight-3*fontOffset, totalFPS);
    
        drawTextGL(0, m_windowHeight-2*fontOffset, chHLTriCount);
        drawTextGL(0, m_windowHeight-1*fontOffset, chGLTriCount);
    }
}

void CHapticViewerView::drawTextGL(int x, int y, char* text)
{
    glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT | GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    y = m_windowHeight - FONT_HEIGHT - y;
    glViewport(x-1, y-1, 0, 0);

    glRasterPos4f(0, 0, 0, 1);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);

    glListBase(m_fontListBase);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

    glPopAttrib();
}

void CHapticViewerView::drawCursorGL()
{
    static const double kCursorRadius = 0.5;
    static const double kCursorHeight = 1.5;
    static const int kCursorTess = 15;
    HLdouble proxyxform[16];

    GLUquadricObj *qobj = 0;

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();

    if (!m_cursorDisplayList)
    {
        m_cursorDisplayList = glGenLists(1);
        glNewList(m_cursorDisplayList, GL_COMPILE);

        qobj = gluNewQuadric();
        gluSphere(qobj, kCursorRadius, kCursorTess, kCursorTess);
        gluDeleteQuadric(qobj);

        glEndList();
    }
    
    // Get the proxy transform in world coordinates.
    hlGetDoublev(HL_PROXY_TRANSFORM, proxyxform);
    glMultMatrixd(proxyxform);

    // Apply the local cursor scale factor.
    glScaled(m_cursorScale, m_cursorScale, m_cursorScale);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.0f, 0.5f, 1.0f);

    glCallList(m_cursorDisplayList);

    glPopMatrix(); 
    glPopAttrib();
}


/******************************************************************************
 Calculates updated object transform for drag object based on changes to
 proxy transform.
******************************************************************************/
void updateDragObjectTransform(Mesh* pObj)
{
    // Calculated delta between current proxy pos and proxy poas at start 
    // of drag.
    hduVector3Dd proxyPos;
    hlGetDoublev(HL_PROXY_POSITION, proxyPos);
    hduVector3Dd dragDeltaTransl = proxyPos - pObj->startDragProxyPos;

    // Same for rotation.
    hduMatrix deltaRotMat;
    // if (rotate)
    {
        hduQuaternion proxyRotq;
        hlGetDoublev(HL_PROXY_ROTATION, proxyRotq);
        hduQuaternion dragDeltaRot = 
            pObj->startDragProxyRot.inverse() * proxyRotq;
        dragDeltaRot.normalize();
        dragDeltaRot.toRotationMatrix(deltaRotMat);

        // Want to rotate about the proxy position, not the origin
        // so need to translate to/from proxy pos.
        hduMatrix toProxy = hduMatrix::createTranslation(-pObj->startDragProxyPos);
        hduMatrix fromProxy = hduMatrix::createTranslation(pObj->startDragProxyPos);
        deltaRotMat = toProxy * deltaRotMat * fromProxy;
    }

    // Compose rotation and translation deltas.
    hduMatrix deltaMat = 
        deltaRotMat * hduMatrix::createTranslation(dragDeltaTransl);

    pObj->transform = pObj->startDragObjTransform * deltaMat;
}

void CHapticViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_isRotatingCamera = true;
}

void CHapticViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_isRotatingCamera = false;
    updateWorkspace();
}

void CHapticViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_isScalingCamera = true;
}

void CHapticViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_isScalingCamera = false;
    updateWorkspace();
}

void CHapticViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
    m_isTranslatingCamera = true;
}

void CHapticViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
    m_isTranslatingCamera = false;
    updateWorkspace();
}

/******************************************************************************* 
 This routine is used by the view rotation code for simulating a virtual
 trackball.  This math computes the z height for a 2D projection onto the
 surface of a 2.5D sphere.  When the input point is near the center of the
 sphere, this routine computes the actual sphere intersection in Z.  When 
 the input point moves towards the outside of the sphere, this routine will 
 solve for a hyperbolic projection, so that it still yields a meaningful answer.
*******************************************************************************/
double projectToTrackball(double radius, double x, double y)
{
    static const double kUnitSphereRadius2D = sqrt(2.0);
    double z;

    double dist = sqrt(x * x + y * y);
    if (dist < radius * kUnitSphereRadius2D / 2.0)
    {
        // Solve for sphere case.
        z = sqrt(radius * radius - dist * dist);
    }
    else
    {
        // Solve for hyperbolic sheet case.
        double t = radius / kUnitSphereRadius2D;
        z = t * t / dist;
    }

    return z;
}

void CHapticViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
    CHapticViewerDoc* pDoc = GetDocument();
    Mesh* pObj = pDoc->getObj();
    if (!pObj) return;
    
    int x = point.x;
    int y = point.y;
    
    if (m_isRotatingCamera)
    {
        static const double kTrackBallRadius = 0.8;   

        hduVector3Dd lastPos;
        lastPos[0] = m_lastMouseX * 2.0 / m_windowWidth - 1.0;
        lastPos[1] = (m_windowHeight - m_lastMouseY) * 2.0 / m_windowHeight - 1.0;
        lastPos[2] = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

        hduVector3Dd currPos;
        currPos[0] = x * 2.0 / m_windowWidth - 1.0;
        currPos[1] = (m_windowHeight - y) * 2.0 / m_windowHeight - 1.0;
        currPos[2] = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);

        currPos.normalize();
        lastPos.normalize();

        hduVector3Dd rotateVec = lastPos.crossProduct(currPos);
        
        double rotateAngle = asin(rotateVec.magnitude());
        if (!hduIsEqual(rotateAngle, 0.0, DBL_EPSILON))
        {
            CHapticViewerDoc* pDoc = GetDocument();
            Mesh* pObj = pDoc->getObj();
            
            hduMatrix deltaRotation = hduMatrix::createRotation(
                rotateVec, rotateAngle);
            m_cameraRotation.multRight(deltaRotation);
        
            updateGLCamera();
        }
    }
    if (m_isTranslatingCamera)
    {
        m_cameraTranslationX += 10 * double(x - m_lastMouseX)/m_windowWidth;
        m_cameraTranslationY -= 10 * double(y - m_lastMouseY)/m_windowWidth;

        updateGLCamera();
    }
    else if (m_isScalingCamera)
    {
        float y1 = m_windowHeight - m_lastMouseY;
        float y2 = m_windowHeight - y;

        m_cameraScale *= 1 + (y1 - y2) / m_windowHeight;  

        updateGLCamera();
    }

    m_lastMouseX = x;
    m_lastMouseY = y;
}

void CHapticViewerView::initCameraTransform()
{
    m_cameraScale = 1.0;
    m_cameraTranslationX = 0;
    m_cameraTranslationY = 0;
    m_cameraRotation.makeIdentity();
}
