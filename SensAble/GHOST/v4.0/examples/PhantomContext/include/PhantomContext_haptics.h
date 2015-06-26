//*****************************************************************************
//      Filename : PhantomContext_haptics.h
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#ifndef _PHANTOM_CONTEXT_HAPTICS_H_
#define _PHANTOM_CONTEXT_HAPTICS_H_

#include "PhantomContext.h"

//*****************************************************************************
//                             C o n s t a n t s 
//*****************************************************************************
#define SPHERE_RADIUS 15 // geometry units are in millimeters (mm)
#define CONE_RADIUS   SPHERE_RADIUS * .65
#define CONE_HEIGHT   SPHERE_RADIUS * 2.0

#define SURFACE_FRICTION 0.2

//*****************************************************************************
//                    F u n c t i o n   P r o t o t y p e s 
//*****************************************************************************
gstScene *init_haptics(int bResetPHANToM);
void term_haptics(void);
void enable_servo_loop(int bEnable);
int  scene_done(void);
int  query_phantom_pos(double *px, double *py, double *pz);
void frustum_event_callback(gstTransform *node, void *cbData, void *userData);
void create_scene_geom(gstSeparator *geomSep = NULL);
void toggle_haptics();
int  get_haptics();

#endif
