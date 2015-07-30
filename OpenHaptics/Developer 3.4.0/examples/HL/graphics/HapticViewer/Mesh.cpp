/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  Mesh.cpp

Description: 
 
  Polygon mesh class.

******************************************************************************/

#include "stdAfx.h"
#include <assert.h>
#include <iostream>
#include <stack>

#include <GL/gl.h>

#include "Mesh.h"
#include "objReader.h"

#include <HDU/hduMath.h>
#include <HDU/hduBoundBox.h>

Mesh::Mesh() :
    m_pBinTree(0)
{
}

Mesh::~Mesh()
{
    if (m_pBinTree) delete m_pBinTree;
}

bool Mesh::load(const char* filename)
{
    VertContainer* pVertices = new VertContainer;
    TriContainer* pTriangles = new TriContainer;

    hduBoundBox3Df vertBox;
    try
    {
        readOBJAscii(filename, pVertices, pTriangles, &vertBox);
    }
    
    catch (const char* s)
    {
        delete pVertices;
        delete pTriangles;
        throw s;
    }

    // Transform vertices such that object fits into a canonical [-1..1] box 
    // centered at the origin.
    hduVector3Df center((vertBox.lo()[0]+vertBox.hi()[0])/2.0,
                        (vertBox.lo()[1]+vertBox.hi()[1])/2.0,
                        (vertBox.lo()[2]+vertBox.hi()[2])/2.0);

    hduVector3Df size(vertBox.hi()[0] - vertBox.lo()[0],
                      vertBox.hi()[1] - vertBox.lo()[1],
                      vertBox.hi()[2] - vertBox.lo()[2]);

    float scale = -1;
    for (int i = 0; i < 3; ++i)
    {
        scale = (size[i] > scale) ? size[i] : scale;
    }
    scale /= 2.0;

    for (VertContainer::iterator vertIt = pVertices->begin();
         vertIt != pVertices->end(); ++vertIt)
    {
        Vertex& vert = (*vertIt);
        vert[0] = (vert[0]-center[0])/scale;
        vert[1] = (vert[1]-center[1])/scale;
        vert[2] = (vert[2]-center[2])/scale;
    }

    // Generate triangles normals.
    for (TriContainer::iterator triIt = pTriangles->begin();
         triIt != pTriangles->end(); ++triIt)
    {
        Triangle& tri = (*triIt);
        
        const hduVector3Df p0 = (*pVertices)[tri.vert[0]];
        const hduVector3Df p1 = (*pVertices)[tri.vert[1]];
        const hduVector3Df p2 = (*pVertices)[tri.vert[2]];

        tri.normal = ((p1-p0).crossProduct(p2-p0));
        tri.normal.normalize();
    }

    // Construct bin tree.  Pass ownership of vertices and triangles 
    // to bintree.
    const bool isCanonicalScale = true;
    m_pBinTree = new BinTree(pVertices, pTriangles,isCanonicalScale);

    return true;
}


hduVector3Df Mesh::center() const
{
    const hduBoundBox3Df box = m_pBinTree->box();
    return (box.lo() + box.hi())/2.0;
}

float Mesh::radius() const
{
    return (center() - m_pBinTree->box().lo()).magnitude();
}


int Mesh::drawGL(const hduVector3Dd* frustumd)
{
    hduVector3Df frustumf[8];
    for (int i = 0; i < 8; ++i)
    {
        frustumf[i] = frustumd[i];
    }

    int triCount = m_pBinTree->traverseAndRender(frustumf);

    return triCount;
}

int Mesh::numTri(const hduVector3Dd* frustumd) const
{
    hduVector3Df frustumf[8];
    for (int i = 0; i < 8; ++i)
    {
        frustumf[i] = frustumd[i];
    }

    return m_pBinTree ? m_pBinTree->numTri(frustumf) : 0;
}

/******************************************************************************/
