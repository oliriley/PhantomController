// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

// WorldBuilderDoc.cpp : implementation of the CWorldBuilderDoc class
//

#include "gstAfx.h"
#include "WorldBuilderApp.h"
#include "WorldBuilderDoc.h"
#include "WorldBuilderFrame.h"
#include "WorldBuilderView.h"

#include "gstPHANToMSelect.h"

#include <algo.h>

#include <gstSpew.h> // XXX

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderDoc

// Change: derive from CHapticDoc, not CDocument
IMPLEMENT_DYNCREATE(CWorldBuilderDoc, CHapticDoc)

// Change: derive from CHapticDoc, not CDocument
BEGIN_MESSAGE_MAP(CWorldBuilderDoc, CHapticDoc)
    //{{AFX_MSG_MAP(CWorldBuilderDoc)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    ON_COMMAND(ID_EDIT_SELECTALL, OnSelectAll)
    ON_COMMAND_RANGE(ID_NEW_CUBE, ID_NEW_TORUS, OnNewPrimitive)
    ON_COMMAND(ID_MAIN_SELECT, OnSelect)
    ON_COMMAND(ID_SELECT_NEXT, OnSelectNext)
    ON_COMMAND(ID_SELECT_PREV, OnSelectPrev)
    ON_COMMAND(ID_DESELECT, OnDeselect)
    ON_COMMAND(ID_MAIN_COPY, OnCopy)
    ON_COMMAND(ID_DEL_OBJECT, OnDelete)
    ON_COMMAND(ID_MAIN_GROUP, OnGroup)
    ON_COMMAND(ID_NEW_GROUP, OnNewGroup)    
    ON_COMMAND(ID_DEL_GROUP, OnDelGroup)
    ON_COMMAND_RANGE(ID_TRANSFORM_MOVE, ID_TRANSFORM_SCALE, OnTransform)
    ON_COMMAND(ID_MAIN_GO, OnGo)
    ON_COMMAND(ID_GRAB, OnGrab)
    ON_COMMAND(IDR_RESTARTSERVO, OnRestartServo)
    ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
    ON_COMMAND(ID_SCALE_UP, OnScaleUp)
    ON_COMMAND(ID_SCALE_DOWN, OnScaleDown)
    ON_COMMAND(ID_GO_HOME, OnGoHome)
    ON_UPDATE_COMMAND_UI(ID_MAIN_SELECT, OnUpdateSelect)
    ON_UPDATE_COMMAND_UI(ID_MAIN_COPY, OnUpdateCopy)
    ON_UPDATE_COMMAND_UI(ID_DEL_OBJECT, OnUpdateDelete)
    ON_UPDATE_COMMAND_UI(ID_MAIN_GROUP, OnUpdateGroup)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TRANSFORM_MOVE, ID_TRANSFORM_SCALE, OnUpdateTransform)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MATERIAL_GETPROPERTIES, ID_MATERIAL_SETPROPERTIES, OnUpdateMaterialProperties)
    ON_UPDATE_COMMAND_UI(ID_MAIN_GO, OnUpdateGo)
    ON_UPDATE_COMMAND_UI(ID_GRAB, OnUpdateGrab)
    ON_UPDATE_COMMAND_UI(IDR_RESTARTSERVO, OnUpdateRestartServo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderDoc construction/destruction

CWorldBuilderDoc::CWorldBuilderDoc()
{
    m_gstManipulator = NULL;
    m_nTransformationMode = ID_MAIN_SELECT;

    m_maxGroupID = 0;
    m_nSelectedGroups = 0;
    m_bGoMode = FALSE;    
    m_glmanager = NULL;
    m_pinchObj;
    m_camera;

    m_gstScene = NULL;
    m_gstRoot = NULL;
    m_gstHapticObjects = NULL;
    m_gstSelectedObjects = NULL;
    m_gstPHANToM = NULL;
    m_gstPhantomSep = NULL;
    m_gstWorkspace = NULL;
    m_hapticFrustum = NULL;

    m_glmanager = NULL;
    m_camera = NULL;
    m_pinchObj = NULL;

    m_grabEnabled = false;
}

CWorldBuilderDoc::~CWorldBuilderDoc()
{
    if (! m_gstScene) {
        // Nothing to do
        return;
    }

    // Only stop servo loop if it is still running
    if (! m_gstScene->getDoneServoLoop())   
        m_gstScene->stopServoLoop();
    m_gstPHANToM->stopEffect();
    m_gstPHANToM->setEffect(NULL);
    SetTransform(ID_MAIN_SELECT);

    // Clear all objects
    DeleteContents();

    delete m_glmanager;
    m_glmanager = NULL;
    delete m_camera;
    m_camera = NULL;
    delete m_gstScene;
    m_gstScene = NULL;

    if (!m_grabEnabled) {
        // if grab is enabled, the pinchObj will have been added to the
        // glmanger and so will be delete when it was delete
        delete m_pinchObj;
        m_pinchObj = NULL;
    }
}

void CWorldBuilderDoc::AddSelection(gstTransform *object)
{
    m_selected.AddTail(object);
}

void CWorldBuilderDoc::RemoveSelection(gstTransform *object)
{
    POSITION pos = m_selected.Find(object);
    if (pos != NULL)
        m_selected.RemoveAt(pos);
}

void CWorldBuilderDoc::DeleteContents()
{
    CHapticDoc::DeleteContents();
    
    if (!m_gstScene) {
        return;
    }

    m_nSelectedGroups = 0;    
    m_nTransformationMode = ID_MAIN_SELECT;
    SetTransform(ID_MAIN_SELECT);
    OnGoHome();

    EnableServoLoop(FALSE);

    m_selected.RemoveAll();
    m_objList.clear();

    m_glmanager->setGraphicsOn(FALSE);
    // free up ghostGL resources (display lists, etc)
    m_glmanager->clearAll();
    // but we want the workspace and haptic frustum to remain invisible
    m_glmanager->setVisible(m_gstWorkspace, FALSE);
    m_glmanager->setVisible(m_hapticFrustum, FALSE);

    // the gstSeparator destructor will delete descendent nodes
    m_gstRoot->removeChild(m_gstHapticObjects);
    delete m_gstHapticObjects;

    // Add separator for all haptic objects in the scene
    m_gstHapticObjects = new gstSeparator;
    m_gstHapticObjects->setName(gstNodeName("Haptic Scene"));
    m_gstRoot->addChild(m_gstHapticObjects);
    
    // Add separator for all select objects
    m_gstSelectedObjects = new gstSeparator;
    m_gstSelectedObjects->setName(gstNodeName("Selected Objects"));
    m_gstHapticObjects->addChild(m_gstSelectedObjects);
}

CObjectProperties* CWorldBuilderDoc::GetProperties()
{
    if (m_bGoMode || m_selected.IsEmpty())
        return NULL;

    ObjectState *state;
    POSITION selectPos = m_selected.GetHeadPosition();
    gstTransform *object = m_selected.GetAt(selectPos);

    if (object->isOfType(gstSeparator::getClassTypeId())) {
        gstSeparator *group = (gstSeparator*)object;        
        state = getUserData(group->getChild(0));
    } else
        state = getUserData(object);

    return &state->properties;
}

  // Sets all selected objects to have the given properties if id == 0,
  // otherwise only copies the property associated with the given control id
void CWorldBuilderDoc::SetProperties(CObjectProperties *properties, UINT id)
{
    if (m_bGoMode)
        return;

    ObjectState *state;
    POSITION selectPos = m_selected.GetHeadPosition();
    while (selectPos != NULL) {
        gstTransform *object = m_selected.GetNext(selectPos);
        if (object->isOfType(gstSeparator::getClassTypeId())) {
            gstSeparator *group = (gstSeparator*)object;
            for (int j = 0; j < group->getNumChildren(); j++) {
                object = group->getChild(j);
                state = getUserData(object);
                state->properties.CopyFrom(properties, id);
                UpdateState(state);
            }
        } else if (object->isOfType(gstShape::getClassTypeId())) {            
            state = getUserData(object);
            state->properties.CopyFrom(properties, id);
            UpdateState(state);
        }
    }

    SetModifiedFlag(TRUE);
}

BOOL CWorldBuilderDoc::LoadSceneFile(CString pathName)
{
    CFileException fe;
    const int flags = CFile::modeRead | CFile::shareDenyWrite;
    CFile *pSceneFile = GetFile(pathName, flags, &fe);

    if (pSceneFile == NULL) {
        ReportSaveLoadException(pathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        return FALSE;
    }

    SetModifiedFlag(TRUE);
    CArchive archive(pSceneFile, CArchive::load | CArchive::bNoFlushOnDelete);
    archive.m_pDocument = this;
    archive.m_bForceFlat = FALSE;

    TRY
    {
        CWaitCursor wait;
        if (pSceneFile->GetLength() != 0)
            Serialize(archive);
        archive.Close();
        ReleaseFile(pSceneFile, FALSE);
    }
    CATCH_ALL(e)
    {
        ReleaseFile(pSceneFile, TRUE);
        DeleteContents();

        TRY
        {
            ReportSaveLoadException(pathName, e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        }
        END_TRY
        DELETE_EXCEPTION(e);
        return FALSE;
    }
    END_CATCH_ALL

    return TRUE;
}


void CWorldBuilderDoc::EnableServoLoop(BOOL enable)
{
    if (!m_gstScene) {
        return;
    }

    if (enable) {
        // For debugging turn off safety
        //m_gstScene->setSafety(FALSE);
        //m_gstScene->setQuitOnDevFault(FALSE);
        if (! m_gstScene->startServoLoop()) {
            exit(-1);
        }
    } else if (!m_gstScene->getDoneServoLoop()) {
        m_gstScene->stopServoLoop();            
    }

    POSITION pos = GetFirstViewPosition();
    // check if we have a view, on exit we might not
    if (pos) {
        CWorldBuilderView *view = (CWorldBuilderView *)GetNextView(pos);
        view->setSuspended(!enable);
    }
}

void CWorldBuilderDoc::FinishManipulation()
{
    StopManipulator();

    bool servolooprunning = !m_gstScene->getDoneServoLoop();
    if (servolooprunning) {
        // gstScene::lock() is newly implemented in GHOST 3.0
        // it locks the scene such that the servoloop still makes
        // force calculations for objects the PHANTOM is currently
        // contacting, but the servo loop does not access the scene
        // graph so changes can be made to it.
        m_gstScene->lock();
    }
    
    gstPoint rot = m_gstSelectedObjects->getRotationAngles();
    gstPoint scale = m_gstSelectedObjects->getScaleFactor();
    for (int i = m_gstSelectedObjects->getNumChildren() - 1; i >= 0; i--) {
        gstTransform *child = m_gstSelectedObjects->getChild(i);
        if (child->isOfType(gstSeparator::getClassTypeId())) {
            gstSeparator* group = (gstSeparator*) child;
            for (int j = 0; j < group->getNumChildren(); j++) {
                gstTransform *object = group->getChild(j);
                gstPoint rot2 = object->getRotationAngles();
                object->setTranslation(m_gstHapticObjects->fromWorld(object->getPosition_WC()));
                object->setRotation(gstVector(1.0, 0.0, 0.0), rot[0]);
                object->rotate(gstVector(0.0, 1.0, 0.0), rot[1]);
                object->rotate(gstVector(0.0, 0.0, 1.0), rot[2]);
                object->rotate(gstVector(1.0, 0.0, 0.0), rot2[0]);
                object->rotate(gstVector(0.0, 1.0, 0.0), rot2[1]);
                object->rotate(gstVector(0.0, 0.0, 1.0), rot2[2]);
                object->scale(scale[0]);
            }
            group->setTranslation(0.0, 0.0, 0.0);
            group->setRotation(gstVector(0.0, 1.0, 0.0), 0);
            group->setScale(1.0);
        } else {
            gstPoint rot2 = child->getRotationAngles();
            child->setTranslation(m_gstHapticObjects->fromWorld(child->getPosition_WC()));
            child->setRotation(gstVector(1.0, 0.0, 0.0), rot[0]);
            child->rotate(gstVector(0.0, 1.0, 0.0), rot[1]);
            child->rotate(gstVector(0.0, 0.0, 1.0), rot[2]);
            child->rotate(gstVector(1.0, 0.0, 0.0), rot2[0]);
            child->rotate(gstVector(0.0, 1.0, 0.0), rot2[1]);
            child->rotate(gstVector(0.0, 0.0, 1.0), rot2[2]);
            child->scale(scale[0]);
        }
        m_gstSelectedObjects->removeChild(child);
        m_gstHapticObjects->addChild(child);
    }

    ASSERT(m_gstSelectedObjects->getNumChildren() == 0);
    // reset transform
    m_gstSelectedObjects->setTransformMatrix(gstTransformMatrix());

    if (servolooprunning) {
        m_gstScene->unlock();
    }
    SetTransform(m_nTransformationMode);
}

void CWorldBuilderDoc::StopManipulator()
{
    if (m_gstPHANToM->getManipulator()) {
        m_gstManipulator->stop();
        m_gstPHANToM->setManipulator(NULL);
        delete m_gstManipulator;
        m_gstManipulator = NULL;
    }
}

void CWorldBuilderDoc::SetTransform(UINT nID)
{
    StopManipulator();
    m_nTransformationMode = nID;

    if ((nID >= ID_TRANSFORM_MOVE) && (nID <= ID_TRANSFORM_SCALE) &&
        m_gstSelectedObjects->getNumChildren() > 0) {

        SetModifiedFlag(TRUE);

        if (nID == ID_TRANSFORM_MOVE)
            m_gstManipulator = new gstTranslateManipulator;
        else if (nID == ID_TRANSFORM_ROTATE)
            m_gstManipulator = new gstRotateManipulator;
        else
            m_gstManipulator = new gstScaleManipulator;

        if ((m_gstSelectedObjects->getNumChildren() == 1) && m_nSelectedGroups == 0) {
              // we add the object itself this so SCALE can to non-uniform
              // scales on a single object, which it can't do on a separator
            m_gstManipulator->setNode(m_gstSelectedObjects->getChild(0));
        } else {
            m_gstManipulator->setNode(m_gstSelectedObjects);
        }

        if ((m_gstSelectedObjects->getNumChildren() == 0)) {
            m_gstManipulator->setNode(m_gstSelectedObjects);
        }
        m_gstPHANToM->setManipulator(m_gstManipulator);
        m_gstManipulator->setUseStylusSwitch(TRUE);
    }
}


void CWorldBuilderDoc::SetSelection()
{
    POSITION selectPos = m_selected.GetHeadPosition();

    while (selectPos != NULL) {
        gstTransform *object = m_selected.GetNext(selectPos);
        m_gstHapticObjects->removeChild(object);
        m_gstSelectedObjects->addChild(object);
    }

    centerSeparator(m_gstSelectedObjects);
    
    SetTransform(m_nTransformationMode);
    
    getFrame()->updateToolbars();
}

ULONG CWorldBuilderDoc::FindFreeGroupID()
{
    if (m_freeGroupID.IsEmpty())
        return ++m_maxGroupID;

    ULONG freeID = m_freeGroupID.GetHead();
    m_freeGroupID.RemoveHead();
    return freeID;
}

// toggle the selection the current object in the m_objList
void CWorldBuilderDoc::toggleCurrent()
{
    if (!m_objList.empty()) {
        toggleSelection(*m_currentObj);
    }
}

void CWorldBuilderDoc::toggleSelection(ObjectState *state)
{
    ASSERT(state);
    gstTransform *object = state->hapticObject;
    ASSERT(object);
    
    BOOL choose = !(state->choose);

    state->choose = choose;
    UpdateState(state);

    FinishManipulation();

    if (choose) {
        gstTransform *toadd = object;
        if (state->groupID != 0) {
              // Part of a group, adjust count
            m_nSelectedGroups++;
              // Add the parent instead
            toadd = object->getParent();
              // Set the child nodes
            setChildrenChoose((gstSeparator *)toadd, choose);            
        }
        AddSelection(toadd);
        m_currentObj = find(m_objList.begin(), m_objList.end(), state);
    } else {
        gstTransform *toremove = object;
        if (state->groupID != 0) {
              // Part of a group, adjust count                    
            m_nSelectedGroups--;
              // Remove the parent instead
            toremove = object->getParent();
              // Set the child nodes
            setChildrenChoose((gstSeparator *)toremove, choose);
        }

        RemoveSelection(toremove);
    }

      // adjust for the new selection
    SetSelection();
   
      // Get the properties for this selection
    getFrame()->getProperties();
}

// SELECT events are generated by gstPHANToMSelect.  The event
// callback for the PHANTOM is selectCallBack() which calls
// handleSelectEvent().  This function impements some "standard"
// selection semantics. Holding CTRL when clicking selects multiple
// objects or allows a single object to be selected, without holding
// control selecting a new object unselects any selected objects
void CWorldBuilderDoc::handleSelectEvent(gstShape *shape)
{
    ObjectState* state = (ObjectState *)m_glmanager->getUserData(shape);

      // ignore if in "go mode" or if grabbing
    if (!state || m_bGoMode || m_grabEnabled) {
        return;
    }

    static const int KEY_DOWN_NOW = (1 << 16);
    bool control = (GetAsyncKeyState(VK_CONTROL) & KEY_DOWN_NOW) != 0;
    bool multiple = (m_selected.GetCount() > 1);
    bool notself = (m_selected.GetCount() == 1) && !state->choose;

      // We don't allow us to unselect ourselves if we are
      // doing a transform (tranlate/rotate/scale) and we are only
      // one selected
    if (!multiple && state->choose &&
        m_nTransformationMode != ID_MAIN_SELECT) {

        return;
    }

      // Desired selection behavior:
      //    If CTRL is down, always just toggle current
      //    If CTRL not down, unselect all and then toggle current
      //        (unless if self is only one selected just toggle self)
    if (!control && (multiple || notself)) {
        OnDeselect();
    }

    toggleSelection(state);
}


void CWorldBuilderDoc::setChildrenChoose(gstSeparator *sep, BOOL choose)
{
    ASSERT(sep);

    for (int i = 0; i < sep->getNumChildren(); i++) {
        gstTransform *child = sep->getChild(i);
        ObjectState *state = getUserData(child);
        state->choose = choose;
        UpdateState(state);
    }
}

void CWorldBuilderDoc::centerSeparator(gstSeparator *sep)
{
    int numchildren = sep->getNumChildren();
    gstPoint center;
    int i;

    ASSERT(numchildren >= 0);

    // no need to center if no children
    if (numchildren == 0) {
        return;
    }

      // find the center
    for (i = 0; i < numchildren; i++) {
        gstTransform *child = sep->getChild(i);
        // recursion
        if (child->isOfType(gstSeparator::getClassTypeId())) {
            centerSeparator((gstSeparator *)child);
        }
        center += child->getPosition();
    }

      // move the separator to the center
    center = center / numchildren;
    sep->setPosition(center);

      // adjust the children's position by the center
    for (i = 0; i < numchildren; i++) {
        gstTransform *child = sep->getChild(i);
        gstPoint childpos = child->getPosition();
        child->setPosition(childpos - center);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderDoc serialization

void CWorldBuilderDoc::Serialize(CArchive& ar)
{
    gstPoint pos, rot, scale;
    ULONG nCount;
    gstTransform *object;
    gstSeparator *group;

    OnDeselect();

    CString sceneMagic;
    sceneMagic.LoadString(ID_MAGIC_SCENE);

    CWaitCursor wait;

    ObjListIter iter = m_objList.begin();
    
    if (ar.IsStoring()) {
        // Saving/Writing file
        ar << sceneMagic;       // Write the header

        nCount = m_objList.size();
        ar << nCount;

        while (iter != m_objList.end()) {
            ObjectState *state = *iter;
            ASSERT(state);
            object = state->hapticObject;
            pos = object->getPosition();
            rot = object->getRotationAngles();
            scale = object->getScaleFactor();
            ar << state;
            ar << pos[0] << pos[1] << pos[2];
            ar << rot[0] << rot[1] << rot[2];
            ar << scale[0] << scale[1] << scale[2];

            nCount--;
            ++iter;
        }
        ASSERT(nCount == 0);

    } else {
        // Loading/Reading file
        ULONG lastGroupID, newGroupID;
        lastGroupID = newGroupID = 0;

        CString magic;
        ar >> magic; // Read the file identifier
        if (magic.Compare(sceneMagic))
            AfxThrowArchiveException(CArchiveException::generic);

        ar >> nCount;

        ObjectState *state;

        while (nCount-- > 0) {
            state = new ObjectState;
            ar >> state;
            ar >> pos[0] >> pos[1] >> pos[2];
            ar >> rot[0] >> rot[1] >> rot[2];
            ar >> scale[0] >> scale[1] >> scale[2];
            object = createShape(state->primitiveType);
            object->setTranslation(pos);
            object->setRotation(gstVector(1.0, 0.0, 0.0), rot[0]);
            object->rotate(gstVector(0.0, 1.0, 0.0), rot[1]);
            object->rotate(gstVector(0.0, 0.0, 1.0), rot[2]);
            object->setScale(scale);
            m_glmanager->setGraphicsCallback(object, NULL, state);
            state->hapticObject = object;
            UpdateState(state);
        
            if (state->groupID == 0) {
                m_gstHapticObjects->addChild(object);
                lastGroupID = 0;
            } else {
                if (state->groupID != lastGroupID) {
                    lastGroupID = state->groupID;
                    newGroupID = FindFreeGroupID();
                    group = new gstSeparator();
                    m_gstHapticObjects->addChild(group);
                }
                state->groupID = newGroupID;
                group->addChild(object);
            }
            m_objList.push_back(state);
        }

        centerSceneInWorkspace();
        scaleSceneInWorkspace();
        EnableServoLoop(TRUE);
        m_glmanager->setGraphicsOn(TRUE);
    }
}

void CWorldBuilderDoc::centerSceneInWorkspace()
{
    m_gstHapticObjects->setTranslation(m_gstWorkspace->getPosition());
}

void CWorldBuilderDoc::scaleSceneInWorkspace()
{
    const double FRACTION_OF_WORKSPACE = 0.90;
    double sceneR = m_gstHapticObjects->getBoundingRadiusOfChildren();
    if (sceneR > 0) {
        double x = m_gstWorkspace->getWidth() / 2;
        double y = m_gstWorkspace->getHeight() / 2;
        double z = m_gstWorkspace->getLength() / 2;
        double workspaceR = sqrt(x * x + y * y + z * z);    
        double scale = (workspaceR * FRACTION_OF_WORKSPACE)/sceneR;
        // only scale down not up, we want the scene to fit within the view
        // initially, but we don't want to zoom in on small scenes
        if (scale < 1) { 
            m_gstHapticObjects->setScale(scale);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CWorldBuilderDoc commands

BOOL CWorldBuilderDoc::OnNewDocument()
{        
    if (! CDocument::OnNewDocument()) {
        return FALSE;
    }

    if (m_gstScene) {
        m_glmanager->setGraphicsOn(TRUE);
        EnableServoLoop(TRUE);
    }

    return TRUE;
}

void CWorldBuilderDoc::OnNewPrimitive(UINT nID)
{
    if (m_bGoMode) {
        return;
    }

    SetModifiedFlag(TRUE);
    OnDeselect();

    ObjectState* state = new ObjectState();
    state->choose = TRUE;

    gstShape *object;
    switch (nID) {
        case ID_NEW_CUBE:
            state->primitiveType = ObjectState::CUBE;
            break;
        case ID_NEW_SPHERE:
            state->primitiveType = ObjectState::SPHERE;
            break;
        case ID_NEW_CONE:
            state->primitiveType = ObjectState::CONE;
            break;
        case ID_NEW_CYLINDER:
            state->primitiveType = ObjectState::CYLINDER;
            break;
        case ID_NEW_TORUS:
            state->primitiveType = ObjectState::TORUS;            
            break;
    }
    object = createShape(state->primitiveType);

    state->hapticObject = object;
    m_glmanager->setGraphicsCallback(object, NULL, state);
    
    // some fixed initial size
    object->setScale(15, 15, 15);
    // center in workspace
    //object->setTranslation(m_gstWorkspace->getPosition());

    m_objList.push_back(state);
    m_currentObj = find(m_objList.begin(), m_objList.end(), state);

    m_gstHapticObjects->addChild(object);
    AddSelection(object);
    UpdateState(state);

    SetSelection();
}

void CWorldBuilderDoc::OnSelect()
{
    OnTransform(ID_MAIN_SELECT);
}

void CWorldBuilderDoc::OnSelectNext()
{
    if (m_objList.size() < 2) {
        return;
    }

    while (m_currentObj != m_objList.end() && (*m_currentObj)->choose) {
        ++m_currentObj;
    }

    if (m_currentObj == m_objList.end()) {
        m_currentObj = m_objList.begin();
    }

    OnDeselect();
    toggleSelection(*m_currentObj);
}

void CWorldBuilderDoc::OnSelectPrev()
{
    if (m_objList.size() < 2) {
        return;
    }

    while (m_currentObj != m_objList.begin() && (*m_currentObj)->choose) {
       --m_currentObj;
    }

    if (m_currentObj == m_objList.begin() && (*m_currentObj)->choose) {
        m_currentObj = m_objList.end();
        --m_currentObj;
    }

    OnDeselect();
    toggleSelection(*m_currentObj);
}

void CWorldBuilderDoc::OnSelectAll()
{    
    OnDeselect();
    SetSelection();

    int lastgroupID = 0;
    // iterate through all objects
    for (m_currentObj = m_objList.begin();
         m_currentObj != m_objList.end();
         ++m_currentObj) {

        // set each one to be choosen
        ObjectState *state = *m_currentObj;
        state->choose = TRUE;
        UpdateState(state);

        // if this object is in a group
        if (state->groupID != 0) {
            // and if this is the first object in that group
            if (state->groupID != lastgroupID) {
                // then increment group count and add object's parent
                m_nSelectedGroups++;
                AddSelection(state->hapticObject->getParent());
                lastgroupID = state->groupID;
            }
        } else {
            // just add object
            lastgroupID = 0;
            AddSelection(state->hapticObject);
        }
    }

    SetSelection();
}

void CWorldBuilderDoc::OnDeselect()
{
    ObjectState *state;
    
    FinishManipulation();
    m_nSelectedGroups = 0;

    while (! m_selected.IsEmpty()) {
        gstTransform *object = m_selected.RemoveTail();
        if (object->isOfType(gstSeparator::getClassTypeId())) {
            gstSeparator *group = (gstSeparator*)object;
            for (int j = 0; j < group->getNumChildren(); j++) {
                state = getUserData(group->getChild(j));
                state->choose = FALSE;
                UpdateState(state);
            }
        } else {
            state = getUserData(object);
            state->choose = FALSE;
            UpdateState(state);
        }
    }
}


ObjectState *CWorldBuilderDoc::makeCopy(gstTransform *object)
{
    ASSERT(object);
    ObjectState *state = getUserData(object);
    ObjectState *newObjectState = new ObjectState();
    newObjectState->primitiveType = state->primitiveType;
    newObjectState->properties.CopyFrom(&state->properties);
    gstShape *copy = createShape(newObjectState->primitiveType);

    gstPoint pos = object->getPosition();
    gstPoint rot = object->getRotationAngles();
    gstPoint scale = object->getScaleFactor();
    copy->setTranslation(pos);
    copy->setRotation(gstVector(1.0, 0.0, 0.0), rot[0]);
    copy->rotate(gstVector(0.0, 1.0, 0.0), rot[1]);
    copy->rotate(gstVector(0.0, 0.0, 1.0), rot[2]);
    copy->setScale(scale);

    m_glmanager->setGraphicsCallback(copy, NULL, newObjectState);
    newObjectState->hapticObject = copy;
    UpdateState(newObjectState);
    m_objList.push_back(newObjectState);

    return newObjectState;
}

void CWorldBuilderDoc::OnCopy()
{
    FinishManipulation();

    if (! m_selected.IsEmpty()) {
        SetModifiedFlag(TRUE);
        POSITION selectPos= m_selected.GetHeadPosition();
        while (selectPos != NULL) {
            gstTransform *object = m_selected.GetNext(selectPos);
            if (object->isOfType(gstSeparator::getClassTypeId())) {
                gstSeparator *group = (gstSeparator*)object;
                gstSeparator *groupCopy = new gstSeparator();
                m_gstHapticObjects->addChild(groupCopy);
                ULONG   newGroupID = FindFreeGroupID();
                for (int j = 0; j < group->getNumChildren(); j++) {                    
                    object = group->getChild(j);
                    ObjectState *newObjectState = makeCopy(object);
                    newObjectState->groupID = newGroupID;
                    groupCopy->addChild(newObjectState->hapticObject);
                }
            } else {
                ObjectState *newObjectState = makeCopy(object);
                m_gstHapticObjects->addChild(newObjectState->hapticObject);
            }
        }
    }
}

void CWorldBuilderDoc::OnDelete()
{
    if (AfxMessageBox(ID_STATIC_DELETE, MB_YESNO | MB_DEFBUTTON2) == IDNO)
        return;

    SetModifiedFlag(TRUE);
    OnDelGroup();

    FinishManipulation();
    StopManipulator();

    m_nSelectedGroups = 0;
    m_selected.RemoveAll();

    bool servolooprunning = !m_gstScene->getDoneServoLoop();
    if (servolooprunning) {        
        EnableServoLoop(FALSE);
    }

    ObjListIter iter = m_objList.begin();
    while (iter != m_objList.end()) {
        ASSERT(*iter);
        if ((*iter)->choose) {
            // We remove the haptic object associated with this ObjectState
            // from the scene graph and delete it before deleting the state
            gstTransform *node = (*iter)->hapticObject;
            gstSeparator *parent = (gstSeparator *)node->getParent();
            parent->removeChild(node);
            delete node;
            delete *iter;
            iter = m_objList.erase(iter);
        } else {
            ++iter;
        }
    }

    if (servolooprunning) {
        EnableServoLoop(TRUE);
    }

    SetSelection();
}

void CWorldBuilderDoc::OnGroup()
{    
    if (m_nSelectedGroups == 0)
        OnNewGroup();
    else
        OnDelGroup();
}

  // Group all selected objects
void CWorldBuilderDoc::OnNewGroup()
{
      // we can only make a group of more than one object
    if (m_selected.GetCount() <= 1) {
        return;
    }

    SetModifiedFlag(TRUE);

    // Break up existing groups with OnDelGroup. We can merge groups
    // be we don't retain the original groups (no sub groups or hierarchy yet)
    OnDelGroup();

    FinishManipulation();
    
    ULONG newGroupID = FindFreeGroupID();
    m_nSelectedGroups = 1;
    gstSeparator *group = new gstSeparator;
    gstTransform *object;
    group->setName(gstNodeName("Group"));

    while (! m_selected.IsEmpty()) {
        object = m_selected.RemoveTail();
        m_gstHapticObjects->removeChild(object);
        group->addChild(object);
        ObjectState *state = getUserData(object);
        state->groupID = newGroupID;
        state->choose = TRUE;
        UpdateState(state);
    }
    AddSelection(group);
    m_gstSelectedObjects->addChild(group);
        // we changed m_gstSelectedObjects, so must call SetTransform
    SetTransform(m_nTransformationMode);

    // Put all elements of the group behind each other

    // Find the first element with new group ID
    ObjListIter iter = m_objList.begin();
    while ((*iter)->groupID != newGroupID) {
        ++iter;
        ASSERT(iter != m_objList.end());
    }
    // mark location of first element with new group ID
    m_currentObj = iter;

    // Subsequent elements with the same ID don't need to be moved
    while (iter != m_objList.end() && (*iter)->groupID == newGroupID) {
        ++iter;
    }

    // Then move all elements with the new group ID to near
    while (iter != m_objList.end()) {        
        if ((*iter)->groupID == newGroupID) {
            m_objList.insert(m_currentObj, *iter);
            iter = m_objList.erase(iter);
        } else {
            ++iter;
        }
    }

    getFrame()->updateToolbars();
}

void CWorldBuilderDoc::OnDelGroup()
{
    // Delete all selected groups, make all released objects to selected
    // Ignore ungrouped objects

    FinishManipulation();
    StopManipulator();    

    m_nSelectedGroups = 0;
    SetModifiedFlag(TRUE);
    gstSeparator *tempSeparator = new gstSeparator;

    POSITION selectPos = m_selected.GetTailPosition();
    POSITION removePos;
    while (selectPos != NULL) {
        removePos = selectPos;
        gstTransform *object = m_selected.GetPrev(selectPos);
        if (object->isOfType(gstSeparator::getClassTypeId())) {
            gstSeparator *group = (gstSeparator*) object;
            m_selected.RemoveAt(removePos);
            m_gstHapticObjects->removeChild(group);
            if (group->getNumChildren() > 0) {
                ObjectState *state = getUserData(group->getChild(0));
                ULONG delID = state->groupID;
                if (delID == m_maxGroupID)    
                    m_maxGroupID--;
                else
                    m_freeGroupID.AddHead(delID);
            }

            for (int j = group->getNumChildren() - 1; j >= 0; j--) {
                object = group->getChild(j);
                group->removeChild(object);
                // Since we're iterating through selectedObjects, it would cause problems
                // if we would add new objects to it, to solve this, new objects will be
                // added to another separator and copied later to selectedObjects
                tempSeparator->addChild(object);

                ObjectState *state = getUserData(object);
                state->groupID = 0;
                state->choose = TRUE;
                UpdateState(state);
            }
            delete group;
        }
    }
    // Add all objects that were temporarily saved in tempSeparator to selectedObjects
    for (int i = tempSeparator->getNumChildren() - 1; i >= 0; i--) {
        gstTransform *object = tempSeparator->getChild(i);
        tempSeparator->removeChild(object);
        AddSelection(object);
        m_gstHapticObjects->addChild(object);
    }

    SetSelection();

    delete tempSeparator;
    getFrame()->updateToolbars();
}

void CWorldBuilderDoc::OnTransform(UINT nID)
{
    AfxGetMainWnd()->GetNextDlgTabItem(AfxGetMainWnd())->SetFocus();
    SetTransform(m_nTransformationMode != nID ? nID : ID_MAIN_SELECT);
    getFrame()->updateToolbars();
}

void CWorldBuilderDoc::updateAll()
{
    for (ObjListIter iter = m_objList.begin();
         iter != m_objList.end();
         ++iter) {

        ASSERT(*iter);
        UpdateState(*iter);
    }
}

void CWorldBuilderDoc::OnGo()
{    
    if (m_bGoMode) {
        m_bGoMode = FALSE;
        updateAll();
    } else {
        FinishManipulation();
        StopManipulator();
        OnDeselect();
        m_bGoMode = TRUE;
        updateAll();
    }

    getFrame()->updateToolbars();
}

void CWorldBuilderDoc::OnGrab()
{
    if (m_grabEnabled) {
        endGrab();
    } else {
        startGrab();
    }

    getFrame()->updateToolbars();
}

void CWorldBuilderDoc::OnRestartServo()
{
    m_gstScene->startServoLoop();
}

void CWorldBuilderDoc::OnFileImport()
{
    CWorldBuilderApp*      pApp = (CWorldBuilderApp *)AfxGetApp();
    CDocTemplate*   docTemplate = pApp->GetAppTemplate();

    // Create a filter
    CString filter, temp, ext, sceneExt;
    docTemplate->GetDocString(temp, CDocTemplate::filterName);
    filter += temp; filter += _T("|");
    docTemplate->GetDocString(temp, CDocTemplate::filterExt);
    sceneExt = temp.Right(temp.GetLength() - 1);
    ext = _T("*"); ext += temp;
    filter += ext;

    CFileDialog importDialog(TRUE, ext, NULL,
                             OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
                             filter, NULL);

    if (importDialog.DoModal() == IDOK) {
        if (importDialog.GetFileExt() == sceneExt)
            LoadSceneFile(importDialog.GetPathName());
        SetModifiedFlag(TRUE);
    }   
}

void CWorldBuilderDoc::OnUpdateSelect(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nTransformationMode == ID_MAIN_SELECT);
}

void CWorldBuilderDoc::OnUpdateCopy(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_selected.GetCount() != 0);
}

void CWorldBuilderDoc::OnUpdateDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_selected.GetCount() != 0);
}

void CWorldBuilderDoc::OnUpdateGroup(CCmdUI* pCmdUI)
{
    if (m_selected.GetCount() == 0 ||
        (m_selected.GetCount() == 1 && m_nSelectedGroups == 0)) {
        pCmdUI->SetCheck(0);
        pCmdUI->Enable(FALSE);
    } else {
        pCmdUI->Enable(TRUE);
        if (m_nSelectedGroups == 0)
            pCmdUI->SetCheck(0);
        else if ((m_nSelectedGroups == 1) &&(m_selected.GetCount() == 1))
            pCmdUI->SetCheck(1);
        else
            pCmdUI->SetCheck(2);
    }
}

void CWorldBuilderDoc::OnUpdateTransform(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nTransformationMode == pCmdUI->m_nID);
}

void CWorldBuilderDoc::OnUpdateMaterialProperties(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(! m_selected.IsEmpty());
}

void CWorldBuilderDoc::OnUpdateGo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable((BOOL) m_gstScene);
    pCmdUI->SetCheck(m_bGoMode); 
}

void CWorldBuilderDoc::OnUpdateGrab(CCmdUI* pCmdUI)
{
    pCmdUI->Enable((BOOL) m_gstScene);
    pCmdUI->SetCheck(m_grabEnabled); 
}

void CWorldBuilderDoc::OnUpdateRestartServo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_gstScene && m_gstScene->getDoneServoLoop());
}

