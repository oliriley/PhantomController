// SensAble Technologies, Inc. Copyright 1996-1999
// All rights reserved.

#include "stdafx.h"
#include "HapticDoc.h"
#include "HapticMouseBase.h"

#include <winioctl.h>
#include "PHANToMMouseIoCtl.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHapticMouseBase

IMPLEMENT_DYNCREATE(CHapticMouseBase, CView)

BEGIN_MESSAGE_MAP(CHapticMouseBase, CView)
    //{{AFX_MSG_MAP(CHapticMouseBase)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PHANToM Mouse default initializers

const BOOL   CHapticMouseBase::kPHANToMMouseEnable = FALSE;
const float  CHapticMouseBase::kPHANToMMouseScale = 1.5;
const float  CHapticMouseBase::kPHANToMMouseAcceleration = 5.0;
const BOOL   CHapticMouseBase::kPHANToMMousePlaneEnabled = TRUE;
const BOOL   CHapticMouseBase::kPHANToMMouseAutoPlaneDepth = TRUE;
const float  CHapticMouseBase::kPHANToMMousePlaneDepth = 0.0;
const float  CHapticMouseBase::kPHANToMMousePlaneAngle = 0.0;
const float  CHapticMouseBase::kPHANToMMousePlaneForce = 8.0;
const float  CHapticMouseBase::kPHANToMMousePlaneDistance = 5.0;
const BOOL   CHapticMouseBase::kPHANToMMousePushThruEnabled = FALSE;
const float  CHapticMouseBase::kPHANToMMousePushThruDepth = 5.0;
const char   CHapticMouseBase::kPHANToMMouseName[] = "Default PHANToM";

BOOL CHapticMouseBase::m_bPHANToMMouseEnabled = FALSE;
BOOL CHapticMouseBase::m_bPHANToMMouseError = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CHapticMouseBase construction/destruction

// Push-thru synchronization data
HANDLE CHapticMouseBase::m_hPushThruEvent = NULL;
HANDLE CHapticMouseBase::m_hThreadPushThruSignalCatcher = NULL;
// Error event synchronization data
HANDLE CHapticMouseBase::m_ahErrorEvents[ERROR_EVENT_COUNT];
HANDLE CHapticMouseBase::m_hThreadErrorSignalCatcher = NULL;

CHapticMouseBase::CHapticMouseBase()
{
    m_bStartProgram = FALSE;
    m_bInitialized = FALSE;
    m_bMouseInView = FALSE;
    m_bHapticsOn = FALSE;

    // set the default mouse entry mode
    m_hapticEntryMode = PHANTOM_MOUSE_FALL_THRU;
    
    // Initialize the PHANToM Mouse config structure
    m_phantomMouseConfig.bEnableMouse = kPHANToMMouseEnable;
    m_phantomMouseConfig.nMouseScale = kPHANToMMouseScale;
    m_phantomMouseConfig.nMouseAccel = kPHANToMMouseAcceleration;
    m_phantomMouseConfig.bPlaneEnabled = kPHANToMMousePlaneEnabled;
    m_phantomMouseConfig.bAutoPlaneDepth = kPHANToMMouseAutoPlaneDepth;
    m_phantomMouseConfig.nPlaneForce = kPHANToMMousePlaneForce;
    m_phantomMouseConfig.nPlaneDistance = kPHANToMMousePlaneDistance;
    m_phantomMouseConfig.nPlaneAngle = kPHANToMMousePlaneAngle;
    m_phantomMouseConfig.nPlaneDepth = kPHANToMMousePlaneDepth;
    m_phantomMouseConfig.bPushThruEnabled = kPHANToMMousePushThruEnabled;
    m_phantomMouseConfig.nPushThruDepth = kPHANToMMousePushThruDepth;
    strcpy(m_phantomMouseConfig.szPHANToMName, kPHANToMMouseName);

    // Push-thru synchronization data
    m_hPushThruEvent = NULL;
    m_hThreadPushThruSignalCatcher = NULL;

    // Error event synchronization data
    memset(m_ahErrorEvents, NULL, sizeof m_ahErrorEvents);
    m_hThreadErrorSignalCatcher = NULL;

    m_hDriver = INVALID_HANDLE_VALUE;

    memset(m_bufInput, 0, sizeof m_bufInput);
    memset(m_bufOutput, 0, sizeof m_bufOutput);
}

