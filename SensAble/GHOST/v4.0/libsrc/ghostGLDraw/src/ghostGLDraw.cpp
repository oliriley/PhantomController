//=============================================================================
//   Filename : ghostGLDraw.cpp
// Written by : Chris Tarr (ctarr@sensable.com)
//              Philip Winston (pwinston@sensable.com)
//              Brandon Itkowitz (bitkowitz@sensable.com)	
//    Project : GhostGL 1.0
//     Module : GhostGL Draw
// ----------------------------------------------------------------------------
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include "ghostGLDraw.h"

GLfloat ghostGLDraw::kDefaultCoordAxisScale(30.0);
GLfloat ghostGLDraw::kAxisArrowRadius(.05);  // based on a unit length axis
GLfloat ghostGLDraw::kAxisArrowHeight(.075); // based on a unit length axis

GLUquadricObj *ghostGLDraw::m_pQuadObj = gluNewQuadric();

void ghostGLDraw::drawCone(const double radius, const double height)
{
    // Make glu cone aligned along y axis going
    // from -height/2 to height/2 on the y-axis.
    // gluCone is aligned along the z-axis from 0-height by default
    glTranslated(0.0,height/2.0,0.0);
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(m_pQuadObj,
		0.0,
		radius,
		height,
		25,
		25);
    glTranslated(0, 0, height);
    gluDisk(m_pQuadObj,
	    0.0,
	    radius,
	    25,
	    25);
}

void ghostGLDraw::drawCone(gstCone *coneH, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(coneH);

    drawCone(coneH->getRadius(),coneH->getHeight());
}

void ghostGLDraw::drawCoordAxes(double scale)
{
    float red_diffuse[] = {1.0, 0.0, 0.0};
    float green_diffuse[] = {0.0, 1.0, 0.0};
    float blue_diffuse[] = {0.0, 0.0, 1.0};
  
    // store the current color information
    glPushAttrib(GL_LIGHTING_BIT);
    
    glPushMatrix();
    glScalef(scale, scale, scale);
    
    // draw the x axis
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
    drawVector(gstVector(1.0,0.0,0.0));
    
    // draw the y axis
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
    drawVector(gstVector(0.0,1.0,0.0));
    
    // draw the z axis
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
    drawVector(gstVector(0.0,0.0,1.0));
    
    glPopMatrix();
    
    // restore the current color information
    glPopAttrib();
}

void ghostGLDraw::drawCube(const double length, const double height, const double width)
{
    double v1[3], v2[3], v3[3], v4[3], v5[3], v6[3], v7[3], v8[3];
    
    v1[0] = -width / 2.0; v1[1] = height / 2.0; v1[2] = length / 2.0;
    v2[0] = width / 2.0; v2[1] = height / 2.0; v2[2] = length / 2.0;
    v3[0] = width / 2.0; v3[1] = height / 2.0; v3[2] = -length / 2.0;
    v4[0] = -width / 2.0; v4[1] = height / 2.0; v4[2] = -length / 2.0;
    v5[0] = -width / 2.0; v5[1]= -height / 2.0; v5[2] = length / 2.0;
    v6[0] = width / 2.0; v6[1] = -height / 2.0; v6[2] = length / 2.0;
    v7[0] = width / 2.0; v7[1] = -height / 2.0; v7[2] = -length / 2.0;
    v8[0] = -width / 2.0; v8[1] = -height / 2.0; v8[2] = -length / 2.0;
    
    glBegin(GL_TRIANGLES);
    
    // front face
    glNormal3f(0.0,0.0,1.0);
    glVertex3dv(v1);glVertex3dv(v6);glVertex3dv(v2);
    glVertex3dv(v1);glVertex3dv(v5);glVertex3dv(v6);
    
    // back face
    glNormal3f(0.0,0.0,-1.0);
    glVertex3dv(v4);glVertex3dv(v3);glVertex3dv(v7);
    glVertex3dv(v4);glVertex3dv(v7);glVertex3dv(v8);
    
    // left face
    glNormal3f(-1.0,0.0,0.0);
    glVertex3dv(v1);glVertex3dv(v4);glVertex3dv(v8);
    glVertex3dv(v1);glVertex3dv(v8);glVertex3dv(v5);
    
    // right face
    glNormal3f(1.0,0.0,0.0);
    glVertex3dv(v2);glVertex3dv(v7);glVertex3dv(v3);
    glVertex3dv(v2);glVertex3dv(v6);glVertex3dv(v7);
    
    // top face
    glNormal3f(0.0,1.0,0.0);
    glVertex3dv(v4);glVertex3dv(v1);glVertex3dv(v2);
    glVertex3dv(v4);glVertex3dv(v2);glVertex3dv(v3);
    
    // bottom face
    glNormal3f(0.0,-1.0,0.0);
    glVertex3dv(v5);glVertex3dv(v8);glVertex3dv(v6);
    glVertex3dv(v8);glVertex3dv(v7);glVertex3dv(v6);
    
    glEnd();
}

