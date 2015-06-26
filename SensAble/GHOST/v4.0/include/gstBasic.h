//=============================================================================
//   Filename : gstBasic.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Includes common headers. Has version function, inherit macros.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_BASIC_H_
#define _GST_BASIC_H_

#include <math.h>
#include <gstWindefs.h>
#include <gstSgidefs.h>
#include <gstSundefs.h>
#include <gstDllExport.h>
#include <gstTypedefs.h>
#include <gstId.h>

// GHOST version, use for conditional compilation
#define GHOST_LIB_VERSION 310

// Returns GHOST version string
GHOST_DLL_IMPORT_EXPORT const char *gstGetVersion();

// Returns PDD version string
GHOST_DLL_IMPORT_EXPORT const char *gstGetPDDVersion();

// Check if two numbers are (nearly) the same
GHOST_DLL_IMPORT_EXPORT inline gstBoolean withinEpsilon(
    double d1,
    double d2,
    double epsilon=0.00000001)
{
	return (fabs(d1-d2) < epsilon);
}


// GHOST_INERIT_HEADER and GHOST_INHERIT_IMPLEMENTATION are two macros
// used implement dynamic typing required by all classes derived
// from gstNode in GHOST (that is any scene graph node). The macros
// add 4 functions and a data member to the class.
//
// Call GHOST_INHERIT_HEADER inside the class declaration (.h file)
//
#define GHOST_INHERIT_HEADER(ThisClass,ParentClass)   \
public:                                               \
    /* static: get type id of this class */           \
    static gstType getClassTypeId()                   \
    {                                                 \
      gstAssignUniqueId(ThisClass##ClassTypeId);      \
      return ThisClass##ClassTypeId;                  \
    }                                                 \
    /* virtual form of getClassTypeId */              \
    virtual gstType getTypeId() const                 \
    {                                                 \
      return getClassTypeId();                        \
    }                                                 \
    /* Return TRUE if class is of the given type or is derived from that type */ \
    static gstBoolean staticIsOfType(gstType type)    \
    {                                                 \
      if (type == getClassTypeId()) return TRUE;      \
      else return (ParentClass::staticIsOfType(type));\
    }                                                 \
    /* virtual form of staticIsOfType */              \
    virtual gstBoolean isOfType(gstType type) const   \
    {                                                 \
      return staticIsOfType(type);                    \
    }                                                 \
                                                      \
                                                      \
private:                                              \
    static gstType ThisClass##ClassTypeId

//
// call GHOST_INHERIT_IMPLEMENTATION insde the source (.cpp file).
//
#define GHOST_INHERIT_IMPLEMENTATION(ThisClass) \
    gstType ThisClass::ThisClass##ClassTypeId


#endif // _GST_BASIC_H_