CHapticMouseBase::~CHapticMouseBase()
{
    DeletePushThruThread();
    DeleteErrorThread();
}

void CHapticMouseBase::EnablePHANToMMouse(BOOL bEnable)
{
    m_bPHANToMMouseEnabled = bEnable;
    m_phantomMouseConfig.bEnableMouse = bEnable;
    UpdateMouseConfig();
}

/////////////////////////////////////////////////////////////////////////////
// Worker functions

BOOL CHapticMouseBase::OpenMouseDriver()
{
    if (m_hDriver != INVALID_HANDLE_VALUE)
        return TRUE; // Connection to mouse driver already open

    char    buf[512];
    int     curUnit = 0;    // Unit to use

    sprintf(buf, "\\\\.\\PHANToM_Mouse_Device%d", curUnit);

    // Create a handle to the PHANToM Mouse driver
    m_hDriver = CreateFile(buf, GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, 0, NULL);
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        printf("ERROR: CreateFile returns %0x\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL CHapticMouseBase::EnableMouseDriver(BOOL bEnable)
{
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        printf("ERROR: Invalid Driver Handle\n");
        return FALSE; // No connection to mouse driver
    }

    // Call I/O Control interface in PHANToM Mouse driver
    ULONG   nOutput;
    if (!DeviceIoControl(m_hDriver, PHANTOM_MOUSE_IOCTL_ENABLEFORCES,
                         &bEnable, sizeof bEnable,
                         m_bufOutput, IOCTL_OUTBUF_SIZE,
                         &nOutput, NULL)) {
        printf("ERROR: DeviceIoControl returns %0x\n", GetLastError());
        return FALSE;
    }

    if (!m_hThreadPushThruSignalCatcher)
    {
        // startup the push-thru thread if push-thru mode is enabled
        if (m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU && !CreatePushThruThread())
            return FALSE;
    }
    
    if (!m_hThreadErrorSignalCatcher)
    {
        // startup the error signaling thread
        if (!CreateErrorThread())
            return FALSE;
    }

    return TRUE;
}

BOOL CHapticMouseBase::IsMouseDriverEnabled()
{
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        printf("ERROR: Invalid Driver Handle\n");
        return FALSE; // No connection to mouse driver
    }
    
    // Call I/O Control interface in PHANToM Mouse driver
    ULONG   nOutput;
    if (!DeviceIoControl(m_hDriver, PHANTOM_MOUSE_IOCTL_ISSTARTED,
                         NULL, 0,
                         m_bufOutput, IOCTL_OUTBUF_SIZE,
                         &nOutput, NULL)) {
        printf("ERROR: DeviceIoControl returns %0x\n", GetLastError());
        return FALSE;
    }

    return *((BOOL *) m_bufOutput) ? TRUE : FALSE;
}

BOOL CHapticMouseBase::CloseMouseDriver()
{
    if (m_hDriver == INVALID_HANDLE_VALUE)
        return TRUE; // Connection to mouse driver already closed

    // Close the handle to the PHANToM Mouse driver
    if (!CloseHandle(m_hDriver))
        printf("ERROR: CloseHandle returns %0x\n", GetLastError());
    m_hDriver = INVALID_HANDLE_VALUE;

    return TRUE;
}

BOOL CHapticMouseBase::UpdateMouseConfig()
{
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        printf("ERROR: Invalid Driver Handle\n");
        return FALSE; // No connection to mouse driver
    }

    KPHANToMMouseConfig kernelMouseConfig;
    
    ConvertMouseConfig(m_phantomMouseConfig, kernelMouseConfig);
    
    // Call I/O Control interface in PHANToM Mouse driver
    ULONG   nOutput;
    if (!DeviceIoControl(m_hDriver, PHANTOM_MOUSE_IOCTL_CONFIG,
        &kernelMouseConfig, sizeof kernelMouseConfig,
        m_bufOutput, IOCTL_OUTBUF_SIZE,
        &nOutput, NULL)) {
        printf("ERROR: DeviceIoControl returns %0x\n", GetLastError());
        return FALSE;
    }
        
    return TRUE;
}

