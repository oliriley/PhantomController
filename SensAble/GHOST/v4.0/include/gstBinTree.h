//=============================================================================
//   Filename : gstBinTree.h
//    Project : GHOST library v3.0
//    Authors : Tim Gallagher
//    Revised :
// ----------------------------------------------------------------------------
// Description: gstBinTree is a binary tree type of spatial partition
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef _GST_BINTREE_H_
#define _GST_BINTREE_H_

#include <gstDllExport.h>

#include <gstSpatialPartition.h>
#include <list.h>
#include <set.h>

#define _GST_STUPID_SPACE_WASTE_ 0

// Added by CTarr for Sunil's performance benchmark testing of intersections
#ifdef PERFTEST_COUNT_NODE_TRAVERSALS
GHOST_DLL_IMPORT_EXPORT int getNodeTraversalCount();
GHOST_DLL_IMPORT_EXPORT void zeroNodeTraversalCount();
#endif


class gstBT_Node;
class gstBT_GeomObj;
class gstBinTree;

typedef set<gstSpatialObject*, less<gstSpatialObject*> > gstSpatialObjectSet;
typedef set<gstBT_Node*, less<gstBT_Node*> > gstBT_NodeSet;
typedef set<gstBT_GeomObj*, less<gstBT_GeomObj*> > gstBT_GeomObjSet;

// Wrapper class for gstSpatialObjects so that they all have nice
// generic properties to be handled by the partition class.
class GHOST_DLL_IMPORT_EXPORT gstBT_GeomObj
{
    public:
        gstBT_GeomObj(gstSpatialObject *o);
        ~gstBT_GeomObj();

        // Access minimum point of bounding box.
        gstPoint &minPt() {return min_;}

        // Access maximum point of bounding box.
        gstPoint &maxPt() {return max_;}

        // Access contained object.
        gstSpatialObject *obj() {return obj_;}
#if(_GST_STUPID_SPACE_WASTE_)
        //gstBT_NodeSet &nodes() {return nodes_;}
#endif

        void inc() {++i_;}
        void dec() {--i_;}

    private:
        gstBT_GeomObj(); // try not to use
        gstPoint min_;
        gstPoint max_;
        gstSpatialObject *obj_;
        int i_;
#if(_GST_STUPID_SPACE_WASTE_)
        //gstBT_NodeSet nodes_;
#endif
};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstBT_GeomObj *, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstBT_GeomObj *, class __default_alloc_template<1,0> >;
#endif


typedef list<gstBT_GeomObj*> gstBT_GeomObjList;

// Class to represent an element of the partition traversal stack.
class GHOST_DLL_IMPORT_EXPORT gstBT_StackElem
{
    friend class gstBT_Stack;

    public:
        gstBT_StackElem() :    node_(0), min_(0.0), max_(0.0) {}
        ~gstBT_StackElem() {}

    private:
        gstBT_Node *node_;
        double min_;
        double max_;
};

// Class to linearize the traversal of the gstBinTree.
// WARNING! This class does NOT check to see if it is exceeding its
// depth bounds. This is for speed. Be careful.
class GHOST_DLL_IMPORT_EXPORT gstBT_Stack
{
    public:
        // Ctor.
        // Initializes and allocates memory for stack.
        gstBT_Stack(int depth = 50) : index_(-1), depth_(depth)
        {
            arr_ = new (gstBT_StackElem(*[depth]));
            for(int i=0; i<depth; ++i) {
                arr_[i] = new gstBT_StackElem;
            }
        }
        // Dtor.
        // Deletes objects in stack and deletes stack itself.
        ~gstBT_Stack()
        {
            for(int i=0; i<depth_; ++i) {
                delete arr_[i];
            }
            delete [] arr_;
        }
        // push a new node and bounds onto the stack
        void push(gstBT_Node *node, double min, double max)
        {
            ++index_;
            gstBT_StackElem *se = arr_[index_];

            se->node_ = node;
            se->min_ = min;
            se->max_ = max;
        }
        // pop top node off of stack, returning node and bounds
        gstBT_Node *pop(double &min, double &max)
        {
            gstBT_StackElem *se = arr_[index_];
            --index_;

            min = se->min_;
            max = se->max_;
            gstBT_Node *ret = se->node_;

            se->node_ = 0;

            return ret;
        }
        // initialize the stack by emptying it
        void init()
        {
            double min, max;
            while(!empty()) {
                pop(min, max);
            }
        }
        // is the stack empty?
        int empty() {return (index_ == -1);}

    private:
        int index_;
        int depth_;
        gstBT_StackElem **arr_;
};

// This class represents nodes within the gstBinTree class.
class GHOST_DLL_IMPORT_EXPORT gstBT_Node
{
    public:
        gstBT_Node();
        ~gstBT_Node();

        // active axis access method
        int &axis() {return axis_;}

