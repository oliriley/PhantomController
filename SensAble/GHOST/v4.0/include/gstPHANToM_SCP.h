//=============================================================================
//   Filename : gstPHANToM_SCP.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represent the PHANToM's Surface Contact Point as a node in the
//              scene graph.  Not recommended for normal use, get the SCP from
//              gstPHANToM::getSCP() functions or from PHANToM graphics callback
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_PHANToM_SCP
#define GST_PHANToM_SCP

#include <gstDllExport.h>
#include <gstShape.h>

// Represents the PHANToM's SCP in the scene graph, usually not needed.
// gstPHANToM node's Surface Contact Point (SCP): If the
// PHANToM is in contact with a surface, the SCP
// is the calculated point of contact on the contacted
// surface.  Otherwise, the SCP coincides with the position of 
// the PHANToM.
// Note:  This class is only mean't to be used as a convenience
//        node.  This allows a separate node for the SCP and
//		  another for the gstPHANToM position.  You query the
//        same state from just the gstPHANToM node.
class GHOST_DLL_IMPORT_EXPORT gstPHANToM_SCP : public gstShape
{
    GHOST_INHERIT_HEADER(gstPHANToM_SCP, gstShape);

public:

	// Constructor.
	gstPHANToM_SCP();

	// Constructor.
	gstPHANToM_SCP(const gstPHANToM_SCP *);

	// Constructor.
	gstPHANToM_SCP(const gstPHANToM_SCP &);

	// Destructor.
	virtual ~gstPHANToM_SCP();

	// Clone.
	virtual gstNode *Clone() const {return ClonePHANToM_SCP();}
	gstPHANToM_SCP *ClonePHANToM_SCP() const {return (new gstPHANToM_SCP(*this));}

gstInternal public:

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Called when object is added to scene graph.
	virtual void		putInSceneGraph() {gstTransform::putInSceneGraph();}

	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// Called when object is removed from scene graph.
	virtual void		removeFromSceneGraph() {gstTransform::removeFromSceneGraph();}

};

#endif