BOOL CHapticMouseBase::SetWhichButton(UCHAR buttonMask)
{
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        printf("ERROR: Invalid Driver Handle\n");
        return FALSE; // No connection to mouse driver
    }

    // store the current button mask to allow bitwise operations
    m_buttonMask = buttonMask;

    // Call I/O Control interface in PHANToM Mouse driver
    ULONG   nOutput;
    if (!DeviceIoControl(m_hDriver, PHANTOM_MOUSE_IOCTL_SETBUTTON,
        &buttonMask, sizeof buttonMask,
        m_bufOutput, IOCTL_OUTBUF_SIZE,
        &nOutput, NULL)) {
        printf("ERROR: DeviceIoControl returns %0x\n", GetLastError());
        return FALSE;
    }
    
    return TRUE;
}

void CHapticMouseBase::SetHapticEntryMode(HapticEntryMode mode)
{
    HapticEntryMode prevMode = m_hapticEntryMode;

    // Set the new haptic entry mode
    m_hapticEntryMode = mode;

    // Enable reporting of the push thru event if the mouse driver is on
    if (IsMouseDriverEnabled() && m_hapticEntryMode == PHANTOM_MOUSE_PUSH_THRU)
    {
    	if (!EnablePushThruMode(TRUE))
            m_hapticEntryMode = prevMode; // if an error occurred, don't change the entry mode
        else if (prevMode == PHANTOM_MOUSE_PUSH_THRU)
            EnablePushThruMode(FALSE);
    }
}

void CHapticMouseBase::ConvertMouseConfig(PHANToMMouseConfig &userConfig,
                                          KPHANToMMouseConfig &kernelConfig)
{
    // make a direct copy of the mouse enable flag
    kernelConfig.bEnableMouse = userConfig.bEnableMouse;

    // convert the mouse scale from float to UNICODE
    swprintf(kernelConfig.pMouseScale, L"%.2f", userConfig.nMouseScale);

    // convert the mouse accel from float to UNICODE
    swprintf(kernelConfig.pMouseAccel, L"%.2f", userConfig.nMouseAccel);

    // make a direct copy of the plane enabled flag
    kernelConfig.bPlaneEnabled = userConfig.bPlaneEnabled;

    // make a direct copy of the auto plane depth flag
    kernelConfig.bAutoPlaneDepth = userConfig.bAutoPlaneDepth;

    // convert the plane depth from float to UNICODE
    swprintf(kernelConfig.pPlaneDepth, L"%.2f", userConfig.nPlaneDepth);

    // convert the plane angle from float to UNICODE
    swprintf(kernelConfig.pPlaneAngle, L"%.2f", userConfig.nPlaneAngle);

    // convert the plane force from float to UNICODE
    swprintf(kernelConfig.pPlaneForce, L"%.2f", userConfig.nPlaneForce);

    // convert the plane distance from float to UNICODE
    swprintf(kernelConfig.pPlaneDistance, L"%.2f", userConfig.nPlaneDistance);        

    // make a direct copy of the push thru enabled flag
    kernelConfig.bPushThruEnabled = userConfig.bPushThruEnabled;
    
    // convert the push thru depth from float to UNICODE
    swprintf(kernelConfig.pPushThruDepth, L"%.2f", userConfig.nPushThruDepth);
    
    // convert the PHANToM config name from ASCII to UNICODE
    USES_CONVERSION;
    wcscpy(kernelConfig.szPHANToMName, A2W(userConfig.szPHANToMName));
} 

/////////////////////////////////////////////////////////////////////////////
// CHapticMouseBase push-thru behaviors

BOOL CHapticMouseBase::CreatePushThruThread()
{
    DWORD   threadIdSignalCatcher = NULL;
    BOOL    bRet = FALSE;

    // Open the event object to be signaled when push-thru has taken place
    m_hPushThruEvent = OpenEvent(SYNCHRONIZE, FALSE, "PHANToMMousePushThruEvent");
    if (!m_hPushThruEvent)
        goto error;     // Error accessing client event

    // Create a thread to wait on the push-thru event.  This thread will call
    // the method to disable the mouse driver and enable haptics in the application
    m_hThreadPushThruSignalCatcher = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) PushThruSignaler,
                                                     this, CREATE_SUSPENDED, &threadIdSignalCatcher);
    // Check if the thread was created
    if (!m_hThreadPushThruSignalCatcher)
        goto error;     // Error creating push-thru signaler thread

    // Set the thread priority to be the desired level (any value between
    // THREAD_PRIORITY_TIME_CRITICAL and THREAD_PRIORITY_IDLE will do)
    if (!SetThreadPriority(m_hThreadPushThruSignalCatcher, THREAD_PRIORITY_NORMAL))
        goto error;     // Error setting thread priority

    // Success
    bRet = TRUE;

