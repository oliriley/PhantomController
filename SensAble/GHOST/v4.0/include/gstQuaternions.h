//=============================================================================
//   Filename : gstQuaternions.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Implement quaternions (to represent rotations)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_QUATERNIONS
#define GST_QUATERNIONS

#include <gstDllExport.h>
#include <gstVector.h>

class gstQuaternion;

// convert a quat to a 3x3 matrix
GHOST_DLL_IMPORT_EXPORT
void quaternionToMatrix(const gstQuaternion &q, double (*r)[3]);

// normalize a quat
GHOST_DLL_IMPORT_EXPORT
void normalizeQuaternion(gstQuaternion &q1);

GHOST_DLL_IMPORT_EXPORT
void matrixToQuaternion(double (*r)[3], gstQuaternion &q);

// multiply 2 quats, put result in 3rd argument
GHOST_DLL_IMPORT_EXPORT
void multQuaternions(const gstQuaternion &q1, const gstQuaternion &q2, gstQuaternion &result);

// multiply 2 and return result
GHOST_DLL_IMPORT_EXPORT
gstQuaternion multQuaternions(const gstQuaternion &q1, const gstQuaternion &q2);

// scale quat by a scalar
GHOST_DLL_IMPORT_EXPORT
void scaleQuaternion(double s, gstQuaternion &q);

// multiply operator for quats
GHOST_DLL_IMPORT_EXPORT
gstQuaternion operator * (const gstQuaternion &q1, const gstQuaternion &q2);

// Implement quaternions (to represent rotations)
class GHOST_DLL_IMPORT_EXPORT gstQuaternion
{
    public:
        double s;
        gstVector v;
        gstQuaternion() : s(1), v(0, 0, 0) { };
        gstQuaternion(double _s, gstVector _v) : s(_s), v(_v) { }
        gstQuaternion(double (*r)[3]) {matrixToQuaternion(r,*this); }
        gstQuaternion(gstVector axis, double ang);
        void scale(double s);

        void normalize() { normalizeQuaternion(*this); }

        void toMatrix(double (*r)[3]);

        void toAxisAngle(gstVector &axis, double &radians);

        gstQuaternion operator *= (const gstQuaternion &q1); 
};

#endif
