//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLParse.h
//
// Rev: 1.0
//
// Description: Access methods for yacc and lex code to comunicate
//              with the VRML graph we are building.
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_PARSE_H_
#define _GST_VRML_PARSE_H_

class ostream;

enum gstVRMLParseStatus
{
	gst_VPS_NULL,
	gst_VPS_BeginNode,
	gst_VPS_EndNode,
    gst_VPS_BeginField,
    gst_VPS_EndField,
    gst_VPS_FieldValue
};

ostream& operator<< (ostream &os, const gstVRMLParseStatus &status);

int gstVRMLBeginNode(int nodetype);
int gstVRMLEndNode(int nodetype);
int gstVRMLBeginField(int fieldtype);
int gstVRMLEndField(int fieldnodetype);
int gstVRMLFieldValue(const char *text);

// used for the VRML DEF/USE keyword interaction
void gstVRMLDefNode();
void gstVRMLDefName(const char *name);
void gstVRMLUseNode(const char *name);

void gstVRMLParseError(int line, const char *msg, const char *text);

#endif // _GST_VRML_PARSE_H_
