//=============================================================================
//   Filename : gstPoint2D.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Represents a point in 2D space (x, y)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPOINT2D_INCLUDED)
#define GSTPOINT2D_INCLUDED

#include <gstDllExport.h> 
#include <gstTypedefs.h>
#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <malloc.h>
#include <list.h>
#include <vector.h>
#include <alloc.h>

// Represents a point in 2D space (x, y)
class GHOST_DLL_IMPORT_EXPORT gstPoint2D
{
    friend GHOST_DLL_IMPORT_EXPORT ostream & operator <<(ostream& os, gstPoint2D& pt);

  public:

    //
    // Constructor.
    //
    inline gstPoint2D(
        const gstPoint2D *p);

    //
    // Constructor.
    //
    inline gstPoint2D(
        const gstPoint2D &p);

    //
    // Constructor.
    //
    inline gstPoint2D(
        double u,
        double v);

    //
    // Constructor.
    //
    inline gstPoint2D();
    
    //
    // Returns magnitude of distance of point to origin.
    //
    inline double distToOrigin();

    //
    // Returns x,y,z coordinates.
    //
    inline void getValue(
        double &u,
        double &v);

    //
    // Returns pointer to dynamically allocated double array containing
    // u, v coordinates.
    //
    inline const double * getValue() const;

    //
    // double coords[0], coords[1], Constructor.
    //
    inline void init(
        double u,
        double v);

    //
    // Check for zero operator.
    //
    inline gstBoolean isZero() const;

    //
    // Substraction operator.
    //
    inline gstPoint2D operator -(
        const gstPoint2D &p) const;

    //
    // Subtraction operato.r
    //
    inline gstPoint2D operator -() const;

    // Scaler muliplication operator.
    friend gstPoint2D operator * (const double s, const gstPoint2D &p)
    { return gstPoint2D(s*p.coords[0], s*p.coords[1]); }

    // Scaler division operator.
    friend gstPoint2D operator / (const gstPoint2D &p, const double d)
    { return gstPoint2D(p.coords[0]/d, p.coords[1]/d); }

    // Scaler muliplication operator.
    friend gstPoint2D operator * (const gstPoint2D &p, const double s)
    { return s * p; }

    // Muliplication operator. 
    friend gstPoint2D operator * (const gstPoint2D &p1, const gstPoint2D &p2)
    { return gstPoint2D(p1[0]*p2[0], p1[1]*p2[1]); }

    // Division operator. 
    friend gstPoint2D operator / (const gstPoint2D &p1, const gstPoint2D &p2)
    { return gstPoint2D(p1[0]/p2[0], p1[1]/p2[1]); }


    // Multiply and assign operator.
    gstPoint2D & operator *= (double d) {
        coords[0] *= d;
        coords[1] *= d;
        return *this;
    }

    // Divide and assign operator.
    gstPoint2D & operator /= (double d) {
        return (*this *= (1.0/d));
    }    
    
    //
    // Inequality test operator.
    //
    inline gstBoolean operator !=(
        const gstPoint2D &p) const;

    //
    // Returns coords[i], allowing point to
    // be accessed and treated as an array.
    //
    inline const double & operator [](
        int i) const;

    //
    // Returns coords[i], allowing point to
    // be accessed and treated as an array.
    //
    inline double & operator [](
        int i);

    //
    // Addition operator.
    //
    inline gstPoint2D operator +(
        const gstPoint2D &p) const;

    //
    // Accumulation (add and assign) operator.
    //
    inline gstPoint2D & operator +=(
        const gstPoint2D &p);

    //
    // Less than operator.
    //
    inline gstBoolean operator <(
        const gstPoint2D &p) const;

    //
    // Assignment operator.
    //
    inline gstPoint2D & operator =(
        const gstPoint2D *p);

    //
    // Assignment operator.
    //
    inline gstPoint2D & operator =(
        const gstPoint2D &p);

    //
    // Subtract and assign operator.
    //
    inline gstPoint2D & operator -=(
        const gstPoint2D &p);

    //
    // Equality test operator.
    //
    inline gstBoolean operator ==(
        const gstPoint2D &p) const;

    //
    // Greater than operator.
    //
    inline gstBoolean operator >(
        const gstPoint2D &p) const;

    //
    // Print to stdout.
    //
    inline void printSelf();

    //
    // Set coords[0] coordinate.
    //
    inline void setu(
        double u);

    //
    // Set coords[1] coordinate.
    //
    inline void setv(
        double v);

    //
    // Returns coords[0] coordinate.
    //
    inline double u() const;

    //
    // Returns coords[1] coordinate.
    //
    inline double v() const;

  protected:


    double coords[2];
};