void ghostGLDraw::drawBoundaryCube(
    const double length,
    const double height,
    const double width)
{
    double v1[3], v2[3], v3[3], v4[3], v5[3], v6[3], v7[3], v8[3];
    
    v1[0] = -width / 2.0; v1[1] = height / 2.0; v1[2] = length / 2.0;
    v2[0] = width / 2.0; v2[1] = height / 2.0; v2[2] = length / 2.0;
    v3[0] = width / 2.0; v3[1] = height / 2.0; v3[2] = -length / 2.0;
    v4[0] = -width / 2.0; v4[1] = height / 2.0; v4[2] = -length / 2.0;
    v5[0] = -width / 2.0; v5[1]= -height / 2.0; v5[2] = length / 2.0;
    v6[0] = width / 2.0; v6[1] = -height / 2.0; v6[2] = length / 2.0;
    v7[0] = width / 2.0; v7[1] = -height / 2.0; v7[2] = -length / 2.0;
    v8[0] = -width / 2.0; v8[1] = -height / 2.0; v8[2] = -length / 2.0;
    
    glBegin(GL_TRIANGLES);
    
    // draw the normal and front faces pointing inwards
    
    // front face
    glNormal3f(0.0,0.0,-1.0);
    glVertex3dv(v1);glVertex3dv(v2);glVertex3dv(v5);
    glVertex3dv(v5);glVertex3dv(v2);glVertex3dv(v6);
    
    // back face
    glNormal3f(0.0,0.0,1.0);
    glVertex3dv(v3);glVertex3dv(v4);glVertex3dv(v7);
    glVertex3dv(v7);glVertex3dv(v4);glVertex3dv(v8);
    
    // left face
    glNormal3f(1.0,0.0,0.0);
    glVertex3dv(v4);glVertex3dv(v1);glVertex3dv(v5);
    glVertex3dv(v4);glVertex3dv(v5);glVertex3dv(v8);
    
    // right face
    glNormal3f(-1.0,0.0,0.0);
    glVertex3dv(v2);glVertex3dv(v3);glVertex3dv(v7);
    glVertex3dv(v2);glVertex3dv(v7);glVertex3dv(v6);
    
    // top face
    glNormal3f(0.0,-1.0,0.0);
    glVertex3dv(v1);glVertex3dv(v4);glVertex3dv(v2);
    glVertex3dv(v2);glVertex3dv(v4);glVertex3dv(v3);
    
    // bottom face
    glNormal3f(0.0,1.0,0.0);
    glVertex3dv(v5);glVertex3dv(v6);glVertex3dv(v7);
    glVertex3dv(v5);glVertex3dv(v7);glVertex3dv(v8);
    
    glEnd();
}

void ghostGLDraw::drawCube(gstCube *cubeH, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(cubeH);

    drawCube(cubeH->getLength(), cubeH->getHeight(), cubeH->getWidth());
}

void ghostGLDraw::drawBoundaryCube(gstBoundaryCube *cubeH, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(cubeH);

    drawBoundaryCube(cubeH->getLength(), cubeH->getHeight(), cubeH->getWidth());
}

void ghostGLDraw::drawCylinder(const double radius, const double height)
{
    // Make glu cylinder aligned along y axis going
    // from -height/2 to height/2 on the y-axis.
    // gluCylinder is aligned along the z-axis from 0-height by default
    glTranslated(0.0, height/2.0, 0.0);
    glRotated(90.0, 1.0, 0.0, 0.0);
    gluCylinder(m_pQuadObj,
		radius,
		radius,
		height,
		24,
		24);
    glPushMatrix();
    // rotate to get normal correct, or could use another quadric
    glRotated(180.0, 1.0, 0.0, 0.0);
    gluDisk(m_pQuadObj,
	    0.0,
	    radius,
	    24,
	    24);
    glPopMatrix();
    glTranslated(0, 0, height);    
    gluDisk(m_pQuadObj,
	    0.0,
	    radius,
	    24,
	    24);
}

void ghostGLDraw::drawCylinder(gstCylinder *cylinderH, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(cylinderH);

    drawCylinder(cylinderH->getRadius(), cylinderH->getHeight());
}

