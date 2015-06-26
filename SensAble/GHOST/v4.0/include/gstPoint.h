//=============================================================================
//   Filename : gstPoint.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Represents a point in 3D space (x, y, z)
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPOINT_INCLUDED)
#define GSTPOINT_INCLUDED

#include <gstDllExport.h>
#include <gstTypedefs.h>
#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <malloc.h>

// Represents a point in 3D space (x, y, z)
class GHOST_DLL_IMPORT_EXPORT gstPoint
{
    // insert operator for gstPoint
	friend GHOST_DLL_IMPORT_EXPORT ostream & operator <<(ostream& os, const gstPoint& pt);
	friend GHOST_DLL_IMPORT_EXPORT istream & operator >>(istream& is, gstPoint& pt);

  public:

	//
	// Constructor.
	//
	inline gstPoint();

	//
	// Constructor.
	//
	inline gstPoint(
		double x,
		double y,
		double z);

	//
	// Constructor.
	//
	inline gstPoint(
		const gstPoint &p);

	//
	// Constructor.
	//
	inline gstPoint(
		const gstPoint *p);

	//
	// Returns magnitude of distance of point to origin.
	//
	inline double distToOrigin();

	//
	// Returns pointer to dynamically allocated double array containing
	// x,y,z coordinates.
	//
	inline const double * getValue() const;

	//
	// Returns x,y,z coordinates.
	//
	inline void getValue(
		double &x,
		double &y,
		double &z);

	//
	// double coords[0], coords[1], coords[2];
	// Constructor.
	//
	inline void init(
		double x,
		double y,
		double z);


	inline void init(
		double x,
		double y,
		double z,
		double w);

	//
	// Check for zero operator.
	//
	inline gstBoolean isZero() const;

	//
	// Subtraction operato.r
	//
	inline gstPoint operator -() const;

	//
	// Substraction operator.
	//
	inline gstPoint operator -(
		const gstPoint &p) const;

	//
	// Inequality test operator.
	//
	inline gstBoolean operator !=(
		const gstPoint &p) const;

	//
	// Returns coords[i], allowing point to
	// be accessed and treated as an array.
	//
	inline double & operator [](
		int i);

	//
	// Returns coords[i], allowing point to
	// be accessed and treated as an array.
	//
	inline const double & operator [](
		int i) const;

	//
	// Addition operator.
	//
	inline gstPoint operator +(
		const gstPoint &p) const;

	//
	// Accumulation (add and assign) operator.
	//
	inline gstPoint & operator +=(
		const gstPoint &p);

	//
	// Less than operator.
	//
	inline gstBoolean operator <(
		const gstPoint &p) const;

	//
	// Assignment operator.
	//
	inline gstPoint & operator =(
		const gstPoint &p);

	//
	// Assignment operator.
	//
	inline gstPoint & operator =(
		const gstPoint *p);

	//
	// Subtract and assign operator.
	//
	inline gstPoint & operator -=(
		const gstPoint &p);

	//
	// Equality test operator.
	//

	inline gstBoolean operator ==(
		const gstPoint &p) const;

	//
	// Greater than operator.
	//
	inline gstBoolean operator >(
		const gstPoint &p) const;


	operator double *();

	//
	// Print to stdout.
	//
	inline void printSelf();

	//
	// Set coords[0] coordinate.
	//
	inline void setx(
		double x);

	//
	// Set coords[1] coordinate.
	//
	inline void sety(
		double y);

	//
	// Set coords[2] coordinate.
	//
	inline void setz(
		double z);

	//
	// Returns 4th homogeneous component of point.
	//
	inline double w() const;

    // Check if point is (nearly) the same
	gstBoolean withinEpsilon(
		const gstPoint &pt,
		double epsilon=0.00000001) const;

	//
	// Returns coords[0] coordinate.
	//
	inline double x() const;

	//
	// Returns coords[1] coordinate.
    //
	inline double y() const;

	//
	//
	// Returns coords[2] coordinate.
	//
	inline double z() const;

  protected:


	double coords[4];
};



GHOST_DLL_IMPORT_EXPORT ostream & operator <<(
	ostream& os,
	const gstPoint& pt);

GHOST_DLL_IMPORT_EXPORT istream & operator >>(
	istream& os,
	gstPoint& pt);


// Check if two points are nearly the same
GHOST_DLL_IMPORT_EXPORT gstBoolean withinEpsilon(
	const gstPoint &pt1,
	const gstPoint &pt2,
	double epsilon = 0.00000001);



inline gstPoint::gstPoint()
{
	coords[0] = 0.0; coords[1] = 0.0; coords[2] = 0.0; coords[3] = 1.0;
}


inline gstPoint::gstPoint(
	double x,
	double y,
	double z)
{
	coords[0] = x; coords[1] = y; coords[2] = z; coords[3] = 1.0;
}