void CWorldBuilderDoc::OnScaleUp()
{
    m_gstHapticObjects->scale(1.1);
}

void CWorldBuilderDoc::OnScaleDown()
{
    m_gstHapticObjects->scale(1.0/1.1);
}

void CWorldBuilderDoc::OnGoHome()
{
    // return to original scale, set to one then scale in scene
    // so that if we've scale it smaller it will go back to original
    m_gstHapticObjects->setScale(1);
    scaleSceneInWorkspace();
    // return to home camera position by setting phantomSep's transform
    // to identity have having ghostGL sync to that
    m_camera->setSyncMode(ghostGLSyncCamera::SYNC_CAMERA_TO_WORKSPACE);
    m_gstPhantomSep->setTransformMatrix(gstTransformMatrix());
    m_camera->update();
    m_camera->setSyncMode(ghostGLSyncCamera::SYNC_WORKSPACE_TO_CAMERA);
    UpdateFrustum();
}

// select events are produced by gstPHANToMSelect
void selectCallBack(gstTransform *object, void *eventdata, void *userdata)
{    
    gstEvent *event = (gstEvent *)eventdata;    
    CWorldBuilderDoc *doc = (CWorldBuilderDoc *)userdata;
    ASSERT(event);
    ASSERT(doc);

    if (event->id == gstPHANToMSelect::SELECT) {
        gstShape *shape = (gstShape *)event->data1v;
        ASSERT(shape);
        doc->handleSelectEvent(shape);
    }
}