#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstPoint2D, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _Vector_base<gstPoint2D, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstPoint2D, __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT vector<gstPoint2D, __default_alloc_template<1,0> >;
#endif


GHOST_DLL_IMPORT_EXPORT ostream & operator <<(
    ostream& os,
    gstPoint2D& pt);

// Check if 2D points are nearly the same
GHOST_DLL_IMPORT_EXPORT gstBoolean withinEpsilon(
    const gstPoint2D &pt1,
    const gstPoint2D &pt2,
    double epsilon = 0.00000001);

inline gstPoint2D::gstPoint2D(
    const gstPoint2D *p)
{
    coords[0] = p->coords[0]; coords[1] = p->coords[1];
}


inline gstPoint2D::gstPoint2D(
    const gstPoint2D &p)
{
    coords[0] = p.coords[0]; coords[1] = p.coords[1];
}


inline gstPoint2D::gstPoint2D(
    double u,
    double v)
{
    coords[0] = u; coords[1] = v;
}


inline gstPoint2D::gstPoint2D()
{
    coords[0] = 0.0; coords[1] = 0.0;
}


inline double gstPoint2D::distToOrigin()
{
        return sqrt(coords[0]*coords[0] +coords[1]*coords[1]);
}


inline void gstPoint2D::getValue(
    double &u,
    double &v)
{
        u = coords[0];
        v = coords[1];
}


inline const double * gstPoint2D::getValue() const
{
        double *ret = (double *)malloc(2*sizeof(double));
        ret[0] = coords[0];
        ret[1] = coords[1];
        return ret;
}


inline void gstPoint2D::init(
    double u,
    double v)
{
    coords[0] = u; coords[1] = v;
}


inline gstBoolean gstPoint2D::isZero() const
{
    return (gstBoolean)(coords[0] == 0.0 && coords[1] == 0.0);
}


inline gstPoint2D gstPoint2D::operator -(
    const gstPoint2D &p) const
{
    return gstPoint2D(coords[0] - p.coords[0],
                      coords[1] - p.coords[1]);
}


inline gstPoint2D gstPoint2D::operator -() const
{
    return gstPoint2D(-coords[0], -coords[1]);
}


inline gstBoolean gstPoint2D::operator !=(
    const gstPoint2D &p) const
{
    return (gstBoolean)(coords[0] != p.coords[0] ||
                        coords[1] != p.coords[1]);
}


inline const double & gstPoint2D::operator [](
    int i) const
{
        return coords[i];
}


inline double & gstPoint2D::operator [](
    int i)
{
        return coords[i];
}


inline gstPoint2D gstPoint2D::operator +(
    const gstPoint2D &p) const
{
    return gstPoint2D(coords[0] + p.coords[0],
                      coords[1] + p.coords[1]);
}


inline gstPoint2D & gstPoint2D::operator +=(
    const gstPoint2D &p)
{
    coords[0] += p.coords[0], coords[1] += p.coords[1]; return *this;
}


inline gstBoolean gstPoint2D::operator <(
    const gstPoint2D &p) const
{
    return (gstBoolean)(coords[0] < p.coords[0] ||
                       (coords[0] == p.coords[0] &&
                       (coords[1] < p.coords[1] )));
}


inline gstPoint2D & gstPoint2D::operator =(
    const gstPoint2D *p)
{
    coords[0] = p->coords[0], coords[1] = p->coords[1]; return *this;
}


inline gstPoint2D & gstPoint2D::operator =(
    const gstPoint2D &p)
{
    coords[0] = p.coords[0], coords[1] = p.coords[1]; return *this;
}


inline gstPoint2D & gstPoint2D::operator -=(
    const gstPoint2D &p)
{
    coords[0] -= p.coords[0], coords[1] -= p.coords[1]; return *this;
}


inline gstBoolean gstPoint2D::operator ==(
    const gstPoint2D &p) const
{
    return (gstBoolean)(coords[0] == p.coords[0] && coords[1] == p.coords[1]);
}


inline gstBoolean gstPoint2D::operator >(
    const gstPoint2D &p) const
{
    return (gstBoolean)(coords[0] > p.coords[0] ||
                        (coords[0] == p.coords[0] && (coords[1] > p.coords[1] )));
}


inline void gstPoint2D::printSelf()
{
        printf("\t\t\t%6.6g,%6.6g\n",coords[0],coords[1]);
}


inline void gstPoint2D::setu(
    double u)
{
    coords[0] = u;
}


inline void gstPoint2D::setv(
    double v)
{
    coords[1] = v;
}


inline double gstPoint2D::u() const
{
    return coords[0];
}


inline double gstPoint2D::v() const
{
    return coords[1];
}


#endif  // GSTPOINT2D_INCLUDED
