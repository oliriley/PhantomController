//=============================================================================
//   Filename : gstModifyBase.h
//    Project : GHOST library v3.0
//    Authors :
//    Revised :
// ----------------------------------------------------------------------------
// Description: Small base class use by gstTriPolyMesh*
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================


#if !defined(GSTMODIFYBASE_INCLUDED)
#define GSTMODIFYBASE_INCLUDED

#include <gstBasic.h>
#include <gstDllExport.h>


// Interface to signal when a instance is modified
class GHOST_DLL_IMPORT_EXPORT gstModifyBase {
public:
	virtual void modified() {}
};

#endif
