/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name: 

  DynamicsMath.cpp

Description:

  Math routines and types for dynamics applications.

*******************************************************************************/

#include "DynamicsMath.h"

DynReal length(DynQuaternion quat)
{
    return sqrt(quat.r * quat.r + quat.i * quat.i +
                quat.j * quat.j + quat.k * quat.k);
}

DynQuaternion normalize(DynQuaternion quat)
{
    DynReal L = length(quat);
    quat.r /= L;
    quat.i /= L;
    quat.j /= L;
    quat.k /= L;

    return quat;
}

DynMatrix3x3 quaternionToMatrix(DynQuaternion q)
{
    DynMatrix3x3 m(3, 3);

    m(0,0) = 1                      - 2*q.j*q.j     - 2*q.k*q.k;
    m(0,1) = 2*q.i*q.j      - 2*q.r*q.k;
    m(0,2) = 2*q.i*q.k      + 2*q.r*q.j;
    m(1,0) = 2*q.i*q.j      + 2*q.r*q.k;
    m(1,1) = 1                      - 2*q.i*q.i     - 2*q.k*q.k;
    m(1,2) = 2*q.j*q.k      - 2*q.r*q.i;
    m(2,0) = 2*q.i*q.k      - 2*q.r*q.j;
    m(2,1) = 2*q.j*q.k      + 2*q.r*q.i;
    m(2,2) = 1                      - 2*q.i*q.i     - 2*q.j*q.j;

    return m;
}

DynQuaternion matrixToQuaternion(DynMatrix3x3 &m)
{
    DynQuaternion q;
    DynReal tr, s;

    tr = m(0,0) + m(1,1) + m(2,2);

    if (tr >= 0)
    {
        s = sqrt(tr + 1);
        q.r = 0.5 * s;
        s = 0.5 / s;
        q.i = (m(2,1) - m(1,2)) * s;
        q.j = (m(0,2) - m(2,0)) * s;
        q.k = (m(1,0) - m(0,1)) * s;
    }
    else
    {
        int i = 0;

        if(m(1,1) > m(0,0))
            i = 1;
        if(m(2,2) > m(i,i))
            i = 2;

        switch(i)
        {
            case 0:
                s = sqrt((m(0,0) - (m(1,1) + m(2,2))) + 1);
                q.i = 0.5 * s;
                s = 0.5 / s;
                q.j = (m(0,1) + m(1,0)) * s;
                q.k = (m(2,0) + m(0,2)) * s;
                q.r = (m(2,1) - m(1,2)) * s;
                break;
            case 1:
                s = sqrt((m(1,1) - (m(2,2) + m(0,0))) + 1);
                q.j = 0.5 * s;
                s = 0.5 / s;
                q.k = (m(1,2) + m(2,1)) * s;
                q.i = (m(0,1) + m(1,0)) * s;
                q.r = (m(0,2) - m(2,0)) * s;
                break;
            case 2:
                s = sqrt((m(2,2) - (m(0,0) + m(1,1))) + 1);
                q.k = 0.5 * s;
                s = 0.5 / s;
                q.i = (m(2,0) + m(0,2)) * s;
                q.j = (m(1,2) + m(2,1)) * s;
                q.r = (m(1,0) - m(0,1)) * s;
                break;
        }
    }
    return q;
}

DynQuaternion operator*(const DynQuaternion &A, const DynQuaternion &B)
{
    DynQuaternion C;

    C.r = A.r*B.r - A.i*B.i - A.j*B.j - A.k*B.k;
    C.i = A.r*B.i + A.i*B.r + A.j*B.k - A.k*B.j;
    C.j = A.r*B.j - A.i*B.k + A.j*B.r + A.k*B.i;
    C.k = A.r*B.k + A.i*B.j - A.j*B.i + A.k*B.r;

    return C;
}

DynQuaternion operator*(const DynVector3 &A, const DynQuaternion &B)
{
    DynQuaternion C;

    C.r = - A[0]*B.i - A[1]*B.j - A[2]*B.k;
    C.i = + A[0]*B.r + A[1]*B.k - A[2]*B.j;
    C.j = - A[0]*B.k + A[1]*B.r + A[2]*B.i;
    C.k = + A[0]*B.j - A[1]*B.i + A[2]*B.r;

    return C;
}

DynQuaternion operator*(const DynReal &r, const DynQuaternion &q)
{
    DynQuaternion C;

    C.r = r * q.r;
    C.i = q.i;
    C.j = q.j;
    C.k = q.k;
        
    return C;
}

/******************************************************************************/

