/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  BinTree.cpp

Description: 

  Implemenation of a bin tree to show view frustum culling using the haptic 
  camera view.

******************************************************************************/
#include "stdAfx.h"
#include <assert.h>
#include <GL/gl.h>
#include "BinTree.h"

/******************************************************************************
 BinTree constructor.
******************************************************************************/
BinTree::BinTree(VertContainer* pVertices,
                 TriContainer* pTriangles,
                 bool isCanonicalScale) :
    m_numTri(0), m_pRoot(0), m_pVertices(0)
{
    m_pVertices = pVertices;

    if (isCanonicalScale)
    {
        m_pBox.Union(hduVector3Df(-1,-1,-1));
        m_pBox.Union(hduVector3Df(1,1,1));
    }
    else
    {
        // Compute bounding box from verticies.
        for (VertContainer::iterator vert = m_pVertices->begin();
             vert != m_pVertices->end(); ++vert)
        {
            m_pBox.Union((*vert));
        }
    }

    m_pRoot = new Node;
    m_pRoot->triList = pTriangles;

    m_pRoot->level = 0;
    m_pRoot->box = m_pBox;

    m_pRoot->plane = plane(m_pRoot->level, m_pRoot->box);
    
    m_pRoot->child[0] = 0;
    m_pRoot->child[1] = 0;

    const int maxLeafTri = 1024;
    constructBinTree(m_pRoot, maxLeafTri);
}

/******************************************************************************
 ~BinTree destructor.
******************************************************************************/
BinTree::~BinTree()
{
    delete m_pVertices;
    freeTree(m_pRoot);
}


void BinTree::constructLeaf(Node* pNode)
{
    // Node is leaf node, compress the TriContainer (vector may consume more 
    // memory than the minimum required to hold the node triangles).
    TriContainer* pCompressedTriList = new TriContainer(*(pNode->triList));
    assert(pCompressedTriList->size() == pCompressedTriList->capacity());

    delete pNode->triList;
    pNode->triList = pCompressedTriList;
    
    m_numTri += pNode->triList->size();
}

/******************************************************************************
 Builds bin tree to depth maxDepth.
******************************************************************************/
void BinTree::constructBinTree(Node* pNode, int maxLeafTri)
{
    if (!pNode) return;

    if (pNode->triList->size() > maxLeafTri)
    {
        allocChildren(pNode);
        pNode->child[0]->triList->reserve(pNode->triList->size());
        pNode->child[1]->triList->reserve(pNode->triList->size());
        partitionTriangles(pNode);

        // Remove children without triangles.
        int negChildTriNum = pNode->child[0]->triList->size();
        int posChildTriNum = pNode->child[1]->triList->size();

        if (negChildTriNum == 0)
        {
            delete pNode->child[0]->triList;
            delete pNode->child[0];
            pNode->child[0] = 0;
        }
        
        if (posChildTriNum == 0)
        {
            delete pNode->child[1]->triList;
            delete pNode->child[1];
            pNode->child[1] = 0;
        }

        constructBinTree(pNode->child[0], maxLeafTri);
        constructBinTree(pNode->child[1], maxLeafTri);
    }
    else
    {
        constructLeaf(pNode);
    }
}


/******************************************************************************
 Frees memory for bin tree.
******************************************************************************/
void BinTree::freeTree(Node* pNode)
{
    if (pNode)
    {
        freeTree(pNode->child[0]);
        freeTree(pNode->child[1]);

        delete pNode->triList;
        delete pNode;
    }
}


hduPlane<float> BinTree::plane(int level,
                               const hduBoundBox3Df& box) const
{
    hduVector3Df center((box.lo()[0]+box.hi()[0])/2.0,
                        (box.lo()[1]+box.hi()[1])/2.0,
                        (box.lo()[2]+box.hi()[2])/2.0);

    hduVector3Df p1, p2;

    switch (level % 3)
    {
        case 0: // Plane splits x axis.
            p1[0] = center[0];
            p1[1] = box.lo()[1];
            p1[2] = box.lo()[2];
            
            p2[0] = center[0];
            p2[1] = box.hi()[1];
            p2[2] = box.lo()[2];
            
            break;

        case 1: // Plane splits y axis.
            p1[0] = box.hi()[0];
            p1[1] = center[1];
            p1[2] = box.hi()[2];

            p2[0] = box.hi()[0];
            p2[1] = center[1];
            p2[2] = box.lo()[2];

            break;

        case 2: // Plane splits z axis.
            p1[0] = box.hi()[0];
            p1[1] = box.lo()[1];
            p1[2] = center[2];

            p2[0] = box.hi()[0];
            p2[1] = box.hi()[1];
            p2[2] = center[2];

            break;

        default:
            assert(false);
            
    }
    
    return hduPlane<float>(center, p1, p2);
}

