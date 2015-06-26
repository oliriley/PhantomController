//=============================================================================
//   Filename : gstPHANToMTranslation.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Phantom controls geometry of subgraph through translation.
//              Dynamic forces are sent back to PHANToM
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_PHANTOM_TRANSLATION
#define GST_PHANTOM_TRANSLATION

#include <gstDllExport.h>
#include <gstPHANToMDynamic.h>


// Translation-only PHANToM dynamic class.  This sets the PHANToM to
// control the geometry of its subgraph through translation
// only.  Dynamic forces are sent back to PHANToM to add force 
// feedback information to translation.  Note that mass and damping
// can be modified through methods inherited from gstDynamic.
class GHOST_DLL_IMPORT_EXPORT gstPHANToMTranslation : public gstPHANToMDynamic
{
    GHOST_INHERIT_HEADER(gstPHANToMTranslation, gstPHANToMDynamic);

public:

	// Constructor.
	gstPHANToMTranslation(char *);

	// Destructor.
	virtual ~gstPHANToMTranslation() {}


gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// When button has velocity or a force, this method
	// does Euler integration of button state and
	// creates button events to be processed by
	// gstTransform::updateEvents().
	void updateDynamics();
};

#endif