// Initial scene graph looks like this:
//                            
//                           / m_gstPHANToM
//            m_gstPhantomSep- m_hapticFrustum
//           /               \ m_gstWorkspace 
//  m_gstRoot
//           \                   
//            m_gstHapticObjects - m_gstSelectedObjects
//
//
void CWorldBuilderDoc::InitHaptics(gstBoolean reset)
{
    // Generate scene
    m_gstScene = new gstScene();
    m_gstRoot = new gstSeparator();
    m_gstRoot->setName(gstNodeName("Root"));
    m_gstScene->setRoot(m_gstRoot);
            
    // Generate PHANToM object  
    m_gstPHANToM = new gstPHANToMSelect(PHANTOM_NAME, reset);
    m_gstPHANToM->setName(gstNodeName("PHANToM"));
    // register the PHANTOM's event callback to receive select events,
    // pass the document pointer along as the userdata
    m_gstPHANToM->setEventCallback(selectCallBack, this);

    m_gstPhantomSep = new gstSeparator;
    m_gstPhantomSep->setName(gstNodeName("PhantomSep"));
    m_gstPhantomSep->addChild(m_gstPHANToM);
    m_gstRoot->addChild(m_gstPhantomSep);
    
    // Add separator for all haptic objects in the scene
    m_gstHapticObjects = new gstSeparator;
    m_gstHapticObjects->setName(gstNodeName("Haptic Scene"));
    m_gstRoot->addChild(m_gstHapticObjects);
    
    // Add separator for all select objects
    m_gstSelectedObjects = new gstSeparator;
    m_gstSelectedObjects->setName(gstNodeName("Selected Objects"));
    m_gstHapticObjects->addChild(m_gstSelectedObjects);

    // setup ghostGL
    InitGhostGL();
    InitFrustum();
}