error:
    if (!bRet)
        DeletePushThruThread();

    return bRet;
}

BOOL CHapticMouseBase::DeletePushThruThread()
{
    if (m_hPushThruEvent) {
        CloseHandle(m_hPushThruEvent);
        m_hPushThruEvent = NULL;
    }

    if (m_hThreadPushThruSignalCatcher) {
        SuspendThread(m_hThreadPushThruSignalCatcher);
        CloseHandle(m_hThreadPushThruSignalCatcher);
        m_hThreadPushThruSignalCatcher = NULL;
    }

    return TRUE;
}

BOOL CHapticMouseBase::EnablePushThruMode(BOOL bEnable)
{
    BOOL bRet = FALSE;

    // Make sure the push thru thread was created first
    if (!m_hThreadPushThruSignalCatcher)
        return FALSE;

    if (bEnable) 
    {
        // Enable the push-thru signaler thread and event triggering
        while (ResumeThread(m_hThreadPushThruSignalCatcher) > 0)
            ;
        
        m_phantomMouseConfig.bPushThruEnabled = TRUE;
        if (IsMouseDriverEnabled())
            UpdateMouseConfig();
        bRet = TRUE;
    }
    else 
    {
        // Disable the push-thru signaler thread and event triggering
        SuspendThread(m_hThreadPushThruSignalCatcher);
        m_phantomMouseConfig.bPushThruEnabled = FALSE;
        if (IsMouseDriverEnabled())
            UpdateMouseConfig();
        bRet = TRUE;
    }

    if (!bRet)
        DeletePushThruThread();

    return bRet;
}

