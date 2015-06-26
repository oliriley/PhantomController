//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLConvert.h
//
// Rev: 1.0
//
// Description: Utility functions for the VRML parsing. These 
//              function attempt to turn various text strings into
//              appropriate types.
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_CONVERT_H_
#define _GST_VRML_CONVERT_H_

#include <gstBasic.h>
class gstPoint;
class gstVector;

enum gstVRMLConvertStatus
{
	gst_VCS_Success,
	gst_VCS_Failure
};

int gstVRMLNoConvertError(gstVRMLConvertStatus status);
gstVRMLConvertStatus gst_SFInt32_To_int(int &i, const char *text);
gstVRMLConvertStatus gst_SFInt32_To_gstBoolean(gstBoolean &b, const char *text);
gstVRMLConvertStatus gst_SFFloat_To_double(double &d, const char *text);
gstVRMLConvertStatus gst_SFVec3F_To_gstPoint(gstPoint &p, const char *text);
gstVRMLConvertStatus gst_SFRotation_To_gstVector_double(
	gstVector &v,
	double &angle,
	const char *text);

#endif // _GST_VRML_CONVERT_H_