void ghostGLDraw::drawNode(gstTransform *transformNode, gstBoolean bDrawTransform)
{
    // draw node based on its type
    if (transformNode->isOfType(gstPHANToM::getClassTypeId())) {
	drawPhantom((gstPHANToM *) transformNode, bDrawTransform);
    } else if (transformNode->isOfType(gstSeparator::getClassTypeId())) {
	drawSeparator((gstSeparator *) transformNode,  bDrawTransform);
    } else if (transformNode->isOfType(gstBoundaryCube::getClassTypeId())) {
	drawBoundaryCube((gstBoundaryCube *) transformNode,  bDrawTransform);
    } else if (transformNode->isOfType(gstSphere::getClassTypeId())) {
	drawSphere((gstSphere *) transformNode, bDrawTransform);
    } else if (transformNode->isOfType(gstCube::getClassTypeId())) {
	drawCube((gstCube *) transformNode, bDrawTransform);
    } else if (transformNode->isOfType(gstCylinder::getClassTypeId())) {
	drawCylinder((gstCylinder *) transformNode, bDrawTransform);
    } else if (transformNode->isOfType(gstTorus::getClassTypeId())) {
	drawTorus((gstTorus *) transformNode, bDrawTransform); 
    } else if (transformNode->isOfType(gstCone::getClassTypeId())) {
	drawCone((gstCone *) transformNode, bDrawTransform); 
    } else if (transformNode->isOfType(gstTriPolyMeshHaptic::getClassTypeId())) {
	drawTriPolyMeshHaptic((gstTriPolyMeshHaptic *) transformNode, bDrawTransform);
    } else {
	// Attempting to draw unsupported node!
    }
    
    //GLenum err = glGetError();
    //if (err != GL_NO_ERROR) {
    //fprintf(stderr, "GL_ERROR: %s\n", gluErrorString(err));
    //assert(0);
    //}
}

void ghostGLDraw::drawPhantom(gstPHANToM *phantom, gstBoolean bDrawTransform)
{
    static const double kCursorRadius = 3.0;
    static const double kCursorHeight = kCursorRadius * 3;
    static const int kCursorSectors = 15;
    static float cursor_diffuse[] = {0.0f, 0.5f, 1.0f, 1.0f};
    
    if (bDrawTransform)
	drawTransform(phantom);
    
    // store the current color information
    glPushAttrib(GL_LIGHTING_BIT);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cursor_diffuse);
    
    if (phantom->getInfo()->hasGimbal) {
	gluCylinder(m_pQuadObj, 0.0, kCursorRadius, kCursorHeight,
		    kCursorSectors, kCursorSectors);
	glTranslated(0.0, 0.0, kCursorHeight);
	gluCylinder(m_pQuadObj, kCursorRadius, 0.0, kCursorHeight / 5.0,
		    kCursorSectors, kCursorSectors);
    } else {
	gluSphere(m_pQuadObj, kCursorRadius, kCursorSectors, kCursorSectors);
    }
    
    // restore the color settings
    glPopAttrib();
}

void ghostGLDraw::drawPoint(const gstPoint &pt)
{
    glTranslated(pt.x(), pt.y(), pt.z());
    drawSphere(2.0);
}

void ghostGLDraw::drawSeparator(gstSeparator *sep, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(sep);
}

void ghostGLDraw::drawSphere(const double radius)
{
    gluSphere(m_pQuadObj, radius, 25, 25);
}

void ghostGLDraw::drawSphere(gstSphere *sphereH, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(sphereH);
    drawSphere(sphereH->getRadius());
}

