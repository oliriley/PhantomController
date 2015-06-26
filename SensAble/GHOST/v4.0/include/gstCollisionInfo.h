//=============================================================================
//   Filename : gstCollisionInfo.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: Structure used internally to track collisions with objects
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_COLLISION_INFO_H_
#define _GST_COLLISION_INFO_H_

#include <gstBasic.h>
#include <gstPoint.h>
#include <gstVector.h>
class gstShape;

// Specifics about a single collision between the PHANToM and a shape
struct GHOST_DLL_IMPORT_EXPORT gstCollisionInfoStruct
{
	gstPoint	SCP;
	gstVector	SCPnormal;
	double	Kobj;
	double	dynamicFriction;
	double	staticFriction;
	double	surfaceDamping;
	gstShape *obj;
};

#endif // _GST_COLLISION_INFO_H_
