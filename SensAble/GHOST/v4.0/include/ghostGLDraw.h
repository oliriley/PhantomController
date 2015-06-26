//=============================================================================
//   Filename : ghostGLDraw.h
// Written by : Chris Tarr (ctarr@sensable.com)
//              Philip Winston (pwinston@sensable.com)
//              Brandon Itkowitz (bitkowitz@sensable.com)	
//    Project : GhostGL 1.0
//     Module : GhostGL Draw
// ----------------------------------------------------------------------------
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GHOST_GL_DRAW_H
#define GHOST_GL_DRAW_H

#ifdef _WIN32
#include <windows.h>
#pragma warning(disable : 4305)     // suppresses warning C4305: 'initializing' : truncation from 'const double' to 'float'
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <gstBasic.h>
#include <gstCone.h>
#include <gstCube.h>
#include <gstCylinder.h>
#include <gstSphere.h>
#include <gstTorus.h>
#include <gstTriPolyMeshHaptic.h>
#include <gstBoundaryCube.h>
#include <gstPHANToM.h>

// Provides OpenGL based routines for drawing GHOST nodes. Each primitive
// has a generic drawing routine based on its geometric description as well as
// a GHOST wrapper. Generally, you'll want to pass a GHOST object to the
// corresponding wrapper function, which will handle calling the appropriate
// OpenGL drawing routine with the necessary parameters.
//
// The GHOST wrapper routines all take two parameters. The first parameter
// is a pointer to the gstShape object. The second parameter specifies
// whether you want ghostGLDraw to apply the object's local transform to the
// OpenGL ModelView matrix stack before drawing the geometry.
class ghostGLDraw
{
public:

    ghostGLDraw(){};
    virtual ~ghostGLDraw(){};

    static void drawCone(const double radius, const double height);

    static void drawCone(gstCone *coneH, gstBoolean bDrawTransform = TRUE);

    static void drawCoordAxes(double scale = kDefaultCoordAxisScale);

    static void drawCube(const double length,
			 const double height,
			 const double width);

    static void drawCube(gstCube *cubeH, gstBoolean bDrawTransform = TRUE);

    static void drawBoundaryCube(const double length,
				 const double height,
				 const double width);

    static void drawBoundaryCube(gstBoundaryCube *cubeH, gstBoolean bDrawTransform = TRUE);

    static void drawCylinder(const double radius, const double height);

    static void drawCylinder(gstCylinder *cylinderH, gstBoolean bDrawTransform = TRUE);

    static void drawNode(gstTransform *transformNode, gstBoolean bDrawTransform = TRUE);

    static void drawPhantom(gstPHANToM *phantom, gstBoolean bDrawTransform = TRUE);

    static void drawPoint(const gstPoint &pt);

    // Draws separator transform.  Does NOT draw children.
    static void drawSeparator(gstSeparator *sep, gstBoolean bDrawTransform = TRUE);

    static void drawSphere(const double radius);

    static void drawSphere(gstSphere *sphereH, gstBoolean bDrawTransform = TRUE);

    static void drawTorus(double inner, double outter);

    static void drawTorus(gstTorus *torus, gstBoolean bDrawTransform = TRUE);

    static void drawTransform(gstTransform *transformObj);

    static void drawTransformMatrix(gstTransformMatrix &matrixH);

    static void drawTriangle(gstTriPoly *tri);

    static void drawTriPolyMesh(gstTriPolyMesh *triMesh);

    static void drawTriPolyMeshHaptic(gstTriPolyMeshHaptic *triMeshH, gstBoolean bDrawTransform = TRUE);

    static void drawVector(const gstVector &vec);

protected:

    static GLUquadricObj * m_pQuadObj;

private:

    static GLfloat kDefaultCoordAxisScale;
    static GLfloat kAxisArrowRadius;
    static GLfloat kAxisArrowHeight;

};

#endif // GHOST_GL_DRAW_H

