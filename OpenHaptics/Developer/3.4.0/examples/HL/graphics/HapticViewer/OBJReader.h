/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  OBJReader.h

Description: 

  Routines for reading meshses from Wavefront .obj files
  
******************************************************************************/

#ifndef CXX_OBJREADER_H_
#define CXX_OBJREADER_H_

#include "stdAfx.h"
#include "Mesh.h"

struct MsgIDError
{
    MsgIDError(const char *s) : msg(s) {}
    
    const char *msg;
};

bool readOBJAscii(const char *filename,
                  VertContainer* verts,
                  TriContainer* tris,
                  hduBoundBox3Df* vertBox
                  );

#endif // CXX_OBJREADER_H_

/******************************************************************************/
