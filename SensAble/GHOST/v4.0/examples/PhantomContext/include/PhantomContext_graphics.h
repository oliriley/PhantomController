//*****************************************************************************
//      Filename : PhantomContext_graphics.h
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef PHANTOM_CONTEXT_GRAPHICS_H
#define PHANTOM_CONTEXT_GRAPHICS_H

#include "PhantomContext.h"

//*****************************************************************************
//                    F u n c t i o n   P r o t o t y p e s 
//*****************************************************************************
void init_graphics(gstScene *pScene = NULL);
void term_graphics(void);
void reshape(int width, int height);
void update_graphics(void);
void init_frustum(void);
void update_frustum(void);
double get_frustum_height();
void setup_cursor(gstTransform *);
void toggle_axes(void);
void toggle_graphics();
int  get_graphics();

#endif // PHANTOM_CONTEXT_GRAPHICS_H  