void CWorldBuilderDoc::InitGhostGL()
{
    m_camera = new ghostGLSyncCamera;
    m_glmanager = new ghostGLManager(m_camera);
    m_glmanager->setAxesOn(FALSE);
    
    m_gstWorkspace = m_gstPHANToM->attachMaximalBoundary();
    m_gstWorkspace->setName(gstNodeName("Workspace"));
   
    m_camera->setWorkspaceBoundary(m_gstWorkspace);
    m_camera->setSyncMode(ghostGLSyncCamera::SYNC_CAMERA_TO_WORKSPACE);
    m_camera->setSyncToWindow(TRUE);         // sync to window is true by default
    m_camera->setViewEntireWorkspace(FALSE); // view the back plane only
    m_camera->setClipToWorkspace(TRUE);      // turn on graphic z-clipping for the workspace
    m_camera->update();                      // initialize the camera transform

    m_camera->setSyncMode(ghostGLSyncCamera::SYNC_WORKSPACE_TO_CAMERA);
    // now update the workspace based on the camera
    m_camera->update();

    // create the pinch transform, this will be added to ghostGL only
    // when the 'G' key is pressed down (and removed when it is released)
    m_pinchObj = new ghostGLPinchXForm;

    m_glmanager->setVisible(m_gstWorkspace, FALSE);

    // load the scene graph into ghostGLmanager
    m_glmanager->loadScene(m_gstScene);
}