void ghostGLDraw::drawTorus(double inner, double outter)
{
    static const int NSIDES = 20;
    static const int NRINGS = 20;
    double theta, phi, theta1, phi1;
    double p0[3], p1[3], p2[3], p3[3];
    double n0[3], n1[3], n2[3], n3[3];
    
    for (int i = 0; i < NRINGS; i++) {
	theta = (double) i *2.0f * M_PI / NRINGS;
	theta1 = (double) (i + 1) * 2.0f * M_PI / NRINGS;
	for (int j = 0; j < NSIDES; j++) {
	    phi = (double) j *2.0f * M_PI / NSIDES;
	    phi1 = (double) (j + 1) * 2.0f * M_PI / NSIDES;
	    
	    p0[0] = cos(theta) * (outter + inner * cos(phi));
	    p0[1] = -sin(theta) * (outter + inner * cos(phi));
	    p0[2] = inner * sin(phi);
	    
	    p1[0] = cos(theta1) * (outter + inner * cos(phi));
	    p1[1] = -sin(theta1) * (outter + inner * cos(phi));
	    p1[2] = inner * sin(phi);
	    
	    p2[0] = cos(theta1) * (outter + inner * cos(phi1));
	    p2[1] = -sin(theta1) * (outter + inner * cos(phi1));
	    p2[2] = inner * sin(phi1);
	    
	    p3[0] = cos(theta) * (outter + inner * cos(phi1));
	    p3[1] = -sin(theta) * (outter + inner * cos(phi1));
	    p3[2] = inner * sin(phi1);
	    
	    n0[0] = cos(theta) * (cos(phi));
	    n0[1] = -sin(theta) * (cos(phi));
	    n0[2] = sin(phi);
	    
	    n1[0] = cos(theta1) * (cos(phi));
	    n1[1] = -sin(theta1) * (cos(phi));
	    n1[2] = sin(phi);
	    
	    n2[0] = cos(theta1) * (cos(phi1));
	    n2[1] = -sin(theta1) * (cos(phi1));
	    n2[2] = sin(phi1);
	    
	    n3[0] = cos(theta) * (cos(phi1));
	    n3[1] = -sin(theta) * (cos(phi1));
	    n3[2] = sin(phi1);
	    
	    glBegin(GL_QUADS);
	    glNormal3dv(n3);
	    glVertex3dv(p3);
	    glNormal3dv(n2);
	    glVertex3dv(p2);
	    glNormal3dv(n1);
	    glVertex3dv(p1);
	    glNormal3dv(n0);
	    glVertex3dv(p0);
	    glEnd();
	}
    }
}

void ghostGLDraw::drawTorus(gstTorus *torus, gstBoolean bDrawTransform)
{
    if (bDrawTransform)
	drawTransform(torus);

    glRotated(-90.0, 1.0, 0.0, 0.0);
    drawTorus(torus->getMinorRadius(), torus->getMajorRadius());
}

void ghostGLDraw::drawTransform(gstTransform *transformObj)
{
    gstTransformMatrix transformMatrix = transformObj->getTransformMatrix();
    drawTransformMatrix(transformMatrix);
}

void ghostGLDraw::drawTransformMatrix(gstTransformMatrix &matrixH)
{	
    glMultMatrixd(matrixH.getOpenGLMatrix());
}

void ghostGLDraw::drawTriangle(gstTriPoly *tri)
{
    gstVertex *vert1=tri->getV1(), *vert2=tri->getV2(), *vert3=tri->getV3();
    
    glNormal3dv(&(tri->normal()[0]));
    glVertex3dv(vert1[0]);
    glVertex3dv(vert2[0]);
    glVertex3dv(vert3[0]);
}

void ghostGLDraw::drawTriPolyMesh(gstTriPolyMesh *triMesh)
{
    // If there is no triMesh then return
    if (!triMesh)
	return;
    
    gstTriPolyPtrHashMapConstIterator trisIter = triMesh->polygonsBegin();
    
    glBegin(GL_TRIANGLES);
    while (trisIter != triMesh->polygonsEnd())
	{
	    drawTriangle((*trisIter).second);
	    ++trisIter;
	}
    glEnd();
}

void ghostGLDraw::drawTriPolyMeshHaptic(
    gstTriPolyMeshHaptic *triMeshH,
    gstBoolean bDrawTransform)
{
    // If there is no triMeshH then return
    if (!triMeshH)
        return;
    
    if (bDrawTransform)
        drawTransform(triMeshH);
    drawTriPolyMesh( (gstTriPolyMesh *) triMeshH->getTriPolyMesh());
}

void ghostGLDraw::drawVector(const gstVector &vec)
{
    glBegin(GL_LINES);
    gstPoint origin(0.0,0.0,0.0);
    glVertex3dv((double *) &(origin[0]));
    glVertex3dv((double *) &(vec[0]));
    glEnd();
    
    glPushMatrix();
    glTranslated(vec.x(), vec.y(), vec.z());
    
    //  Rotating y-axis to coincide with direction of ray.
    //  Crossp of these vecs gives axis to rotate about.
    //  The arcsin of the crossp magnitude give the angle
    //  to rotate in radians.  This equation is found in
    //  Standard Mathematical Tables and Formulae 30th edition Pg 114

    gstVector rotationVec = crossP(vec, gstVector(0.0, 1.0, 0.0));
    double rotationAngle = asin(rotationVec.norm());
    rotationAngle = - (180.0 / M_PI) * rotationAngle;
    glRotated(rotationAngle, rotationVec[0], rotationVec[1], rotationVec[2]);
    drawCone(kAxisArrowRadius, kAxisArrowHeight);
    glPopMatrix();
}



