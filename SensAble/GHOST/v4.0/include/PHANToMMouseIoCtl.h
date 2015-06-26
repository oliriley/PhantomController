//
// PHANToMMouseIoCtl.h
// Control codes for the PHANToM_Mouse driver
//
// Copyright (c) 1998  SensAble Technologies, Inc.
// All rights reserved
//

#ifndef __PHANTOM_MOUSE_IOCTL_H__
#define __PHANTOM_MOUSE_IOCTL_H__

// This structure is a mirror of PHANToMMouseConfig, except it stores all of its
// values as Unicode strings. This is the safest way of getting data into the
// kernel, since the kernel doesn't directly support floats

#define FLOAT_STRING_LENGTH     32

typedef struct {
    BOOL     bEnableMouse;
    wchar_t  pMouseScale[FLOAT_STRING_LENGTH];
    wchar_t  pMouseAccel[FLOAT_STRING_LENGTH];
    BOOL     bPlaneEnabled;
    BOOL     bAutoPlaneDepth;
    wchar_t  pPlaneDepth[FLOAT_STRING_LENGTH];
    wchar_t  pPlaneAngle[FLOAT_STRING_LENGTH];
    wchar_t  pPlaneForce[FLOAT_STRING_LENGTH];
    wchar_t  pPlaneDistance[FLOAT_STRING_LENGTH];
    BOOL     bPushThruEnabled;
    wchar_t  pPushThruDepth[FLOAT_STRING_LENGTH];
    wchar_t  szPHANToMName[128];
} KPHANToMMouseConfig;


#define MOUSE_LEFT_BUTTON_MASK   0x01
#define MOUSE_MIDDLE_BUTTON_MASK 0x02
#define MOUSE_RIGHT_BUTTON_MASK  0x04

#define PHANTOM_MOUSE_EVENT     0x050565    // All generated mouse events are tagged with this value

#define FILE_DEVICE_PHANTOM     50000       // Arbitrary, in the range 32768-65535 (see ntddk.h)

#define IOCTL_INBUF_SIZE        512         // Max data size for device input (USER -> KERNEL)
#define IOCTL_OUTBUF_SIZE       512         // Max data size for device output (KERNEL -> USER)

#define PHANTOM_MOUSE_IOCTL_RESTART         CTL_CODE(FILE_DEVICE_PHANTOM, 0xA00, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PHANTOM_MOUSE_IOCTL_ENABLEFORCES    CTL_CODE(FILE_DEVICE_PHANTOM, 0xA01, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PHANTOM_MOUSE_IOCTL_ISSTARTED       CTL_CODE(FILE_DEVICE_PHANTOM, 0xA02, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PHANTOM_MOUSE_IOCTL_CONFIG          CTL_CODE(FILE_DEVICE_PHANTOM, 0xA03, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PHANTOM_MOUSE_IOCTL_SETBUTTON       CTL_CODE(FILE_DEVICE_PHANTOM, 0xA04, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif  // __PHANTOM_MOUSE_IOCTL_H__


