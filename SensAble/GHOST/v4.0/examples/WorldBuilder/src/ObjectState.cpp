//////////////////////////////////////////////////////////////////////
// ObjectStruct.cpp: implementation of the ObjectState class
#include "gstAfx.h"
#include "ObjectState.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define DEFDYNFRICTION  1
#define DEFSTATFRICTION 1
#define DEFSURFDAMPING  1
#define DEFSURFSPRING   1

//////////////////////////////////////////////////////////////////////
// CObjectProperties construction/destruction

CObjectProperties::CObjectProperties()
{
    damping = 0.0;              // Resistance to velocity Kg / (1000000 * sec) (0..5.0)
    stiffness = 1.0;            // Stiffness = spring (0..1.0)
    staticFriction = 0.0;       // (0..1.0)
    dynamicFriction = 0.0;      // (0..1.0)
    color = RGB(255, 255, 255);
}

CObjectProperties::~CObjectProperties()
{
}

//////////////////////////////////////////////////////////////////////
// CObjectProperties methods

  // copy all properties
void CObjectProperties::CopyFromAll(CObjectProperties *otherProperties)
{
    damping = otherProperties->damping;
    stiffness = otherProperties->stiffness;
    staticFriction = otherProperties->staticFriction;
    dynamicFriction = otherProperties->dynamicFriction;
    color = otherProperties->color;
}

  // copy just one property, using control ID's to indicate which property
void CObjectProperties::CopyFrom(CObjectProperties *otherProperties, UINT id)
{
    switch (id) {
        case 0:
            CopyFromAll(otherProperties);
            break;
        case ID_MATERIAL_COLOR:
        case ID_MATERIAL_COLORPALETTE:
            color = otherProperties->color;
            break;
        case ID_MATERIAL_DAMPING:
            damping = otherProperties->damping;
            break;
        case ID_MATERIAL_STIFFNESS:
            stiffness = otherProperties->stiffness;
            break;
        case ID_MATERIAL_SFRICTION:
            staticFriction = otherProperties->staticFriction;
            break;
        case ID_MATERIAL_DFRICTION:
            dynamicFriction = otherProperties->dynamicFriction;
            break;
        default:
              // unexpected id given
            assert(0);
            break;
    }
}

CArchive& operator<<(CArchive& ar, CObjectProperties& properties)
{
    ar << properties.damping;
    ar << properties.stiffness;
    ar << properties.staticFriction;
    ar << properties.dynamicFriction;
    ar << properties.color;
    return ar;
}

CArchive& operator>>(CArchive& ar, CObjectProperties& properties)
{
    ar >> properties.damping;
    ar >> properties.stiffness;
    ar >> properties.staticFriction;
    ar >> properties.dynamicFriction;
    ar >> properties.color;
    return ar;
}


//////////////////////////////////////////////////////////////////////
// ObjectState construction/destruction

ObjectState::ObjectState()
{
    // Set defaults for each new object
    primitiveType = 0;
    groupID = 0;
    choose = FALSE;
}

ObjectState::~ObjectState()
{
    // we do not delete the hapticObject pointer
}

//////////////////////////////////////////////////////////////////////
// ObjectState methods

void ObjectState::Update()
{
    gstShape *object = (gstShape *)hapticObject;
    ASSERT(object);
    object->setSurfaceKdamping(properties.damping / 1000.0);
    if (properties.stiffness == 0)
        object->setSurfaceKspring(0.00000001);
    else
        object->setSurfaceKspring(properties.stiffness);
    object->setSurfaceFstatic(properties.staticFriction);
    object->setSurfaceFdynamic(properties.dynamicFriction);
}


CArchive& operator<<(CArchive& ar, ObjectState* object)
{
    ar << object->primitiveType;
    ar << object->groupID;
    ar << object->properties;
    return ar;
}

CArchive& operator>>(CArchive& ar, ObjectState* object)
{
    ar >> object->primitiveType;
    ar >> object->groupID;
    ar >> object->properties;
    return ar;
}

