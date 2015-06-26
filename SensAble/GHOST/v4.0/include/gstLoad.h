//=============================================================================
//   Filename : gstLoad.h
//    Project : GHOST library v3.0
//    Authors : 
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Interface for getting haptic load (not supported)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GSTLOAD_INCLUDED
#define GSTLOAD_INCLUDED

// Interface for getting haptic load (not supported)

void open_load_THREAD(void *);
void open_load_THREAD(void *);
void send_load(int load);
int open_load_connection();
void close_load_connection();
                              
#endif
