//=============================================================================
//   Filename : gstSpew.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: Utility function to print out the shape and some
//              details of a gstNode tree.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_SPEW_H_
#define _GST_SPEW_H_

#include <gstDllExport.h>
#include <iostream.h>
class gstSeparator;


GHOST_DLL_IMPORT_EXPORT void gstSpew(
	gstSeparator *sep = 0, 
	ostream &os = cout,
	int level = 0);


#endif // _GST_SPEW_H_
