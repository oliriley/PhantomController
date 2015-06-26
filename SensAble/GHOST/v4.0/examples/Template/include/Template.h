//=============================================================================
//   Filename : Template.h
// Written by : 
//    Project : Template Ghost Application
//     Module : Platform Independent Application Entry
// ----------------------------------------------------------------------------
// This is a template GHOST application
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef TEMPLATE_H
#define TEMPLATE_H

#ifdef _WIN32
// Disable data conversion warnings
#pragma warning(disable : 4305)     // X86
#endif

//=============================================================================
//                        Global Variables / Constants
//=============================================================================
#define PHANTOM_NAME    "Default PHANToM"  // PHANToM configuration string

//=============================================================================
//                             Function Prototypes
//=============================================================================
void start_program(int bResetPHANToM);
void end_program(void);

#endif // TEMPLATE_H
