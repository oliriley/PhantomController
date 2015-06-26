//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRML.h
//
// Rev: 1.0
//
// Description: Public access functions to the VRML parsing 
//              functionality and the error reporting thereof.
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_H_
#define _GST_VRML_H_

#include <gstVRMLDllExport.h>
#include <gstVRMLError.h>
#include <gstBasic.h>

class gstSeparator;
class gstVRMLError;

// Read a VRML 2 file and convert into a GHOST scene graph.
// Given the name of a file in the VRML 2.0 format, this function
// returns a gstSeparator containing the VRML scene graph in the
// GHOST v2 format.
GHOST_VRML_DLL_IMPORT_EXPORT gstSeparator *gstReadVRMLFile(const char *filename);

// Report errors about reading or conversion.
// Errors in the reading, parsing and instantiation of the the
// input file are recorded in gstVRMLError class structures and
// placed in a stack in the order in which they occur.  The stack
// is reset when gstReadVRMLFile is called.
// The following functions should can be used to retreive any
// errors encountered during parsing.

// get earliest error, return no error if none
GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError gstVRMLGetError();
// how many errors
GHOST_VRML_DLL_IMPORT_EXPORT int gstVRMLGetNumErrors();
// get earliest and remove it from list
GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError gstVRMLPopEarliestError();
// get earliest error
GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError gstVRMLGetEarliestError();
// get latest and remove it from list
GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError gstVRMLPopLatestError();
// get latest error
GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError gstVRMLGetLatestError();
// clear all errors
GHOST_VRML_DLL_IMPORT_EXPORT void gstVRMLClearErrors();
// dump errors to given filename
GHOST_VRML_DLL_IMPORT_EXPORT int gstVRMLWriteErrorsToFile(const char *filename);
// convert error type to string representation
GHOST_VRML_DLL_IMPORT_EXPORT const char *gstVRMLGetErrorTypeName(gstVRMLErrorType errType);

#endif // _GST_VRML_H_