DWORD CHapticMouseBase::PushThruSignaler(void *pData)
{
    CHapticMouseBase *pThis = (CHapticMouseBase *) pData;

    while (TRUE) {
        if (WaitForSingleObject(m_hPushThruEvent, INFINITE) != WAIT_OBJECT_0)
            break;      // Error waiting for event, get out

        // Post a Windows message about the push-thru event
        pThis->PostMessage(WM_PHANTOMMOUSEPUSHTHRU);
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CHapticMouseBase error event behaviors

BOOL CHapticMouseBase::CreateErrorThread()
{
    DWORD   threadIdSignalCatcher = NULL;
    BOOL    bRet = FALSE;

    // Open the event objects to be signaled when mouse driver errors occur
    for (int i = 0; i < ERROR_EVENT_COUNT; i++) {
        char szErrorEventName[128];
        sprintf(szErrorEventName, "PHANToMMouseErrorEvent%d", i);
        m_ahErrorEvents[i] = OpenEvent(SYNCHRONIZE, FALSE, szErrorEventName);
        if (!m_ahErrorEvents[i])
            goto error;     // Error accessing error event
    }

    // Create a thread to wait on the error events
    m_hThreadErrorSignalCatcher = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ErrorSignaler,
                                               this, CREATE_SUSPENDED, &threadIdSignalCatcher);
    // Check if the thread was created
    if (!m_hThreadErrorSignalCatcher)
        goto error;     // Error creating error signaler thread

    // Set the thread priority to be the desired level (any value between
    // THREAD_PRIORITY_TIME_CRITICAL and THREAD_PRIORITY_IDLE will do)
    if (!SetThreadPriority(m_hThreadErrorSignalCatcher, THREAD_PRIORITY_NORMAL))
        goto error;     // Error setting thread priority

    // Start up the error signaler thread
    if (ResumeThread(m_hThreadErrorSignalCatcher) != 1)
        goto error;     // Error starting error signaler thread

    // Success
    bRet = TRUE;

error:
    if (!bRet)
        DeleteErrorThread();

    return bRet;
}

BOOL CHapticMouseBase::DeleteErrorThread()
{
    for (int i = 0; i < ERROR_EVENT_COUNT; i++)
        if (m_ahErrorEvents[i]) {
            CloseHandle(m_ahErrorEvents[i]);
            m_ahErrorEvents[i] = NULL;
        }

    if (m_hThreadErrorSignalCatcher) {
        SuspendThread(m_hThreadErrorSignalCatcher);
        CloseHandle(m_hThreadErrorSignalCatcher);
        m_hThreadErrorSignalCatcher = NULL;
    }

    return TRUE;
}

DWORD CHapticMouseBase::ErrorSignaler(void *pData)
{
    CHapticMouseBase *pThis = (CHapticMouseBase *) pData;

    const int kPHTM_WARM_MOTORS = -1;
    const int kPHTM_MAX_FORCE = -2;
    const int kPHTM_DEV_FAULT = -4;
    const int kPHTM_EXCEEDED_MAX_VELOCITY = -13;
    BOOL result = FALSE;

    while (TRUE) {
        DWORD dwErrorEvent = WaitForMultipleObjects(ERROR_EVENT_COUNT, m_ahErrorEvents, FALSE, INFINITE);
        if (dwErrorEvent == WAIT_FAILED)
            break;      // Error waiting for event, get out

        char szMessage[512];
        LPCSTR apszErrorNames[] = {
                    "PHTM_OK",                      // 0
                    "PHTM_WARM_MOTORS",             // -1
                    "PHTM_MAX_FORCE",               // -2
                    "PHTM_BAD_CHANNEL_SELECT",      // -3
                    "PHTM_DEV_FAULT",               // -4
                    "PHTM_BAD_FILE",                // -5
                    "PHTM_BAD_FILE_NAME",           // -6
                    "PHTM_MEMORY_ERROR",            // -8
                    "PHTM_DRIVER_ERROR",            // -10
                    "PHTM_BAD_ID",                  // -11
                    "PHTM_FORCES_DISABLED",         // -12
                    "PHTM_EXCEEDED_MAX_VELOCITY",   // -13
                    "PHTM_ILLEGAL_UPDATE_RATE",     // -14
                    "PHTM_ILLEGAL_MAX_VELOCITY",    // -15
                    "PHTM_NO_RESET",                // -16
                    "PHTM_SAFETY_DRIVER_ERROR",     // -17
                    "PHTM_BAD_ADDRESS",             // -18
                    "PHTM_NO_MMAP",                 // -19
                    "PHTM_NO_CARRIER",              // -20
                    "PHTM_CARD_IN_USE",             // -21
                    "PHTM_BAD_CARD_TYPE",           // -22
                    "PHTM_NO_CARD",                 // -23
                    "PHTM_BAD_SLOT",                // -24
                    "PHTM_NO_DRIVER",               // -25
                    "PHTM_NO_TIMER",                // -26
                    "PHTM_EVENT_NOT_OWNED",         // -27
                    "PHTM_COMM_ERROR",              // -28
                    };

        // Adjust error value
        DWORD   dwError = dwErrorEvent - WAIT_OBJECT_0;
        sprintf(szMessage, "Mouse driver reports error -%d (%s)\n", dwError, apszErrorNames[dwError]);

        // disallow calls to EnterHaptics or LeaveHaptics until error is resolved
        pThis->m_bPHANToMMouseError = TRUE;
        
        AfxMessageBox(szMessage, MB_OK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);

        // depending on the error type, attempt to restart the mouse driver
        dwError *= -1; // make the error number negative again
        if ((dwError == kPHTM_WARM_MOTORS) || (dwError == kPHTM_MAX_FORCE) ||
            (dwError == kPHTM_EXCEEDED_MAX_VELOCITY) || (dwError == kPHTM_DEV_FAULT)) {

            sprintf(szMessage, "Attempting to restart mouse driver...");

            if (pThis->IsMouseDriverEnabled()) {
                if (pThis->EnableMouseDriver(FALSE))
                    result = pThis->EnableMouseDriver(TRUE);
            } else
                result = pThis->EnableMouseDriver(TRUE);

            strcat(szMessage, result ? "ok\n" : "failed\n");
            OutputDebugString(szMessage);
        }

        // if the error was resolved, disable the mouse error flag
        if (result)
            pThis->m_bPHANToMMouseError = FALSE;
        else
            AfxMessageBox("An unrecoverable error has occurred in the PHANToM Mouse driver",
            MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);

    }

    return 0;
}

BOOL CHapticMouseBase::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CHapticMouseBase::OnDraw(CDC* pDC)
{
}
