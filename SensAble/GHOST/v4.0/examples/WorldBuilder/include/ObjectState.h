//////////////////////////////////////////////////////////////////////
// ObjectState.h: interface for the ObjectState class

#ifndef _OBJECTSTRUCT_H_
#define _OBJECTSTRUCT_H_

#include "sysdep.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////
// CObjectProperties

class CObjectProperties
{
public:
    // Constructor and destructor
    CObjectProperties();
    virtual ~CObjectProperties();
    
     // Copy just the property associated with the control id, if id == 0
     // then copy's all properties
    void CopyFrom(CObjectProperties *otherProperties, UINT id = 0);
    void CopyFromAll(CObjectProperties *otherProperties);
    friend CArchive& operator<<(CArchive& ar, CObjectProperties& properties);
    friend CArchive& operator>>(CArchive& ar, CObjectProperties& properties);

public:
    // these first 4 object properties are directly mapped into
    // gstShape properties in GHOST
    double damping;             // Kg / (1000.0 * sec) (0..0.005)
    double stiffness;           // =spring (0..1.0)
    double staticFriction;      // (0..1.0)
    double dynamicFriction;     // (0..1.0)
    // color is not a GHOST property, but ghostGL draws this for us
    COLORREF color;
};

//////////////////////////////////////////////////////////////////////
// ObjectState

class ObjectState
{
public:
    // Constructor and destructor
    ObjectState();
    virtual ~ObjectState();

    void Update();
    friend CArchive& operator<<(CArchive& ar, ObjectState* object);
    friend CArchive& operator>>(CArchive& ar, ObjectState* object);

public:
    gstTransform*       hapticObject;
    // Store display list
    unsigned short      primitiveType;
    // Store group number
    unsigned long       groupID;
    // TRUE then object was choose last
    int                 choose;
    CObjectProperties   properties;

    enum PrimitiveType { CUBE, SPHERE, CONE, CYLINDER, TORUS };
};

#endif  // _OBJECTSTRUCT_H_