/******************************************************************************
 Evaluates triangle relative to plane.  returns op code indicating which side 
 of the plane each point lies (0 = negative, 1 = positive).
******************************************************************************/
int BinTree::evaluateTriangle(const hduPlane<float>& plane,
                              const Triangle& tri)
{
    int res = 0;
    for (int i = 0; i < 3; ++i)
    {
        const hduVector3Df& pos = (*m_pVertices)[tri.vert[i]];
        double perpDist = plane.perpDistance(pos);
        if (perpDist > 0.0)
        {
            res |= (1 << i);
        }
    }
    
    return res;
}

/******************************************************************************
 Moves triangles from pNode into children.
******************************************************************************/
void BinTree::partitionTriangles(Node* pNode)
{
    for (TriContainer::iterator tri = pNode->triList->begin();
         tri != pNode->triList->end(); ++tri)
    {
        int opcode = evaluateTriangle(pNode->plane, (*tri));
        if (opcode == 0x0)
        {
            pNode->child[0]->triList->push_back(*tri);
        }
        else if (opcode == 0x7)
        {
            pNode->child[1]->triList->push_back(*tri);
        }
        else
        {
            // triangle crosses plane
            // just put a reference to the triangle in each bin
            // splitting the triangle is an exercise left to the reader
            pNode->child[0]->triList->push_back(*tri);
            pNode->child[1]->triList->push_back(*tri);
        }
    }
    
    pNode->triList->clear();
    delete pNode->triList;
    pNode->triList = 0;

}

/******************************************************************************
 Allocs children of node pNode.
******************************************************************************/
void BinTree::allocChildren(Node* pNode)
{
    Node* child0 = new Node;
    Node* child1 = new Node;
    
    // Generate child boxes based on parent and parent plane.
    splitBox(child0->box, child1->box, pNode->box, pNode->level);
    int level = pNode->level + 1;

    child0->level = level;
    child0->plane = plane(level, child0->box);
    child0->triList = new TriContainer;

    child0->child[0] = 0;
    child0->child[1] = 0;

    child1->level = level;
    child1->plane = plane(level, child1->box);
    child1->triList = new TriContainer;

    child1->child[0] = 0;
    child1->child[1] = 0;

    pNode->child[0] = child0;
    pNode->child[1] = child1;
}

/******************************************************************************
 Splits the bin tree.                                                                               
******************************************************************************/
void BinTree::splitBox(hduBoundBox3Df& negativeBox,
                       hduBoundBox3Df& positiveBox,
                       const hduBoundBox3Df& box,
                       int level)
{
    negativeBox = box;
    positiveBox = box;

    switch (level % 3)
    {
        case 0:
            negativeBox.rHi()[0] = (box.lo()[0]+box.hi()[0])/2.0;
            positiveBox.rLo()[0] = negativeBox.hi()[0];
            break;

        case 1:
            negativeBox.rHi()[1] = (box.lo()[1]+box.hi()[1])/2.0;
            positiveBox.rLo()[1] = negativeBox.hi()[1];
            break;

        case 2:
            negativeBox.rHi()[2] = (box.lo()[2]+box.hi()[2])/2.0;
            positiveBox.rLo()[2] = negativeBox.hi()[2];
            break;
    }
}

