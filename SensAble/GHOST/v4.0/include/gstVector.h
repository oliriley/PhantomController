//=============================================================================
//   Filename : gstVector.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Represents a 3d vector
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include <vector.h>
#include <list.h>
#include <gstDllExport.h>
#include <gstTypedefs.h>
#include <gstPoint.h>

#ifndef GST_VECTOR
#define GST_VECTOR


// 3D vector class (x,y,z).
class GHOST_DLL_IMPORT_EXPORT gstVector : public gstPoint
{

public:

    // Constructor.
    gstVector()                            : gstPoint() {}

    // Constructor.
    gstVector(const gstPoint &point)    : gstPoint(point) {}

    // Constructor.
    gstVector(const gstPoint *point)    : gstPoint(*point){}

    // Constructor.
    gstVector(const gstVector &v)    : gstPoint(v.x(), v.y(), v.z()) {}
    
    // Constructor.
    gstVector(double x, double y, double z): gstPoint(x, y, z) {}
    
    // Constructor.
    gstVector(double array[3])            : gstPoint(array[0],array[1],array[2]) {}

    // Normalize vector.
    gstVector &normalize (); //lint -esym(534,gstVector::normalize)

    // Return vector magnitude.
    double norm() const { return (sqrt(coords[0]*coords[0]+coords[1]*coords[1]+coords[2]*coords[2])); }

    // Normalize and return norm
    double normalizeReturnNorm();

    //
    // Return number of longest axis component of vector (0=x, 1=y, 2=z).
    //
    int getLongestAxisComponent() const;

    //
    // Return number of longest axis component of vector (0=x, 1=y, 2=z).
    //
    int getSecondLongestAxisComponent() const;

    //
    // Return number of shortest axis component of vector (0=x, 1=y, 2=z).
    //
    int getShortestAxisComponent() const;

    // Assignment operator.
    gstVector&  operator =    (const gstVector &p) {
        coords[0] = p.coords[0];
        coords[1] = p.coords[1];
        coords[2] = p.coords[2];
        coords[3] = p.coords[3];
        return *this;
    }

    // Assignment operator.
    gstVector&  operator =    (const gstVector *p) {
        coords[0] = p->coords[0];
        coords[1] = p->coords[1];
        coords[2] = p->coords[2];
        coords[3] = p->coords[3];
        return *this;
    }

    // Subtraction operato.r
    gstVector   operator -    () const {
        return gstVector(-coords[0], -coords[1], -coords[2]);
    }
 
    // Accumulation (add and assign) operator.
    gstVector&  operator +=    (const gstVector &p) {
        coords[0] += p.coords[0];
        coords[1] += p.coords[1];
        coords[2] += p.coords[2];
        return *this;
    }

    // Subtract and assign operator.
    gstVector&  operator -= (const gstVector &p) {
        coords[0] -= p.coords[0];
        coords[1] -= p.coords[1];
        coords[2] -= p.coords[2];
        return *this;
    }
    
    // Addition operator.
    gstVector   operator +  (const gstVector &p) const {
        return gstVector(coords[0] + p.coords[0],
                         coords[1] + p.coords[1],
                         coords[2] + p.coords[2]);
    }

    // Substraction operator.
    gstVector   operator -  (const gstVector &p) const {
        return gstVector(coords[0] - p.coords[0],
                         coords[1] - p.coords[1],
                         coords[2] - p.coords[2]);
    }
 
    // Returns cross product of two vectors.
    friend GHOST_DLL_IMPORT_EXPORT gstVector crossP(const gstVector &, const gstVector &);

    // Returns dot product of two vectors.
    friend GHOST_DLL_IMPORT_EXPORT double dotP(const gstVector &, const gstVector &);
  
    // Returns magnitude of the distance between two points.
    friend GHOST_DLL_IMPORT_EXPORT double dist(const gstPoint &, const gstPoint &);
  
    // Returns dot product of current vector and vector a.
    double dot(const gstVector &a) {
         return (dotP(this,a));
    }
  
    // Returns cross product of current vector and vector a.
    gstVector cross(const gstVector &a) {
        return (crossP(this,a));
    }

    // Scaler muliplication operator.
    friend gstVector operator * (const double s, const gstVector &v)
    { return gstVector(s*v.coords[0], s*v.coords[1], s*v.coords[2]); }

    // Scaler division operator.
    friend gstVector operator / (const gstVector &v, const double d)
    { return gstVector(v.coords[0]/d, v.coords[1]/d, v.coords[2]/d); }

    // Equality operator.
    friend int operator == (const gstVector &a, const gstVector &b)
    { return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]); }

    // Inequality operator.
    friend int operator != (const gstVector &a, const gstVector &b)
    { return (!(a == b));  }
  
    // Scaler muliplication operator.
    friend gstVector operator * (const gstVector &v, const double s)
    { return s * v; }

    // Muliplication operator. 
    friend gstVector operator * (const gstVector &v1, const gstVector &v2)
    { return gstVector(v1[0]*v2[0], v1[1]*v2[1], v1[2]*v2[2]); }

    // Division operator. 
    friend gstVector operator / (const gstVector &v1, const gstVector &v2)
    { return gstVector(v1[0]/v2[0], v1[1]/v2[1], v1[2]/v2[2]); }


    // Multiply and assign operator.
    gstVector &operator *= (double d) {
        coords[0] *= d;
        coords[1] *= d;
        coords[2] *= d;
        return *this;
    }

    // Divide and assign operator.
    gstVector & operator /= (double d) {
        return (*this *= (1.0/d));
    }

  
    // Returns the magnitude of projection from point a
    // onto vector.
    double distance(const gstPoint &a) {
         double ratio = (dotP(this,a)/this->norm()/this->norm());
         gstVector proj2;
         gstVector proj = *this*ratio;
         return (gstVector(a-proj)).norm();
     }

};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstVector, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstVector, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstVector, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstVector, __default_alloc_template<1,0> >;
#endif


// Zero length vector.
#define zeroVect gstVector(0.0,0.0,0.0)

// X Hat vector.
#define xhatVect gstVector(1.0,0.0,0.0)

// Y Hat vector.
#define yhatVect gstVector(0.0,1.0,0.0)

// Z Hat vector.
#define zhatVect gstVector(0.0,0.0,1.0)


#endif
