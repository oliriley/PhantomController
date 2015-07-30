/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  TimerObject.h

Description: 

  Helper facility for managing the high resolution timer
  
******************************************************************************/

#ifndef TimerObject_DEFINE
#define TimerObject_DEFINE

//-----------------------------------------------------------------------------
// TimerObject
// Wrapper class for Windows high resolution timer
// To use a timer object:
//      timerObject tObj;
//      tObj.restart();
// do stuff you want to time
//      double result = tObj.getTime();
// result now contains time in seconds between call to start and stop.
//-----------------------------------------------------------------------------
#ifdef WIN32
#define NOMINMAX
#include <windows.h>

class TimerObject
{

public:

    // Default Constructor.
    TimerObject();

    // Destructor
    virtual ~TimerObject() {};

    // Start/Restart timing.
    void restart();

    // Get time in seconds since timer was started.
    const double getTime() const;

    const __int64 getCTicks() const;
    static __int64 secToCTicks(double seconds);

    static double cTicksToSec(__int64 cTicks);

protected:

    LARGE_INTEGER m_liStarted;
};

// Default Constructor.
inline TimerObject::TimerObject()
{
    restart();
}

// Get time in seconds since timer was started.
inline const double TimerObject::getTime() const
{
    LARGE_INTEGER liNow;
    QueryPerformanceCounter(&liNow);
    // 64 bit math: QuadPart is a LONGLONG which is an __int64
    return cTicksToSec(getCTicks());
}

inline const __int64 TimerObject::getCTicks() const
{
    LARGE_INTEGER liNow;
    QueryPerformanceCounter(&liNow);
    // 64 bit math: QuadPart is a LONGLONG which is an __int64
    return __int64(liNow.QuadPart - m_liStarted.QuadPart);
}

// Start/Restart timing.
inline void TimerObject::restart()
{
    QueryPerformanceCounter(&m_liStarted);
}

#elif defined(linux) || defined(__APPLE__)

#include <sys/time.h>

class TimerObject
{

public:

    // Default Constructor.
    TimerObject();

    // Destructor
    virtual ~TimerObject() {};

    // Start/Restart timing.
    void restart();

    // Get time in seconds since timer was started.
    const double getTime() const;

protected:

    double m_liStarted;
};

// Default Constructor.
inline TimerObject::TimerObject()
{
    restart();
}

// Get time in seconds since timer was started.
inline const double TimerObject::getTime() const
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (tv.tv_sec + tv.tv_usec / 1e6);
}

// Start/Restart timing.
inline void TimerObject::restart()
{
    m_liStarted = getTime();
}

#endif

#endif // TimerObject
