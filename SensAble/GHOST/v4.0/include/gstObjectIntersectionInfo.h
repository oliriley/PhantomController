//=============================================================================
//   Filename : gstObjectIntersectionInfo.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Simple structure used in object intersection tests
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTOBJECTINTERSECTIONINFO_INCLUDED)
#define GSTOBJECTINTERSECTIONINFO_INCLUDED


// Structure for various intersection tests
class gstObjectIntersectionInfo
{
  public:

	int test;

	typedef enum resultValues
        {RV_NONE, RV_OVERLAPPING, RV_ENCLOSED, RV_ENCLOSING} IntersectionType;
};


#endif  // GSTOBJECTINTERSECTIONINFO_INCLUDED