        // depth of this node
        int &depth() {return depth_;}

        // access minimum extent point
        gstPoint &minPt() {return min_;}

        // access minimum extent point
        gstPoint &maxPt() {return max_;}

        // get the ith child node. note unsafe behavior.
        gstBT_Node *getChild(int i) {return child_[i];}

        // set the ith child node. note unsafe behavior.
        void setChild(int i, gstBT_Node *child) {child_[i] = child;}

        // sort both children into a "near" and "far" division
        // relative to a particular axis and plane
        void getChildren(
            int axis,
            const gstPoint &origin,
            gstBT_Node **nearP,
            gstBT_Node **farP);

        // get the parent node
        gstBT_Node *getParent() {return parent_;}

        // set the parent node
        void setParent(gstBT_Node *p) {parent_ = p;}

        // list access methods
        gstBT_GeomObjList &members() {return list_;}

        // get the partition tree which contains this node
        gstBinTree *getTree() {return tree_;}

        // set the partition tree which contains this node
        void setTree(gstBinTree *t) {tree_ = t;}

    private:
        int axis_;                // active axis of node 0,1,2 = x,y,z
        int depth_;                // depth of this node in the tree
        gstPoint min_;            // min point of partitioned space
        gstPoint max_;            // max point of partitioned space
        gstBT_Node *parent_;    // parent node
        gstBT_Node *child_[2];    // left and right child
        gstBT_GeomObjList list_;// list of polys at this level of tree
        gstBinTree *tree_;        // tree containing this node
};

// A particular type of spatial partition.
// This is a binary tree almost as described in various parts of
// the Graphics Gems series. With minor modifications and bug fixes.
class GHOST_DLL_IMPORT_EXPORT gstBinTree : public gstSpatialPartition
{
    public:
        gstBinTree(gstSpatialObject *);
        virtual ~gstBinTree();

        // initialize the tree
        virtual void init();

        // void the tree
        void wipe();

        // remove object from tree to prepare for modification
        virtual gstBoolean beginModify(gstSpatialObject *);

        // reinsert object into tree after modification
        virtual gstBoolean endModify(gstSpatialObject *);

        // return union of bounding boxes of contained objects. cached.
        virtual gstBoundingBox getBoundingBox() {return bbox_;}

        // return bounding sphere of bounding box
        virtual gstBoundingSphere getBoundingSphere()
        {
            return getBoundingBox().getBoundingSphere();
        }

        // type of intersection
        enum Type {
            In,            // intersection going in
            Out,        // intersection going out
            Either        // either direction
        };

        // gstBoundingCube intersection methods
        virtual gstObjectIntersectionInfo::IntersectionType intersect_BC(
            const gstBoundingCube &cube);

        // interface for gstLineIntersectionInfoFirst_Param intersection methods
        gstLineIntersectionInfo::IntersectionType BT_intersect_LS_P(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_Param &intersectionInfo,
            Type type);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_LS_P(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_Param &intersectionInfo);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_LS_P(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_Param &intersectionInfo);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_P(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_Param &intersectionInfo);

