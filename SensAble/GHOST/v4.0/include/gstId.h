//=============================================================================
//   Filename : gstId.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: For creating unique id's for events and dynamic typing
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_ID_H_
#define _GST_ID_H_

#include <gstDllExport.h>
#include <gstTypedefs.h>

gstEventType GHOST_DLL_IMPORT_EXPORT gstNextUniqueId();

void GHOST_DLL_IMPORT_EXPORT gstAssignUniqueId(gstType &type);

#endif // _GST_ID_H_
