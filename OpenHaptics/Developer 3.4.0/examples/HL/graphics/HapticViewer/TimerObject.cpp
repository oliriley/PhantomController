/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  TimerObject.cpp

Description: 

  Helper facility for managing the high resolution timer
  
******************************************************************************/

#include "stdafx.h"
#include "TimerObject.h"

// mark counter used for labeling markTime output
static unsigned long g_markCount = 0;

#ifdef WIN32

// frequency for performance counter
static LARGE_INTEGER g_liFreq = {0, 0};

namespace
{
    // Since the TimerObject frequency is obtained the by the first TimerObject
    // created, we create a global here to take the extra overhead during
    // startup and to avoid any scary thread safety problems.
    class TimerInit
    {
    public:
        TimerInit()
        {
            // we need to get a divisor for the performance counter.
            QueryPerformanceFrequency(&g_liFreq);
        }
    } TIMER_INIT;
}

__int64 TimerObject::secToCTicks(double seconds)
{
    return __int64(seconds*g_liFreq.QuadPart);
}

double TimerObject::cTicksToSec(__int64 cTicks)
{
    return double(cTicks) / g_liFreq.QuadPart;
}

#endif // WIN32
