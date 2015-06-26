// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.
//
// WorldBuilderDoc.h : interface of the CWorldBuilderDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDBUILDERDOC_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_)
#define AFX_WORLDBUILDERDOC_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <HapticDoc.h>

#include <ghostGLManager.h>
#include <ghostGLPinchXform.h>
#include <ghostGLSyncCamera.h>
#include "HapticFrustum.h"

class CWorldBuilderFrame;
class ObjectState;
class CObjectProperties;

#include <list.h>

typedef list<ObjectState *> ObjectStateList;
typedef ObjectStateList::iterator ObjListIter;
typedef CTypedPtrList<CPtrList, gstTransform*> CShapeList;

// Change: derive from CHapticDoc, not CDocument
class CWorldBuilderDoc : public CHapticDoc
{
protected: // create from serialization only
    CWorldBuilderDoc();
    DECLARE_DYNCREATE(CWorldBuilderDoc)
    void    AddSelection(gstTransform *object);
    void    RemoveSelection(gstTransform *object);
    void    DeleteContents();
    BOOL    LoadSceneFile(CString pathName);
    void    SetSelection();
    void    FinishManipulation();
    void    StopManipulator();
    void    SetTransform(UINT nID);
    ULONG   FindFreeGroupID();
    void    InitHaptics(gstBoolean reset);
    void    InitGhostGL();
    void    InitFrustum();
    void    UpdateFrustum();
    void    startGrab();
    void    endGrab();
    gstBoolean isSceneRunning();
    void    Redraw();
    void    Reshape(int w, int h);
    void    SetHaptics(gstBoolean state);
    BOOL    QueryCursorPos(double* pX, double* pY, double* pZ);

    void    EnableServoLoop(BOOL enable);
    void    toggleCurrent();
    void    toggleSelection(ObjectState *state);
    void    handleSelectEvent(gstShape *shape);
    void    setChildrenChoose(gstSeparator *sep, BOOL choose);
    void    centerSeparator(gstSeparator *sep);
    void    centerSceneInWorkspace();
    void    scaleSceneInWorkspace();
    void    updateAll();
    void    UpdateState(ObjectState *state);
    ObjectState *makeCopy(gstTransform *object);
    gstShape *createShape(unsigned short type);

// Attributes
public:

// Operations
public:
    CObjectProperties*  GetProperties();
      // Sets all selected objects to have the given properties if id == 0,
      // otherwise only copies the property associated with the given control id
    void                SetProperties(CObjectProperties *properties, UINT id = 0);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWorldBuilderDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CWorldBuilderDoc();

// Generated message map functions
// XXX: should be protected, changed to public during transition
public:
    //{{AFX_MSG(CWorldBuilderDoc)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    afx_msg void OnSelectAll();
    afx_msg void OnNewPrimitive(UINT nID);
    afx_msg void OnSelect();
    afx_msg void OnSelectNext();
    afx_msg void OnSelectPrev();
    afx_msg void OnDeselect();
    afx_msg void OnCopy();
    afx_msg void OnDelete();
    afx_msg void OnGroup();
    afx_msg void OnNewGroup();
    afx_msg void OnDelGroup();
    afx_msg void OnTransform(UINT nID);
    afx_msg void OnGo();
    afx_msg void OnGrab();
    afx_msg void OnRestartServo();
    afx_msg void OnFileImport();
    afx_msg void OnScaleUp();
    afx_msg void OnScaleDown();
    afx_msg void OnGoHome();
    afx_msg void OnUpdateSelect(CCmdUI* pCmdUI);
    afx_msg void OnUpdateCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
    afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
    afx_msg void OnUpdateTransform(CCmdUI* pCmdUI);
    afx_msg void OnUpdateMaterialProperties(CCmdUI* pCmdUI);
    afx_msg void OnUpdateGo(CCmdUI* pCmdUI);
    afx_msg void OnUpdateGrab(CCmdUI* pCmdUI);
    afx_msg void OnUpdateRestartServo(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    // List with graphic objects
    CShapeList      m_selected;

    // GHOST system variables
    gstManipulator* m_gstManipulator;
    UINT            m_nTransformationMode;

    CList<ULONG, ULONG> m_freeGroupID;
    int                 m_nSelectedGroups;

    bool    m_grabEnabled;

private:
    ghostGLManager    *m_glmanager;
    ghostGLPinchXForm *m_pinchObj;
    ghostGLSyncCamera *m_camera;

    gstScene           *m_gstScene;
    gstSeparator       *m_gstRoot;
    gstSeparator       *m_gstHapticObjects;
    gstSeparator       *m_gstSelectedObjects;
    gstPHANToM         *m_gstPHANToM;
    gstSeparator       *m_gstPhantomSep;
    gstBoundaryCube    *m_gstWorkspace;
    HapticFrustum      *m_hapticFrustum;
    ObjectStateList     m_objList;
    ObjListIter         m_currentObj;
    BOOL                m_bGoMode;
    ULONG               m_maxGroupID;

    CWorldBuilderFrame *getFrame() { 
        CWorldBuilderFrame *frame = (CWorldBuilderFrame *)AfxGetMainWnd();
        ASSERT(frame);
        return frame;
    }
    ObjectState *getUserData(gstTransform *obj) {
        ObjectState *data = (ObjectState *)m_glmanager->getUserData(obj);
        ASSERT(data);
        return data;
    }
};

void selectCallBack(gstTransform *object, void *eventData, void *param);
void graphicsCallBack(gstTransform *object, void *eventData, void *param);
void eventCallBack(gstTransform *object, void *eventData, void *param);
void frustum_event_callback(gstTransform *node, void *cbData, void *userData);

#define PHANTOM_NAME "Default PHANToM"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDBUILDERDOC_H__C950C8FA_B9EC_11D1_B8C5_000000000000__INCLUDED_)