gstShape *CWorldBuilderDoc::createShape(unsigned short type)
{
    gstShape *shape;

    switch (type) {
        case ObjectState::CUBE:
          shape = new gstCube;
          shape->setName(gstNodeName("Cube"));
          break;
        case ObjectState::SPHERE:
          shape = new gstSphere;
          shape->setName(gstNodeName("Sphere"));
          break;
        case ObjectState::CONE:
          shape = new gstCone;
          shape->setName(gstNodeName("Cone"));
          break;
        case ObjectState::CYLINDER:
          shape = new gstCylinder;
          shape->setName(gstNodeName("Cylinder"));
          break;
        case ObjectState::TORUS:
          shape = new gstTorus;
          shape->setName(gstNodeName("Torus"));
          break;
    }

    return shape;
}

void CWorldBuilderDoc::UpdateState(ObjectState *state)
{
    ASSERT(state);
    state->Update();

    gstTransform *node = state->hapticObject;
    ASSERT(node);
    COLORREF color = state->properties.color;

    // When in "go mode" everything is solid
    gstBoolean solid = (state->choose == TRUE) || m_bGoMode;

    gfxDisplaySettings *disp = m_glmanager->getDisplaySettings(node);
    ASSERT(disp);

    if (disp->preDisplayList < 1) {
        disp->preDisplayList = glGenLists(1);
        ASSERT(disp->preDisplayList);
    }
    glNewList(disp->preDisplayList, GL_COMPILE);
        glPushAttrib(GL_LIGHTING_BIT);
        glPushAttrib(GL_POLYGON_BIT);
        GLfloat colorfv[4] = {
            (color & 0xFF)/255.0,
            ((color >> 8) & 0xFF)/255.0,
            ((color >> 16) & 0xFF)/255.0, 1.0 };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorfv);
        
        if (!solid) {            
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    glEndList();

    if (disp->postDisplayList < 1) {
        disp->postDisplayList = glGenLists(1);
        ASSERT(disp->postDisplayList);
    }
    glNewList(disp->postDisplayList, GL_COMPILE);
        glPopAttrib();
        glPopAttrib();
    glEndList();    
}


