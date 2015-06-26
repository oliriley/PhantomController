//=============================================================================
//   Filename : gstEventStack.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Mechanism for storing gstEvents
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_EVENTSTACK
#define GST_EVENTSTACK

#include <gstDllExport.h>

// Maximum number of events
#define EVENT_STACK_SIZE 8

// Event data structure
typedef struct GHOST_DLL_IMPORT_EXPORT _gstEvent {
	int			id;
	int			data1d;
	int			data2d;
	int			data3d;
	double		data1f;
	double		data2f;
	double		data3f;
	void		*data1v;
} gstEvent;


#include <gstBasic.h>

// Stack for storing gstTransform events
class GHOST_DLL_IMPORT_EXPORT gstEventStack {

public:

	// Constructor.
	gstEventStack() {
		numEvents=0;
		eventIndexTop = 0;
		eventIndexBottom = 0;
	}

	// Destructor.
	~gstEventStack() {}
	
	// Push event onto event stack.
	inline gstBoolean push(const gstEvent &newEvent);
	
	// Pop event from event stack.
	inline gstBoolean pop(gstEvent &nextEvent);

	// Clear event stack.
	inline void	clear() {
		numEvents = 0;
		eventIndexTop = 0;
		eventIndexBottom = 0;
	}

protected:

	gstEvent eventStack[ EVENT_STACK_SIZE ];
	int		 numEvents, eventIndexTop, eventIndexBottom;
};


// Push event onto event stack.
inline gstBoolean	gstEventStack::push(const gstEvent &newEvent) {
	if (numEvents < EVENT_STACK_SIZE) {
		eventStack[eventIndexTop] = newEvent;
		eventIndexTop = (eventIndexTop+1)%EVENT_STACK_SIZE;
		numEvents++;
		return TRUE;
	} else {
		return FALSE;
	}
}


// Pop event from event stack.
inline gstBoolean	gstEventStack::pop(gstEvent &nextEvent) {
	if (numEvents > 0) {
		nextEvent = eventStack[eventIndexBottom];
		eventIndexBottom = (eventIndexBottom + 1)%EVENT_STACK_SIZE;
		numEvents--;
		return TRUE;
	} else {
		return FALSE;
	}
}

#endif // GST_EVENTSTACK
