//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLWrite.h
//
// Rev: 1.0
//
// Description: Utilities for VRML output.
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_WRITE_H_
#define _GST_VRML_WRITE_H_

#include <gstVRMLDllExport.h>
class gstSeparator;
class ostream;

// For internal use only.  Rotations this function writes are not correct, may
// have other problems, for experimental use only.
GHOST_VRML_DLL_IMPORT_EXPORT gstBoolean gstWriteBogusVRMLFile(const char *filename, gstSeparator *node);

void gstVRMLWriteHeader(ostream &os);
void gstVRMLWriteNode(ostream &os, gstSeparator *node, int level = 0);

#endif
