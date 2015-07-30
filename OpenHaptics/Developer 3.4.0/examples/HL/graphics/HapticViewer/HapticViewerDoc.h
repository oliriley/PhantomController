/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  HapticViewerDoc.h

Description: 

  MFC CDocument derived document class for haptic viewer.

******************************************************************************/

#if !defined(AFX_HAPTICVIEWERDOC_H__AF5EEA0B_A057_11D4_A8D5_000086513E90__INCLUDED_)
#define AFX_HAPTICVIEWERDOC_H__AF5EEA0B_A057_11D4_A8D5_000086513E90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh.h"
#include <HDU/hduVector.h>

// struct representing one of the shapes in the scene
// that can be felt, touched and drawn
class CHapticViewerDoc : public CDocument
{
protected: // create from serialization only
    CHapticViewerDoc();
    DECLARE_DYNCREATE(CHapticViewerDoc)

// Attributes
        public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHapticViewerDoc)
public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CHapticViewerDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    static void HLCALLBACK buttonDownClientThreadCallback(
        HLenum event, HLuint object, 
        HLenum thread, HLcache *cache, 
        void *userdata);

    static void HLCALLBACK buttonUpClientThreadCallback(
        HLenum event, HLuint object, 
        HLenum thread, HLcache *cache, 
        void *userdata);

    Mesh* getObj() const { return m_pObj; }

public:
    bool m_pauseDraw;

protected:
    Mesh* m_pObj;

// Generated message map functions
protected:
    //{{AFX_MSG(CHapticViewerDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HAPTICVIEWERDOC_H__AF5EEA0B_A057_11D4_A8D5_000086513E90__INCLUDED_)