//*****************************************************************************
// Function : init_frustum_dimensions
// ----------------------------------
// Description : Determines the dimensions to set the haptic frustum, given a
//               gstBoundaryCube object and the viewing parameters
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void CWorldBuilderDoc::InitFrustum(void)
{
    static const int FORCE_THRESHOLD = 2.0; // Newtons

    assert(!m_hapticFrustum);

    // create a haptic frustum and set its dimensions so that its bounding planes
    // intersect the back plane edges of the workspace
    m_hapticFrustum = new HapticFrustum();
    m_hapticFrustum->setTouchableFrom(gstTriPolyMeshHaptic::RV_FRONT);
    m_hapticFrustum->setThreshold(FORCE_THRESHOLD);
    m_hapticFrustum->setEventCallback(frustum_event_callback, NULL);
    
    // add the haptic frustum as a sibling of the phantom
    m_gstPhantomSep->addChild(m_hapticFrustum);

    // remove the boundary object, we only set it so that we could
    // use attachMaximalBoundary() to determine the size our frustum
    // should be
    m_gstPHANToM->setBoundaryObj(NULL);

    // disable drawing of the haptic frustum and the workspace
    m_glmanager->setVisible(m_hapticFrustum, FALSE);

    // resize the haptic frustum to match the current boundary
    UpdateFrustum();
}