void drawBox(const Node* pNode)
{
    const hduBoundBox3Df& box = pNode->box;

    glBegin(GL_LINE_LOOP);
    glVertex3f(box.lo()[0], box.lo()[1], box.lo()[2]);
    glVertex3f(box.hi()[0], box.lo()[1], box.lo()[2]);
    glVertex3f(box.hi()[0], box.hi()[1], box.lo()[2]);
    glVertex3f(box.lo()[0], box.hi()[1], box.lo()[2]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(box.lo()[0], box.lo()[1], box.hi()[2]);
    glVertex3f(box.hi()[0], box.lo()[1], box.hi()[2]);
    glVertex3f(box.hi()[0], box.hi()[1], box.hi()[2]);
    glVertex3f(box.lo()[0], box.hi()[1], box.hi()[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(box.lo()[0], box.lo()[1], box.lo()[2]);
    glVertex3f(box.lo()[0], box.lo()[1], box.hi()[2]);
    glVertex3f(box.lo()[0], box.hi()[1], box.lo()[2]);
    glVertex3f(box.lo()[0], box.hi()[1], box.hi()[2]);

    glVertex3f(box.hi()[0], box.lo()[1], box.lo()[2]);
    glVertex3f(box.hi()[0], box.lo()[1], box.hi()[2]);
    glVertex3f(box.hi()[0], box.hi()[1], box.lo()[2]);
    glVertex3f(box.hi()[0], box.hi()[1], box.hi()[2]);

    glEnd();

}

int BinTree::traverseAndRender()
{
    int triCount;
    traverseAndRender(m_pRoot, triCount);
    return triCount;
}

int BinTree::traverseAndRender(const hduVector3Df* frustum)
{
    int triCount = 0;
    traverseAndRender(m_pRoot, frustum, triCount);
    return triCount;
}

void BinTree::traverseAndRenderBoxes()
{
    traverseAndRenderBoxes(m_pRoot);
}

void BinTree::traverseAndRenderBoxes(const hduVector3Df* frustum)
{
    traverseAndRenderBoxes(m_pRoot, frustum);
}


void BinTree::renderLeaf(const Node* pNode)
{
    glBegin(GL_TRIANGLES);
    for (TriContainer::const_iterator triIt = pNode->triList->begin();
         triIt != pNode->triList->end(); ++triIt)
    {
        const Triangle& tri = (*triIt);
        glNormal3f(tri.normal[0],tri.normal[1],tri.normal[2]);
            
        const hduVector3Df p0 = (*m_pVertices)[tri.vert[0]];
        const hduVector3Df p1 = (*m_pVertices)[tri.vert[1]];
        const hduVector3Df p2 = (*m_pVertices)[tri.vert[2]];

        glVertex3fv(p0);
        glVertex3fv(p1);
        glVertex3fv(p2);
    }
    glEnd();
}


void BinTree::traverseAndRender(const Node* pNode, int& triCount)
{
    if (!pNode) return;

    if (pNode->triList)
    {
        renderLeaf(pNode);
        triCount += pNode->triList->size();
    }
    else
    {
        traverseAndRender(pNode->child[0], triCount);
        traverseAndRender(pNode->child[1], triCount);
    }
}

void BinTree::traverseAndRender(const Node* pNode, 
                                const hduVector3Df* frustum, 
                                int& triCount)
{
    if (!pNode) return;

    if (pNode->triList)
    {
        renderLeaf(pNode);
        triCount += pNode->triList->size();
    }
    else
    {
        int opcode = classifyFrustum(pNode, frustum);
        if (opcode == 0x0)
            traverseAndRender(pNode->child[0], frustum, triCount);
        else if (opcode == 0xFF)
            traverseAndRender(pNode->child[1], frustum, triCount);
        else
        {
            traverseAndRender(pNode->child[0], frustum, triCount);
            traverseAndRender(pNode->child[1], frustum, triCount);
        }
    }
}

void BinTree::traverseAndRenderBoxes(const Node* pNode)
{
    if (!pNode) return;

    drawBox(pNode);

    traverseAndRenderBoxes(pNode->child[0]);
    traverseAndRenderBoxes(pNode->child[1]);

}

void BinTree::traverseAndRenderBoxes(const Node* pNode, 
                                     const hduVector3Df* frustum)
{
    if (!pNode) return;

    drawBox(pNode);

    int opcode = classifyFrustum(pNode, frustum);
    if (opcode == 0x0)
        traverseAndRenderBoxes(pNode->child[0], frustum);
    else if (opcode == 0xFF)
        traverseAndRenderBoxes(pNode->child[1], frustum);
    else
    {
        traverseAndRenderBoxes(pNode->child[0], frustum);
        traverseAndRenderBoxes(pNode->child[1], frustum);
    }
}

int BinTree::classifyFrustum(const Node* pNode, 
                             const hduVector3Df* frustum) const
{ 
    int opcode = 0;
    const hduPlane<float>& plane = pNode->plane;
    for (int i = 0; i < 8; ++i)
    {
        hduVector3Df pt(frustum[i][0], 
                        frustum[i][1], 
                        frustum[i][2]);

        double perpDist = plane.perpDistance(pt);
        if (perpDist > 0)
        {
            opcode |= (1 << i);
        }
    }

    return opcode;
}

int BinTree::numTri(const hduVector3Df* frustum) const
{
    int triCount = 0;
    numTri(m_pRoot, frustum, triCount);

    return triCount;
}

void BinTree::numTri(const Node* pNode, 
                     const hduVector3Df* frustum, 
                     int triCount) const
{
    if (!pNode) return;

    if (pNode->triList)
    {
        triCount += pNode->triList->size();
    }
    else
    {
        int opcode = classifyFrustum(pNode, frustum);
        if (opcode == 0x0)
            numTri(pNode->child[0], frustum, triCount);
        else if (opcode == 0xFF)
            numTri(pNode->child[1], frustum, triCount);
        else
        {
            numTri(pNode->child[0], frustum, triCount);
            numTri(pNode->child[1], frustum, triCount);
        }
    }
}

/******************************************************************************/