inline gstPoint::gstPoint(
	const gstPoint &p)
{
	coords[0] = p.coords[0];
    coords[1] = p.coords[1];
    coords[2] = p.coords[2];
    coords[3] = p.coords[3];
}


inline gstPoint::gstPoint(
	const gstPoint *p)
{
	coords[0] = p->coords[0];
    coords[1] = p->coords[1];
    coords[2] = p->coords[2];
    coords[3] = p->coords[3];
}


inline double gstPoint::distToOrigin()
{
    return sqrt(coords[0]*coords[0] +
                coords[1]*coords[1] +
                coords[2]*coords[2]);
}


inline const double * gstPoint::getValue() const
{
	    double *ret = (double *)malloc(3*sizeof(double));
	    ret[0] = coords[0];
	    ret[1] = coords[1];
	    ret[2] = coords[2];
	    return ret;
}


inline void gstPoint::getValue(
	double &x,
	double &y,
	double &z)
{
	    x = coords[0];
	    y = coords[1];
	    z = coords[2];
}


inline void gstPoint::init(
	double x,
	double y,
	double z)
{
	coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    coords[3] = 1.0;
}


inline void gstPoint::init(
	double x,
	double y,
	double z,
	double w)
{
	coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    coords[3] = w;
}


inline gstBoolean gstPoint::isZero() const
{
	return (gstBoolean)(coords[0] == 0.0 &&
                        coords[1] == 0.0 &&
                        coords[2] == 0.0);
}


inline gstPoint gstPoint::operator -() const
{
	return gstPoint(-coords[0], -coords[1], -coords[2]);
}


inline gstPoint gstPoint::operator -(
	const gstPoint &p) const
{
	return gstPoint(coords[0] - p.coords[0],
                    coords[1] - p.coords[1],
                    coords[2] - p.coords[2]);
}


inline gstBoolean gstPoint::operator !=(
	const gstPoint &p) const
{
	return (gstBoolean)(coords[0] != p.coords[0] ||
                        coords[1] != p.coords[1] ||
                        coords[2] != p.coords[2]);
}


inline double & gstPoint::operator [](
	int i)
{
	    return coords[i];
}


inline const double & gstPoint::operator [](
	int i) const
{
	    return coords[i];
}


inline gstPoint gstPoint::operator +(
	const gstPoint &p) const
{
	return gstPoint(coords[0] + p.coords[0],
                    coords[1] + p.coords[1],
                    coords[2] + p.coords[2]);
}


inline gstPoint & gstPoint::operator +=(
	const gstPoint &p)
{
	coords[0] += p.coords[0],
    coords[1] += p.coords[1],
    coords[2] += p.coords[2];
    return *this;
}


inline gstBoolean gstPoint::operator <(
	const gstPoint &p) const
{
	return (gstBoolean)(coords[0] < p.coords[0] ||
                            (coords[0] == p.coords[0] &&
                            (coords[1] < p.coords[1] ||
                            (coords[1] == p.coords[1] &&
                            coords[2] < p.coords[2]))));
}


inline gstPoint & gstPoint::operator =(
	const gstPoint &p)
{
	coords[0] = p.coords[0];
    coords[1] = p.coords[1];
    coords[2] = p.coords[2];
    coords[3] = p.coords[3];
    return *this;
}


inline gstPoint & gstPoint::operator =(
	const gstPoint *p)
{
	coords[0] = p->coords[0];
    coords[1] = p->coords[1];
    coords[2] = p->coords[2];
    coords[3] = p->coords[3];
    return *this;
}


inline gstPoint & gstPoint::operator -=(
	const gstPoint &p)
{
	coords[0] -= p.coords[0];
    coords[1] -= p.coords[1];
    coords[2] -= p.coords[2];
    return *this;
}


inline gstBoolean gstPoint::operator ==(
	const gstPoint &p) const
{
	return (gstBoolean)(coords[0] == p.coords[0] &&
                        coords[1] == p.coords[1] &&
                        coords[2] == p.coords[2]);
}


inline gstBoolean gstPoint::operator >(
	const gstPoint &p) const
{
	return (gstBoolean)(coords[0] > p.coords[0] ||
                            (coords[0] == p.coords[0] &&
                            (coords[1] > p.coords[1] || 
                            (coords[1] == p.coords[1] &&
                            coords[2] > p.coords[2]))));
}


inline void gstPoint::printSelf()
{
	    printf("\t\t\t%6.6g,%6.6g,%6.6g\n",coords[0],coords[1],coords[2]);
}


inline void gstPoint::setx(double x)
{
	coords[0] = x;
}


inline void gstPoint::sety(double y)
{
	coords[1] = y;
}


inline void gstPoint::setz(double z)
{
	coords[2] = z;
}


inline double gstPoint::w() const
{
	return coords[3];
}


inline double gstPoint::x() const
{
	return coords[0];
}


inline double gstPoint::y() const
{
	return coords[1];
}


inline double gstPoint::z() const
{
	return coords[2];
}


#endif  // GSTPOINT_INCLUDED