//*****************************************************************************
// Function : update_frustum
// -------------------------
// Description : Resizes the haptic frustum based on the new dimensions of the
//               workspace boundary
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void CWorldBuilderDoc::UpdateFrustum(void)
{
    gstPoint viewerPos;
    float nearPlane, farPlane, aspectRatio, fieldOfView;
    float nearOffset, farOffset, border;
    ghostGLSyncCamera *myCamera;
    
    // get the current sync camera and set it up for workspace / camera syncing
    myCamera = (ghostGLSyncCamera *)m_glmanager->getCamera();

    fieldOfView = myCamera->getViewAngle();
    aspectRatio = (float) myCamera->getWindowWidth() / (float) myCamera->getWindowHeight();

    // need to calculate the nearPlane and farPlane, since the camera won't always coincide
    viewerPos = myCamera->getWorkspaceOffset();
    nearPlane = viewerPos.z() - m_gstWorkspace->getLength() / 2.0;
    farPlane = viewerPos.z() + m_gstWorkspace->getLength() / 2.0;

    // the near and far plane offsets allow for a difference in setting between the 
    // graphics clipping planes and the haptic bounding planes
    nearOffset = 0;
    farOffset = 0;
    border = 0;

    m_hapticFrustum->setPerspectiveView(viewerPos, fieldOfView, aspectRatio,
        nearPlane, farPlane, nearOffset, farOffset, border);

    // adjust the ghostGL graphic clipping planes so that the cursor doesn't get clipped
    nearPlane = myCamera->getNearClippingPlane();
    farPlane = myCamera->getFarClippingPlane();

    static const int PLANE_OFFSET = 15;
    myCamera->setClippingPlanes(nearPlane - PLANE_OFFSET, farPlane + PLANE_OFFSET);
}

