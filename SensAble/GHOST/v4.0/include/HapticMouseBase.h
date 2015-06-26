// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#ifndef __HAPTIC_MOUSE_BASE_H__
#define __HAPTIC_MOUSE_BASE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HapticDoc.h"
#include "PHANToMMouseIoCtl.h"

#define ERROR_EVENT_COUNT   30

#define WM_PHANTOMMOUSEPUSHTHRU     WM_USER  // valid for private user defined messaging

// This structure is only valid on the user side. It will need to be converted to
// a suitable form for the kernel
// Each field has specifies its allowable range via the notation [min, max]
typedef struct {
    BOOL  bEnableMouse;        // must be set in order for the mouse to activate
    float nMouseScale;	       // scale ratio between (x, y) movement in PHANToM space and pixel space [1, 10] 
    float nMouseAccel;	       // mouse acceleration [1, 10]
    BOOL  bPlaneEnabled;	   // whether or not to use the contact plane for mouse simulation
    BOOL  bAutoPlaneDepth;     // overrides the plane depth setting to be at the initial z of the PHANToM
    float nPlaneDepth;	       // z depth of the mouse plane [-200, 200]
    float nPlaneAngle; 	       // angle (degrees) for cw rotation of plane about the x axis [0, 90]
    float nPlaneForce;         // magnifier for the relative force between planes [1, 10]
    float nPlaneDistance;      // magnifier for the relative distance between planes [1, 10]
    BOOL  bPushThruEnabled;    // send an event message back signifying a push through event
    float nPushThruDepth;      // z depth that will cause a push through event [0, 20]
    char  szPHANToMName[128];  // the name of the configured PHANToM device to use
} PHANToMMouseConfig;

class CHapticMouseBase : public CView
{
protected: // create from serialization only

    CHapticMouseBase();
    DECLARE_DYNCREATE(CHapticMouseBase)

// Attributes
public:

    typedef enum {
        PHANTOM_MOUSE_FALL_THRU = 0,
        PHANTOM_MOUSE_CLICK_THRU,
        PHANTOM_MOUSE_PUSH_THRU
    } HapticEntryMode;

    // PMouse Configuration Routines
    void EnablePHANToMMouse(BOOL bEnable);

    BOOL IsPHANToMMouseEnabled() const {
        return m_bPHANToMMouseEnabled;
    }

    void SetPHANToMMouseConfig(const PHANToMMouseConfig &config) {
        m_phantomMouseConfig = config;

    	// make sure the PHANToMName string gets copied
	    strcpy(m_phantomMouseConfig.szPHANToMName, config.szPHANToMName);
    }

    const PHANToMMouseConfig &GetPHANToMMouseConfig() const {
        return m_phantomMouseConfig;
    }

    HapticEntryMode GetHapticEntryMode() const {
	    return m_hapticEntryMode;
    }

    CHapticDoc *GetDocument() {
        return (CHapticDoc *) m_pDocument;
    }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CHapticMouseBase();

// Generated message map functions
protected:
	//{{AFX_MSG(CHapticMouseBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

    // Worker functions
    BOOL OpenMouseDriver();
    BOOL CloseMouseDriver();
    BOOL EnableMouseDriver(BOOL bEnable = TRUE);
    BOOL IsMouseDriverEnabled();
    BOOL UpdateMouseConfig();
    BOOL SetWhichButton(UCHAR buttonMask);
    void SetHapticEntryMode(HapticEntryMode mode);

    // Push-through behaviors
    BOOL CreatePushThruThread();
    BOOL DeletePushThruThread();
    BOOL EnablePushThruMode(BOOL bEnable = TRUE);
    static DWORD PushThruSignaler(void *pData);

    // Error event behaviors
    BOOL CreateErrorThread();
    BOOL DeleteErrorThread();
    static DWORD ErrorSignaler(void *pData);

private:

    void ConvertMouseConfig(PHANToMMouseConfig &, KPHANToMMouseConfig &);

protected:

    static BOOL m_bPHANToMMouseEnabled; // maintains the state of whether pmouse is going to be used
    static BOOL m_bPHANToMMouseError;   // flag is set when a mouse error occurs

    BOOL m_bStartProgram;        // when this flag is true, initialization will occur
    BOOL m_bInitialized;         // set to true following the invocation of StartProgram
    BOOL m_bMouseInView;         // true when the mouse cursor is within the view region
    BOOL m_bHapticsOn;           // true when the application servoloop is on
    
    PHANToMMouseConfig  m_phantomMouseConfig; // configuration data for the phantom mouse simulation
    HapticEntryMode     m_hapticEntryMode;    // mechanism for determining haptic entry
    UCHAR               m_buttonMask;         // mask used for storing the current mouse button config

    HANDLE  m_hDriver;                      // Handle to the PHANToM Mouse driver    
    CHAR    m_bufInput[IOCTL_INBUF_SIZE];   // Input to device
    CHAR    m_bufOutput[IOCTL_OUTBUF_SIZE]; // Output from device

    // default PHANToM Mouse initializer values
    static const BOOL   kPHANToMMouseEnable;             // must be set in order for the mouse to activate
    static const float  kPHANToMMouseScale;              // scale between (x, y) movement in PHANToM space and pixel space 
    static const float  kPHANToMMouseAcceleration;       // magnifier for the mouse velocity
    static const BOOL   kPHANToMMousePlaneEnabled;       // whether or not to use the contact plane for mouse simulation
    static const BOOL   kPHANToMMouseAutoPlaneDepth;     // overrides the plane depth setting to be at the initial z of the PHANToM
    static const float  kPHANToMMousePlaneDepth;         // z depth of the mouse plane
    static const float  kPHANToMMousePlaneAngle;         // angle for ccw rotation of plane about the x axis
    static const float  kPHANToMMousePlaneForce;         // magnifier for the relative force between non-gimbal planes
    static const float  kPHANToMMousePlaneDistance;      // magnifier for the relative distance between non-gimbal planes
    static const BOOL   kPHANToMMousePushThruEnabled;    // send an event message back signifying a push through event
    static const float  kPHANToMMousePushThruDepth;      // z offset from plane that will cause a push through event
    static const char   kPHANToMMouseName[];		     // default name of the configured PHANToM device to use

    // Push-through synchronization data
    static HANDLE m_hPushThruEvent;
    static HANDLE m_hThreadPushThruSignalCatcher;
    // Error event synchronization data
    static HANDLE m_ahErrorEvents[ERROR_EVENT_COUNT];
    static HANDLE m_hThreadErrorSignalCatcher;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __HAPTIC_MOUSE_BASE_H__
