//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLDllExport.h
//
// Rev: 1.0
//
// Description: Include file for exporting or importing
//              classes and functions of the GHOST VRML Dll
//
// Note: On Windows you must have one of the two symbols defined
//       either GHOST_VRML_DLL_BUILD or GHOST_VRML_DLL_LINK
//
// Author: Tim Gallagher
// Previous Authors: Ted Bardasz
//
// History:
#ifndef _GST_VRML_DLL_EXPORT_H_
#define _GST_VRML_DLL_EXPORT_H_

#ifdef _WINDOWS

	#pragma warning( disable : 4231 4660 )

	#if defined(GHOST_VRML_DLL_BUILD) // building the dll

		#define GHOST_VRML_DLL_IMPORT_EXPORT __declspec(dllexport)
		#define GHOST_VRML_EXPIMP_TEMPLATE

	#else // building against the dll

		#define GHOST_VRML_DLL_IMPORT_EXPORT __declspec(dllimport)
		#define GHOST_VRML_EXPIMP_TEMPLATE extern

	#endif // build/link symbols

	#define GHOST_VRML_EXTRA_TEMPLATE_DECLARATIONS 1

#else	// !_WINDOWS => Must be Unix

	#define GHOST_VRML_DLL_IMPORT_EXPORT
	#define GHOST_VRML_EXPIMP_TEMPLATE

	#define GHOST_VRML_EXTRA_TEMPLATE_DECLARATIONS 0

#endif // platforms

#endif // _GST_VRML_DLL_EXPORT_H_			
