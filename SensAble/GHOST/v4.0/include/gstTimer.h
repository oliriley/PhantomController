//=============================================================================
//   Filename : gstTimer.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Wraper for measuring elapsed time
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_TIMER
#define GST_TIMER

#include <gstDllExport.h>

#ifdef WIN32

 #include <process.h>
 // This is all for the WinNT multimedia timer that
 // is used to maintain update rate.
 #include <windows.h>
 #include <mmsystem.h>
 #include <LARGEINT.H>

#else

 #include <sys/time.h>
 static struct timeval startTime;

#endif

// Wraper for measuring elapsed time
class GHOST_DLL_IMPORT_EXPORT gstTimer {
public:
	gstTimer();
	virtual ~gstTimer(){}

	float secondsSinceLastQuery();
	float secondsSinceReset();
	void reset();

private:
#ifdef WIN32
	LARGE_INTEGER d_liFirst, d_liLast, d_liFreq;
	DWORD d_PCCountsPerMs;
#else
	struct timeval d_liFirst, d_liLast, d_liFreq;
#endif
	
};

// Old debugTimer convenience method
void debugTimerStart();
// Old debugTimer convenience method
float debugTimerStop();

// Provides a wrapper for gstTimer that will allow you to sprinkle the
// codebase with instances of gstTimerRecords and accumulate timing data
// The timing data will be output to a file after a set number of updates

#define MAX_TIMER_UPDATES	1000

class gstTimerRecord {
public:
    gstTimerRecord(const char *recordName, int maxUpdates = MAX_TIMER_UPDATES);
    virtual ~gstTimerRecord();

    void start();   // start the timer
    void stop();    // stop the timer and store delta

    void outputTimerData(); // handles writing all of the presently logged data to file

    static void setFileName(const char *);

private:
    static char*    m_fileName;     // output filename for log

    gstTimer        m_timer;

    int             m_updateCount;  // current update count
    int             m_maxUpdates;   // array limit set at construction
    float*          m_timerData;    // an array of time values
    char*           m_recordName;   // tag name for data set
};


#endif
