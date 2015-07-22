/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  Mesh.h

Description: 

  Polygon mesh class.

******************************************************************************/

#ifndef CXX_MESH_H_
#define CXX_MESH_H_

#include <vector>
#include <list>

#include <GL/gl.h>
#include <HL/hl.h>
#include <HDU/hduVector.h>
#include <HDU/hduPlane.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>

#include "BinTree.h"

using namespace std;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool load(const char* filename);
    int drawGL(const hduVector3Dd* frustumd);

    int numTri() const { return m_pBinTree ? m_pBinTree->numTri() : 0; }
    int numTri(const hduVector3Dd* frustumd) const;

    hduBoundBox3Df box() { return m_pBinTree->box(); }
    hduVector3Df center() const;
    float radius() const;

    HLuint shapeId;
    GLuint displayList;
    hduMatrix transform;

    bool isDragging;
    
    hduMatrix startDragObjTransform;
    hduVector3Dd startDragProxyPos;
    hduQuaternion startDragProxyRot;

private:
    BinTree* m_pBinTree;
};

#endif // CXX_MESH_H_

/******************************************************************************/
