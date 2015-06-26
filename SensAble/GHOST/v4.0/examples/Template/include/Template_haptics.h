//=============================================================================
//   Filename : Template_haptics.h
// Written by : 
//    Project : Template Ghost Application
//     Module : Platform Independent Haptics
// ----------------------------------------------------------------------------
// This is a template GHOST application
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef TEMPLATE_HAPTICS_H
#define TEMPLATE_HAPTICS_H

#include "Template.h"

void init_haptics(gstScene *pScene = NULL, int bResetPHANToM = TRUE);
void term_haptics(void);
void enable_servo_loop(int bEnable);
int scene_done(void);
int query_phantom_pos(double *px, double *py, double *pz);

#endif // TEMPLATE_HAPTICS_H