        // interface for gstLineIntersectionInfoFirst_ParamSpatObj intersection methods
        gstLineIntersectionInfo::IntersectionType BT_intersect_LS_PSO(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo,
            Type type);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstIn_LS_PSO(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstInOut_LS_PSO(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

        // see gstSpatialObject
        virtual gstLineIntersectionInfo::IntersectionType intersectFirstOut_LS_PSO(
            const gstLineSegment &lineSegment,
            gstLineIntersectionInfoFirst_ParamSpatObj &intersectionInfo);

        // the principal work horse intersection function
        gstBoolean treeIntersect(
            const gstLineSegment &lseg,        // line segment to intersect with
            gstBT_GeomObj **obj,            // contained object hit
            gstLineIntersectionInfoFirst_Param &intersectionInfo,
            Type type);                        // type of intersection

        // low level intersection bounding routine
        gstBoolean boxIntersect(
            const gstLineSegment &lseg,
            const gstVector &lseg_unit_dir,
            const gstPoint &min,
            const gstPoint &max, 
            double &returnMin,
            double &returnMax);

        // low level object collection intersection routine
        gstBoolean objIntersect(
            const gstLineSegment &lseg,        // line segment to intersect with
            double min,                        // minimum parameter value
            double max,                        // maximum parameter value
            gstBT_GeomObjList &objList,        // list of objects
            gstBT_GeomObj **obj,            // hit object
            gstLineIntersectionInfoFirst_Param &intersectionInfo,
            Type type);                        // type of intersection

        // retrieve those objs whose bboxes intersect this bbox
        gstBoolean bboxIntersect(
            const gstBoundingBox &bbox,        // a bounding box
            gstBT_NodeSet &nodes);            // those nodes which intersect the box

        // return the minimum point of the partition space
        gstPoint getMin() const {return min_;}

        // return the maximum point of the partition space
        gstPoint getMax() const {return max_;}

        // set the minimum point of the partition space
        void setMin(const gstPoint &m) {min_ = m;}

        // set the maximum point of the partition space
        void setMax(const gstPoint &m) {max_ = m;}

        // does the partition box contain any of the box passed in
        gstBoolean bboxContained(const gstBoundingBox &bbox);

        // method to access parent poly collection
        gstSpatialObject *getParent() {return parent_;}

        // access to child nodes methods
        gstBT_Node *root() {return root_;}

        // return the object list
        gstBT_GeomObjList &members() {return list_;}

        // add an object to a node
        void addGeomToNode(gstBT_GeomObj *obj, gstBT_Node *node);

        // test if an object is in a node
        gstBoolean isGeomInNode(gstBT_GeomObj *obj, gstBT_Node *node);

        // remove an object from a node
        void removeGeomFromNode(gstBT_GeomObj *obj, gstBT_Node *node);

        // insert this object into this node, subdividing if necessary
        void insertGeomIntoNode(gstBT_GeomObj *obj, gstBT_Node *node);

        // insert this gstSpatialObject into the tree
        gstBoolean insert_object(gstSpatialObject *so);

        // get the internal traversal stack
        gstBT_Stack *getStack() {return stack_;}

        // initialize the internal traversal stack
        void initStack();

        // push a node onto the stack
        void pushStack(gstBT_Node *node, double min, double max);

        // pop a node off the stack returning the newest top item
        void popStack(gstBT_Node **node, double &min, double &max);

        // get the overall partition space scale factor
        double getScale() const {return scale_;}

        // get whether to use a hardwired partition space
        gstBoolean getUseHardBox() const {return use_hard_box_;}

        // get the hardwaired partition space
        gstBoundingBox getHardBox() const {return hard_box_;}

        // set the overall partition space scale factor
        void setScale(double s) {scale_ = s;}

        // set whether to use a hardwired partition space
        void setUseHardBox(gstBoolean u) {use_hard_box_ = u;}

        // set the hardwaired partition space
        void setHardBox(gstBoundingBox b) {hard_box_ = b;}

        // get the maximum tree depth for the class
        static int getMaxTreeDepth() {return max_tree_depth_;}

        // get the maximum number of objects in a leaf node of the tree
        static int getMaxListLength() {return max_list_length_;}

        // get the precision of the tree
        static double getEpsilon() {return epsilon_;}

        // get the overlap between node bounding boxes as a percentage of the box size
        static double getPartitionOverlapTol() {return part_overlap_tol_;}

        // get the maximum tree depth for the class
        static void setMaxTreeDepth(int m) {max_tree_depth_ = m;}

        // get the maximum number of objects in a leaf node of the tree
        static void setMaxListLength(int m) {max_list_length_ = m;}

        // get the precision of the tree
        static void setEpsilon(double e) {epsilon_ = e;}

        // get the overlap between node bounding boxes as a percentage of the box size
        static void setPartitionOverlapTol(double t) {part_overlap_tol_ = t;}

    protected:
        void subdivide(gstBT_Node *node);

        // Recursive bounding box intersection routine used by bboxIntersect
        void bbox_intersect(
            const gstBoundingBox &bbox,
            gstBT_NodeSet &nodes,
            gstBT_Node *node);

        // get the set of objects currently being modified
        gstSpatialObjectSet &getModifySet() {return modify_set_;}

    private:
        // do not use
        gstBinTree();

        void setBoundingBox(const gstBoundingBox bbox) {bbox_ = bbox;}
        void get_spat_objs(gstBT_Node*, gstSpatialObjectSet &s);

        gstPoint min_;            // min point of partitioned space
        gstPoint max_;            // max point of partitioned space
        gstBT_Node *root_;        // root of tree
        gstBoundingBox bbox_;    // tree node bounding box    
        gstBT_GeomObjList list_;// list of polys at this level of tree
        gstBT_Stack *stack_;        // linear time traversal stack
        gstSpatialObject *parent_;    // parent object
        gstSpatialObjectSet modify_set_; // set of objects being modified

        double scale_; // how much bigger should the 
                       // partition space be than the
                       // minimum bounding box of the data
        gstBoolean use_hard_box_; // should we use a hard wired box?
        gstBoundingBox hard_box_; // the hard wired box to use

        static int max_tree_depth_; // how deep can the tree get?
        static int max_list_length_;// max number of objs in a leaf node?
        static double epsilon_;        // min distance comparison
        static double part_overlap_tol_; // partition overlap tolerance    
};

class ostream;
void GHOST_DLL_IMPORT_EXPORT gstBT_PrintTree(ostream &os, gstBT_Node *node, int level = 0);

#endif // _GST_BINTREE_H_
