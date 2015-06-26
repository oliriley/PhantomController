//=============================================================================
//   Filename : gstPHANToMRotation.h
//    Project : GHOST library v3.0
//    Authors : Josh Handley
//    Revised : 
// ----------------------------------------------------------------------------
// Description: Phantom controls geometry of subgraph through translation and
//              rotation.  Dynamic forces are sent back to PHANToM
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_PHANTOM_ROTATION
#define GST_PHANTOM_ROTATION

#include <gstDllExport.h>
#include <gstPHANToMDynamic.h>
#include <gstPHANToMTranslation.h>


// Rigid body PHANToM dynamic class.  This sets the PHANToM to
// control the geometry of its subgraph through rotation.  
// Dynamic forces are sent back to PHANToM simulating a rotating
// mass with realistic inertial properties.  Note that Mass and 
// damping are set through the appropriate methods inherited 
// from gstDynamic.
class GHOST_DLL_IMPORT_EXPORT gstPHANToMRotation : public gstPHANToMTranslation
{
    GHOST_INHERIT_HEADER(gstPHANToMRotation, gstPHANToMTranslation);

public:

	// Constructor.
	gstPHANToMRotation(char *);

	// Destructor.
	virtual ~gstPHANToMRotation() {}

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


