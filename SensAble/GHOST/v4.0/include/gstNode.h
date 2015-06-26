//=============================================================================
//   Filename : gstNode.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Base class for all nodes in the scene graph
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_NODE
#define GST_NODE	

#include <gstBasic.h>
#include <gstNodeName.h>

class gstTransform;
struct GHOST_DLL_IMPORT_EXPORT gstNodeList
{
	gstTransform *node;
	gstNodeList *next;
};

// Base class for scene graph nodes.
class GHOST_DLL_IMPORT_EXPORT gstNode
{
public:

	// Destructor.
	virtual ~gstNode();

	// Clone.
	virtual gstNode *Clone() const = 0;

	// Casting.
	virtual gstTransform *AsTransform() {return 0;}

	// static: get type id of this class
	static gstType getClassTypeId()
	{
		gstAssignUniqueId(gstNodeClassTypeId);
		return gstNodeClassTypeId;
	}

	// virtual form of getClasstypeId
	virtual gstType		getTypeId() const { return getClassTypeId(); }

    // static: Return TRUE if class is of the given type
    // or is derived from that type 
	virtual gstBoolean	isOfType(gstType type) const {return staticIsOfType(type);}

	// virtual form of staticIsOfType 
	static gstBoolean staticIsOfType(gstType type)
	{
		if (type == getClassTypeId()) return TRUE;
		else return FALSE;
	}

	// Returns TRUE if node is in scene graph.
	gstBoolean			getInSceneGraph() const { return inSceneGraph; }

	// FOR_GHOST_EXTENSION
	// Called when object is put in scene graph.
	virtual void		putInSceneGraph() { inSceneGraph = TRUE; }

	// FOR_GHOST_EXTENSION
	// Called when object is removed from scene graph.
	virtual void		removeFromSceneGraph() { inSceneGraph = FALSE; }

	// Set name of node.
	virtual void	   setName(const gstNodeName &name) { nodeName = name; }

	// Returns name of node.
	virtual gstNodeName 
					   getName() const { return nodeName; }

	// Returns first node in subtree with nodeName = name.
	// Otherwise, returns NULL
	virtual gstNode	   *getByName(const gstNodeName &name) {
		if (nodeName == name) return this;
		return NULL;
	}

protected:

	// This class is intended as a base class only, the constructors
	// are protected so that instances can not be created.
	gstNode ();
	gstNode (const gstNode &);
	gstNode (const gstNode *);

	static gstType		gstNodeClassTypeId;
	gstBoolean			inSceneGraph;

	gstNodeName			nodeName;
};
	
#endif
