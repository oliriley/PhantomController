/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  BinTree.h

Description:

  Implemenation of a bin tree to show view frustum culling
  using the haptic camera view.

******************************************************************************/

#ifndef CXX_BINTREE_h_DEFINE
#define CXX_BINTREE_h_DEFINE

#include <vector>
#include <list>
#include <HDU/hduVector.h>
#include <HDU/hduPlane.h>
#include <HDU/hduBoundBox.h>

using namespace std;

// Vertex (position only).
typedef hduVector3Df Vertex;

struct Triangle
{
    Triangle(int v0, int v1, int v2)
    {
        vert[0] = v0; vert[1] = v1; vert[2] = v2;
    }

    int vert[3];          // indices of vertices compose triangle
    hduVector3Df normal;  // triangle normal
};

// Containers for primitives.
typedef vector<Vertex> VertContainer;
typedef vector<Triangle> TriContainer;

struct Node
{
    int level;              // Level of node in bin tree.
    hduBoundBox3Df box;     // Bounding box of node.
    hduPlane<float> plane;  // Plane that divides children of this node.
    TriContainer* triList;  // Triangles contained in this node.

    struct Node *child[2];  // 0 = negative, 1 = positive child.
};

class BinTree
{
public:
    BinTree(VertContainer* pVertices,
            TriContainer* pTriangles,
            bool isCanonicalScale);

    ~BinTree();

    int numTri() const { return m_numTri; }
    int numTri(const hduVector3Df* frustum) const;

    const hduBoundBox3Df& box() const { return m_pBox; }

    // Drawing functions.
    int traverseAndRender();
    int traverseAndRender(const hduVector3Df* frustum);

    void traverseAndRenderBoxes();
    void traverseAndRenderBoxes(const hduVector3Df* frustum);

private:
    void numTri(const Node* pNode, 
                const hduVector3Df* frustum, 
                int triCount) const;

    // Drawing helper functions.
    void traverseAndRender(const Node* pNode, int& triCount);
    void traverseAndRender(const Node* pNode, 
                           const hduVector3Df* frustum, 
                           int& triCount);

    void traverseAndRenderBoxes(const Node* pNode);
    void traverseAndRenderBoxes(const Node* pNode, 
                                const hduVector3Df* frustum);

    void renderLeaf(const Node* pNode);

    void freeTree(Node* pNode);

    int classifyFrustum(const Node* pNode, const hduVector3Df* frustum) const;

    void constructBinTree(Node* pNode, int maxLeafTri);
    void constructLeaf(Node* pNode);

    hduPlane<float> plane(int level,
                          const hduBoundBox3Df& box) const;
    int evaluateTriangle(const hduPlane<float>& plane,
                         const Triangle& tri);
    void partitionTriangles(Node* pNode);
    void allocChildren(Node* pNode);
    void splitBox(hduBoundBox3Df& negativeBox,
                  hduBoundBox3Df& positiveBox,
                  const hduBoundBox3Df& box,
                  int level);

private:
    int m_numTri;
    hduBoundBox3Df m_pBox; // Bounding box of bin tree.

    Node* m_pRoot; // Root node.
    VertContainer* m_pVertices;
};

#endif // CXX_BINTREE_h_DEFINE

/******************************************************************************/
