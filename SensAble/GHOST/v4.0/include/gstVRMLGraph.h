//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLGraph.h
//
// Rev: 1.0
//
// Description: Principle work horse class for VRML parsing.
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_GRAPH_H_
#define _GST_VRML_GRAPH_H_

#include <gstVRMLParse.h>
#include <stack.h>
#include <gstBasic.h>
#include <gstNodeName.h>
#include <gstPoint.h>

class gstTransform;
class gstSeparator;
class gstVRMLGraph;

class gstVRMLGraph
{
	public:
		gstVRMLGraph();
		~gstVRMLGraph();

		gstSeparator *GetRoot() {return root_;}
		gstSeparator *GetCurrentNode() {return current_node_;}
		void SetCurrentNode(gstSeparator *);

		void switchToNonHapticSceneGraph();
		void switchToHapticSceneGraph();
		
		void AddNode(gstTransform *);
		void PushNode(gstSeparator *);
		void PopNode();
		void ParseError();
		void CleanUpAndDie(gstSeparator *sep);
		gstTransform *GetRecent() {return recent_;}
		void CopyNode(const char *name);

		stack<int, deque<int> > &NodeTypeStack() {return node_type_stack_;}
		gstBoolean BeginNode(int nodetype);
		gstBoolean EndNode(int nodetype);

		stack<int, deque<int> > &FieldTypeStack() {return field_type_stack_;}
		gstBoolean BeginField(int fieldtype);
		gstBoolean EndField(int fieldtype);

		gstBoolean FieldValue(const char *fieldvalue);

		gstBoolean Def() {return def_;}
		void SetDef(gstBoolean);
		gstNodeName &DefName() {return def_name_;}
		void SetDefName(const char *name);

	protected:
		void SetRoot(gstSeparator *root);
		void SetRecent(gstTransform *);

	private:
		gstBoolean d_usingNonHapticSceneGraph;
		gstSeparator *root_;
		gstSeparator *d_nonHapticRoot;
		gstTransform *d_recentSave;
		gstSeparator *d_currentSave;
		gstSeparator *current_node_;
		gstTransform *recent_;

		void add_node(gstSeparator *parent, gstTransform *child);

		stack<int, deque<int> > node_type_stack_;
		stack<int, deque<int> > field_type_stack_;
		gstBoolean juggle(gstVRMLParseStatus status, const char *text = "");

		// these are used to handle VRML DEF/USE mechanism
		gstBoolean def_;
		gstNodeName def_name_;

		// the following is used in the run time creation 
		// of the meshes as we red in from file...
		vector<gstPoint> mesh_verts_; // vertices
		vector<gstPoint> &GetPMVerts() {return mesh_verts_;}

		vector<gstPoint> mesh_norms_; // normals
		vector<gstPoint> &GetPMNorms() {return mesh_norms_;}

		vector<vector<int> > mesh_norm_faces_; // collection of normals
		vector<vector<int> > &GetPMNormFaces() {return mesh_norm_faces_;}

		gstBoolean normal_per_vertex_;
		gstBoolean GetNormalPerVertex() const {return normal_per_vertex_;}
		void SetNormalPerVertex(gstBoolean b) {normal_per_vertex_ = b;}

		vector<vector<int> > mesh_faces_; // faces
		vector<vector<int> > &GetPMFaces() {return mesh_faces_;}
		
		void rebuild_mesh();
		void clean_up_mesh();

		gstBoolean need_new_face_;
		gstBoolean &need_new_face() {return need_new_face_;}

		gstBoolean need_new_norm_face_;
		gstBoolean &need_new_norm_face() {return need_new_norm_face_;}

		double crease_angle_;
		double GetCreaseAngle() const {return crease_angle_;}
		void SetCreaseAngle(double a) {crease_angle_ = a;}

};

#endif // _GST_VRML_GRAPH_H_
