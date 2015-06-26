//=============================================================================
//   Filename : Template_graphics.h
// Written by : 
//    Project : Template Ghost Application
//     Module : Platform Independent Graphics
// ----------------------------------------------------------------------------
// This is a template GHOST application
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef TEMPLATE_GRAPHICS_H
#define TEMPLATE_GRAPHICS_H

#include "Template.h"

void init_graphics(gstScene *pScene = NULL);
void term_graphics(void);
void reshape(GLint width, GLint height);
void update_graphics(void);

#endif // TEMPLATE_GRAPHICS_H