//*****************************************************************************
// Function : frustumEventCallback
// -------------------------------
// Description : Receives the event callback when a HapticFrustum event occurs
// Return type : void 
// Argument #1 : gstTransform *node
// Argument #2 : void *cbData
// Argument #3 : void *userData
//*****************************************************************************
void frustum_event_callback(gstTransform *node, void *cbData, void *userData)
{
    gstEvent *event = (gstEvent *) cbData;
    
    // if a pop through event occurs, leave this haptic context
    if (event->id == HapticFrustum::POPPED_THROUGH) {
        
#ifdef WIN32
        CWinApp *appPtr = (CWinApp *) AfxGetApp();
        CFrameWnd *winPtr = (CFrameWnd *) appPtr->GetMainWnd();
        CHapticView *hapticView = (CHapticView *) winPtr->GetActiveView();        
#else // SGI or SUN
        AppView *hapticView = get_app_view();        
#endif

        // force the PHANToM to change into mouse mode
        hapticView->OnPhantomLeave();
    }
}       

void CWorldBuilderDoc::startGrab()
{
    m_glmanager->addActionObject(m_pinchObj);
    m_grabEnabled = true;
    SetTransform(ID_MAIN_SELECT);
}

void CWorldBuilderDoc::endGrab()
{
    m_glmanager->removeActionObject(m_pinchObj);
    m_grabEnabled = false;
}

gstBoolean CWorldBuilderDoc::isSceneRunning()
{
    return m_gstScene && !m_gstScene->getDoneServoLoop();
}

void CWorldBuilderDoc::Redraw()
{
    m_glmanager->redraw();
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "GLError: %s\n", gluErrorString(err));
    }
}

void CWorldBuilderDoc::Reshape(int w, int h)
{
    // early reshape might not have glmanager
    if (m_glmanager) {
        m_glmanager->reshape(w, h);    
        UpdateFrustum();
        centerSceneInWorkspace();
    } else {
        glViewport(0, 0, w, h);
 
    }   
}

void CWorldBuilderDoc::SetHaptics(gstBoolean state)
{
    ASSERT(m_gstRoot);
    ASSERT(m_gstHapticObjects);
    if (state) {
        m_gstRoot->addChild(m_gstHapticObjects);
    } else {
        /* Remove from everything haptic scene graph.  Note that when objects are
        removed from the scene, the scene does not perform memory deallocation.
        It is still up to the application to free the memory whenever an object
        is permamently deleted from the scene.  In this case, we want to keep
        the haptic scene allocated because we will re-add it when haptics are
        toggled on. */
        m_gstRoot->removeChild(m_gstHapticObjects);
    }
}


BOOL CWorldBuilderDoc::QueryCursorPos(double* pX, double* pY, double* pZ)
{
    if (! m_gstPHANToM)
        return FALSE;

    gstPoint phantomPos = m_gstPHANToM->getPosition_WC();

    *pX = phantomPos[0];
    *pY = phantomPos[1];
    *pZ = phantomPos[2];

    return TRUE;
}

