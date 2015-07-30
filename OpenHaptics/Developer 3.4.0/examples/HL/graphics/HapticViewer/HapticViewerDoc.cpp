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

  MFC CDocument derived document class for haptic viewer.

******************************************************************************/

#include "stdafx.h"
#include <assert.h>

#include "HapticViewer.h"
#include "HapticViewerDoc.h"

#include "Mesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerDoc

IMPLEMENT_DYNCREATE(CHapticViewerDoc, CDocument)

    BEGIN_MESSAGE_MAP(CHapticViewerDoc, CDocument)
    //{{AFX_MSG_MAP(CHapticViewerDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerDoc construction/destruction

void HLCALLBACK buttonDownClientThreadCallback(HLenum event, HLuint object, 
                                               HLenum thread, HLcache *cache, 
                                               void *userdata);
void HLCALLBACK buttonUpClientThreadCallback(HLenum event, HLuint object, 
                                             HLenum thread, HLcache *cache, 
                                             void *userdata);

CHapticViewerDoc::CHapticViewerDoc()
    : m_pObj(0), m_pauseDraw(false)
{
    // TODO: add one-time construction code here

}

CHapticViewerDoc::~CHapticViewerDoc()
{
    if (m_pObj)
        delete m_pObj;
}

BOOL CHapticViewerDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}

BOOL CHapticViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
    {
        return FALSE;
    }

    Mesh* newObj = new Mesh();

    try
    {
        newObj->load(lpszPathName);
    }

    catch (const char* s)
    {
        delete newObj;
        
        // toggle m_pauseDraw around message box to
        // avoid problem with View obscuring the
        // message box
        m_pauseDraw = true;
        AfxMessageBox(s);
        m_pauseDraw = false;

        UpdateAllViews(NULL);
        return FALSE;
    }

    m_pauseDraw = false;

    if (m_pObj)
    {
        // remove callbacks for existing shape (if one exists)
        hlRemoveEventCallback(HL_EVENT_1BUTTONDOWN, 
                              m_pObj->shapeId,
                              HL_CLIENT_THREAD, 
                              buttonDownClientThreadCallback);

        hlRemoveEventCallback(HL_EVENT_1BUTTONUP, 
                              HL_OBJECT_ANY, HL_CLIENT_THREAD, 
                              buttonUpClientThreadCallback);

        hlDeleteShapes(m_pObj->shapeId, 1);

        delete m_pObj;
        m_pObj = 0;
    }

    m_pObj = newObj;

    m_pObj->shapeId = hlGenShapes(1);
    m_pObj->transform = hduMatrix::createTranslation(0,0,0);
    m_pObj->isDragging = false;

    hlAddEventCallback(HL_EVENT_1BUTTONDOWN, 
                       m_pObj->shapeId,
                       HL_CLIENT_THREAD, 
                       buttonDownClientThreadCallback,
                       this);

    hlAddEventCallback(HL_EVENT_1BUTTONUP, HL_OBJECT_ANY, 
                       HL_CLIENT_THREAD, 
                       buttonUpClientThreadCallback, this);

    UpdateAllViews(NULL);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerDoc serialization

void CHapticViewerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerDoc diagnostics

#ifdef _DEBUG
void CHapticViewerDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CHapticViewerDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHapticViewerDoc commands

/******************************************************************************
Event callback triggered when styus button is depressed
and touching one of the draggable objects.  This callback is always
called in client thread.
******************************************************************************/
void HLCALLBACK CHapticViewerDoc::buttonDownClientThreadCallback(
    HLenum event, HLuint object, 
    HLenum thread, HLcache *cache, 
    void *userdata)
{
    assert(object != HL_OBJECT_ANY);

    CHapticViewerDoc *pThis = static_cast<CHapticViewerDoc*>(userdata);
    Mesh* pObj = pThis->getObj();
    
    // store off proxy position so we can compute how much it moves each
    // frame which is how much the drag object should also move
    hlGetDoublev(HL_PROXY_POSITION, pObj->startDragProxyPos);
    hlGetDoublev(HL_PROXY_ROTATION, pObj->startDragProxyRot);

    // store off initial position and orientation of drag object
    pObj->startDragObjTransform = pObj->transform;
    pObj->isDragging = true;
}

/******************************************************************************
Event callback triggered when styus button is depressed
and touching one of the draggable objects.
******************************************************************************/
void HLCALLBACK CHapticViewerDoc::buttonUpClientThreadCallback(
    HLenum event, HLuint object, 
    HLenum thread, HLcache *cache, 
    void *userdata)
{
    CHapticViewerDoc *pThis = static_cast<CHapticViewerDoc*>(userdata);
    Mesh* pObj = pThis->getObj();

    pObj->isDragging = false;
}
