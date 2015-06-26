//=============================================================================
//   Filename : gstDllExport.h
//    Project : GHOST library v3.0
//    Authors : Ted Bardasz
//    Revised :
// ----------------------------------------------------------------------------
// Description: Build macros
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================
#ifndef _GST_DLL_EXPORT_H_
#define _GST_DLL_EXPORT_H_

// The headers classes and functions in the headers need to have certain
// specifiers for building the DLL, and different ones from applications
// which are using the DLL.  These macros set the specifiers appropriately.

#if defined( WIN32 ) || defined ( _WINDOWS )

	#pragma warning( disable : 4231 4660 )

	#ifdef GHOST_DLL_BUILD

		#define GHOST_DLL_IMPORT_EXPORT __declspec(dllexport)
		#define GHOST_EXPIMP_TEMPLATE

	#else

		#define GHOST_DLL_IMPORT_EXPORT __declspec(dllimport)
		#define GHOST_EXPIMP_TEMPLATE extern

	#endif // GHOST_DLL_BUILD

	#define GHOST_EXTRA_TEMPLATE_DECLARATIONS 1

#else	// !_WINDOWS => Must be Unix

	#define GHOST_DLL_IMPORT_EXPORT
	#define GHOST_EXPIMP_TEMPLATE

	#define GHOST_EXTRA_TEMPLATE_DECLARATIONS 0
	
#endif // _WINDOWS


#endif // _GST_DLL_EXPORT_H_			
