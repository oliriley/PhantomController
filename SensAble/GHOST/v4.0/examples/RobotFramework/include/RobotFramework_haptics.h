//=============================================================================
//   Filename : RobotFramework_haptics.cpp
// Written by : Brandon Itkowitz
//    Project : Example GhostGL Application w/ Transforms & Color
//     Module : Platform Independent Haptics
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef ROBOT_FRAMEWORK_HAPTICS_H
#define ROBOT_FRAMEWORK_HAPTICS_H

#include "RobotFramework.h"

#define ROBOT_TORSO_HEIGHT      2.0
#define ROBOT_TORSO_RADIUS      0.5
#define ROBOT_SHOULDER_RADIUS   ROBOT_TORSO_RADIUS / 2.0
#define ROBOT_ARM_LENGTH        ROBOT_TORSO_HEIGHT * .75
#define ROBOT_ARM_RADIUS        ROBOT_SHOULDER_RADIUS * .75
#define ROBOT_HAND_RADIUS       ROBOT_SHOULDER_RADIUS * .75
#define ROBOT_LEG_LENGTH        ROBOT_TORSO_HEIGHT
#define ROBOT_LEG_RADIUS        ROBOT_TORSO_RADIUS * .33
#define ROBOT_FOOT_LENGTH       ROBOT_ARM_LENGTH * .5
#define ROBOT_FOOT_WIDTH        ROBOT_LEG_RADIUS * 2.0
#define ROBOT_FOOT_HEIGHT       ROBOT_FOOT_WIDTH * 3.0 / 4.0
#define ROBOT_TOTAL_HEIGHT      (ROBOT_TORSO_HEIGHT + ROBOT_TORSO_RADIUS * 2.0 + ROBOT_LEG_LENGTH + ROBOT_FOOT_HEIGHT)

//==============================================================================
//                   F u n c t i o n   P r o t o t y p e s 
//==============================================================================

gstScene *init_haptics(int bResetPHANToM);
void term_haptics(void);
void enable_servo_loop(int enable);
int  scene_done(void);
int  query_phantom_pos(double *px, double *py, double *pz);

void init_robot_transform();
gstSeparator *create_robot();
gstSeparator *create_robot_arm();
gstSeparator *create_robot_leg();

#endif // ROBOT_FRAMEWORK_HAPTICS_H
