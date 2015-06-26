//
// File:gstBasicTransformMatrix.h
//
// Copyright (C) 2000 SensAble Technologies, Inc.
// All rights reserved.
//
//
//
// Provides basic functionality of a 4x4 transform matrix.
//

#ifndef GST_BASIC_TRANFORM_MATRIX
#define GST_BASIC_TRANFORM_MATRIX

#include <gstGenericMatrix.h>
#include <gstBasic.h>
#include <gstPoint.h>
#include <gstVector.h>


class GHOST_DLL_IMPORT_EXPORT gstBasicTransformMatrix
{

  public:

	//
	// Default constructor.
	//
	gstBasicTransformMatrix()
    {
        makeIdentity();
    }

	//
	// Constructor from array of 16 values
    // stored by row.
	//
	gstBasicTransformMatrix(const double * const mat);

    //
	// Construct from 16 values.  Values construct the following matrix:
	//  a1  a2  a3  a4
    //  a5  a6  a7  a8
    //  a9 a10 a11 a12
    // a13 a14 a15 a16
 	//
    gstBasicTransformMatrix(double a1, double a2, double a3, double a4,
        double a5, double a6, double a7, double a8,
        double a9, double a10, double a11, double a12,
        double a13, double a14, double a15, double a16)
    {
      double a[4][4] = {
        {a1,a2,a3,a4},{a5,a6,a7,a8},{a9,a10,a11,a12},{a13,a14,a15,a16}};
        set (a);
    }

    //
    // Constructor from 4x4 array of values
    //

    gstBasicTransformMatrix(
	const double a[4][4])
    {
        set(a);
    }

    //
    // Compare matrices (returns true if all elements
    // of one matrix are within epsilon of the other).
    //
	bool compare(const gstBasicTransformMatrix& rhs,
                double epsilon=GST_EPSILON) const;

    //
    // Comparison operator (==).
    //
	bool operator ==(
		const gstBasicTransformMatrix& rhs) const
    {
        return compare(rhs);
    }

    //
    // Comparison operator (!=)
    //
	bool operator !=(
		const gstBasicTransformMatrix& rhs) const
    {
        return !compare(rhs);
    }

	//
	// Get value at location (i,j).
	//
	double get(
		const int i,
		const int j) const
    {
        return m_elements[i][j];
    }

	//
	// Set value at location (i,j).
	//
	void set(
		const int i,
		const int j,
		const double value)
    {
        m_elements[i][j] = value;
    }

    //
    // operator() get element (i,j)
    //
    double & operator()(const int i, const int j)
    {
        return m_elements[i][j];
    }
    const double & operator()(const int i, const int j) const
    {
        return m_elements[i][j];
    }

    //
    // operator[][] returns element (i,j)
    //
	double * operator [](const int i) 
    {
		return &m_elements[i][0];
	}    
	const double * operator[](int i) const 
    {
		return &m_elements[i][0];
	}

    //
    // Matrix Multiplication
    //
    gstBasicTransformMatrix multiply(
        const gstBasicTransformMatrix & rhs) const;

    //
    // Matrix Multiplication (operator *).
    //
	gstBasicTransformMatrix operator *(
		const gstBasicTransformMatrix &rhs) const;

    //
    // Matrix Multiplication (operator *=).
    //
	gstBasicTransformMatrix & operator *=(
		const gstBasicTransformMatrix &rhs);

	//
	// post multiply point with matrix and returns 
    // resultant point. Assumes fourth coordinate is
    // 1 (picks up translation).
	//
	gstPoint postMultiply(
		const gstPoint &p) const;

	//
	// pre multiply point with matrix and returns 
    // resultant point.  Assumes fourth coordinate is 
    // 1  (picks up translation).
	//
	gstPoint preMultiply(
		const gstPoint &p) const;

	//
	// post multiply vector with matrix and returns 
    // resultant vector.  Assumes fourth coordinate is 
    // 0 (does not pick up translation).
	//
	gstVector postMultiply(
		const gstVector &v) const;

	//
	// pre multiply vector with matrix and returns 
    // resultant vector.  Assumes fourth coordinate is 
    // 0 (does not pick up translation).
	//
	gstVector preMultiply(
		const gstVector &v) const;

        //
        // Sets to identity matrix.
        //
        void makeIdentity()
    {
        gstGenericMatrix::makeIdentity4x4(*this);
    }

    //
    // Tests if matrix is an identity matrix.
    //
    bool isIdentity();

    //
    // get transpose of matrix
    //
    gstBasicTransformMatrix getTranspose() const;

    //
    // Set value of matrix to its transpose
    //
    void transpose();


    //
    // Get inverse of matrix.  'success' is set to TRUE if the matrix
    // was inverted (i.e. has an inverse), and FALSE if the matrix
    // inverse failed (e.g. the matrix was singular).  The return
    // value is indeterminate if success is FALSE.  
    // Calling getInverse without the 'success' variable will
    // also yield indeterminate results if no inverse for the matrix
    // exists.
    //
    gstBasicTransformMatrix getInverse();
    gstBasicTransformMatrix getInverse(bool &success);

    //
    // Sets value of matrix to inverse.  
    // Returns TRUE if the matrix was successfully inverted, FALSE if there
    // is no inverse to the matrix (e.g. the matrix is singular).
    // The matrix is not modified (i.e. is unchanged as a result of
    // calling the function) if the function returns FALSE.
    //
    bool invert();

    //
    // Set 4x4 array.
    //
    void set(const double a[4][4])
    {
        copy4x4(m_elements, a);
    }

    //
    // Set 4x4 array.
    //
    void set(const gstBasicTransformMatrix &m)
    {
        set(m.m_elements);
    }

    //
    // Get 4x4 array.
    //
    void get(double a[4][4]) const
    {
        copy4x4(a, m_elements);
    }

	//
	// Copies values from copyFromMat to copyToMat.
	//
	inline static void copy4x4(
		double dst[4][4],
		const  double src[4][4])
    {
        memcpy(dst, src, sizeof(double) * 16);
    }

    //
    // Returns a matrix with the given translation.
    // Translations are set along the last row of the matrix.  
    // The generated translation matrix is:
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // x y z 1
    //
    static gstBasicTransformMatrix createTranslation(const double x, 
                                                     const double y, 
                                                     const double z);
    static gstBasicTransformMatrix createTranslation(const gstPoint &p);

    //
    // Returns a matrix with the given scale.
    // The generated scale matrix is:
    // x 0 0 0
    // 0 y 0 0
    // 0 0 z 0
    // 0 0 0 1
    //
    static gstBasicTransformMatrix createScale(const double x, 
                                               const double y, 
                                               const double z);
    static gstBasicTransformMatrix createScale(const gstPoint &p);

    // 
    // Returns a matrix with the given rotation.
    // The matrix is created with a rotation of <radians> 
    // radians around the specified <v> vector.
    // <v> is required to have non-zero magnitude.  Other than that,
    // the magnitude of <v> does not affect the rotation (i.e.
    // a normalized version of <v> is used to determine the
    // rotation).
    //
    static gstBasicTransformMatrix createRotation(const gstVector &v, 
                                                  const double radians);

    // 
    // Returns a matrix with the given rotation.
    // The matrix is created with a rotation of <radians> 
    // radians around a vector with components {x,y,z}.
    // The vector {x,y,z} is required to have non-zero magnitude.  
    // Other than that, the magnitude of the vector does not affect the 
    // rotation (i.e. a normalized version of {x,y,z} is used to 
    // determine the rotation).
    //
    static gstBasicTransformMatrix createRotation(const double x, 
                                                  const double y, 
                                                  const double z, 
                                                  const double radians);

    //
    // Returns a gstBasicTransformMatrix with a
    // rotation of <radians> radians around the X axis.
    //
    static gstBasicTransformMatrix createRotationAroundX(const double radians) 
    {
      return createRotation(gstVector(1,0,0),radians);
    }

    //
    // Returns a gstBasicTransformMatrix with a
    // rotation of <angle> radians around the Y axis.
    //
    static gstBasicTransformMatrix createRotationAroundY(const double radians) 
    {
      return createRotation(gstVector(0,1,0),radians);
    }

    //
    // Returns a gstBasicTransformMatrix with a
    // rotation of <angle> radians around the Z axis.
    //
    static gstBasicTransformMatrix createRotationAroundZ(const double radians) 
    {
      return createRotation(gstVector(0,0,1),radians);
    }
	
private:

    double m_elements[4][4];

};

inline ostream& operator<<(ostream& os, 
                     const gstBasicTransformMatrix &mat)
{
    return gstGenericMatrix::output<
                              gstBasicTransformMatrix, 
                              4, 4, ostream>(os, mat);
}

inline gstPoint operator *(
	const gstPoint &pt,
	const gstBasicTransformMatrix &mat)
{
    return mat.preMultiply(pt);
}

inline gstPoint operator *(
	const gstBasicTransformMatrix &mat,
	const gstPoint &pt)
{
    return mat.postMultiply(pt);
}

inline gstVector operator *(
	const gstVector &vec,
	const gstBasicTransformMatrix &mat)
{
    return mat.preMultiply(vec);
}

inline gstVector operator *(
	const gstBasicTransformMatrix &mat,
	const gstVector &vec)
{
    return mat.postMultiply(vec);
}

//
// Constructor from array of 16 values
// stored by row.
//
inline gstBasicTransformMatrix::gstBasicTransformMatrix(
	const double * const mat)
{
  for (int i=0;i < 4 ;++i )
  {
    for (int j=0;j < 4 ;++j )
    {
	    	set(i,j,mat[4*i+j]);
    }
  }    
} 

//
// Compare matrices (returns true if all elements
// of one matrix are within epsilon of the other).
//
inline bool gstBasicTransformMatrix::compare(
                const gstBasicTransformMatrix& rhs,
                double epsilon) const
{
    return gstGenericMatrix::compare<
                gstBasicTransformMatrix,
                gstBasicTransformMatrix,
                4,4, double>(*this, rhs, epsilon);
}

//
// Matrix Multiplication
//
inline gstBasicTransformMatrix 
gstBasicTransformMatrix::multiply(
    const gstBasicTransformMatrix & rhs) const
{
    gstBasicTransformMatrix res;
    gstGenericMatrix::mulMatrixMatrix4x4(res, *this, rhs);
    return res;
}


//
// Matrix Multiplication (operator *).
//
inline gstBasicTransformMatrix 
gstBasicTransformMatrix::operator *(
	const gstBasicTransformMatrix &rhs) const
{
    gstBasicTransformMatrix res;
    gstGenericMatrix::mulMatrixMatrix4x4(res, *this, rhs);
    return res;
}

//
// Matrix Multiplication (operator *=).
//
inline gstBasicTransformMatrix & 
gstBasicTransformMatrix::operator *=(
	const gstBasicTransformMatrix &rhs)
{
    gstBasicTransformMatrix res;
    gstGenericMatrix::mulMatrixMatrix4x4(res, *this, rhs);
    set(res);
    return *this;
}

inline gstPoint gstBasicTransformMatrix::postMultiply(
	const gstPoint &p) const
{
    gstPoint res;
    gstGenericMatrix::mulMatrix4x4Point3(res, *this, p);
    return res;
}

//
// pre multiply point with matrix and returns 
// resultant point.  Assumes fourth coordinate is 
// 1  (picks up translation).
//
inline gstPoint gstBasicTransformMatrix::preMultiply(
	const gstPoint &p) const
{
    gstPoint res;
    gstGenericMatrix::mulPoint3Matrix4x4(res, p, *this);
    return res;
}

//
// post multiply vector with matrix and returns 
// resultant vector.  Assumes fourth coordinate is 
// 0 (does not pick up translation).
//
inline gstVector gstBasicTransformMatrix::postMultiply(
	const gstVector &v) const
{
    gstVector res;
    gstGenericMatrix::mulMatrix4x4Vector3(res, *this, v);
    return res;
}

//
// pre multiply vector with matrix and returns 
// resultant vector.  Assumes fourth coordinate is 
// 0 (does not pick up translation).
//
inline gstVector gstBasicTransformMatrix::preMultiply(
	const gstVector &v) const
{
    gstVector res;
    gstGenericMatrix::mulVector3Matrix4x4(res, v, *this);
    return res;
}

//
// get transpose of matrix
//
inline gstBasicTransformMatrix 
gstBasicTransformMatrix::getTranspose() const
{
    gstBasicTransformMatrix m;
    gstGenericMatrix::transpose4x4(m, *this);
    return m;
}

//
// Set value of matrix to its transpose
//
inline void gstBasicTransformMatrix::transpose()
{
  gstBasicTransformMatrix m;
  gstGenericMatrix::transpose4x4(m, *this);
  set(m);
}

//
// Sets value of matrix to its inverse.  
// Returns TRUE if the matrix was successfully inverted, FALSE if there
// is no inverse to the matrix (e.g. the matrix is singular).
// The matrix is not modified (i.e. is unchanged as a result of
// calling the function) if the function returns FALSE.
//
inline bool gstBasicTransformMatrix::invert() 
{   
  bool success;
  gstBasicTransformMatrix m = getInverse(success);
  if (success)
    set(m);
  return success;
} 

//
// Get inverse of matrix.  The result is indeterminate if
// no inverse exists for the matrix (e.g. the matrix is singular).
//
inline gstBasicTransformMatrix gstBasicTransformMatrix::getInverse() 
{
  bool success;
  return getInverse(success);
}

//
// Tests if matrix is an identity matrix.
//
inline bool gstBasicTransformMatrix::isIdentity() 
{
  gstBasicTransformMatrix i;
  i.makeIdentity();
  return compare(i,.0001);
}

#endif  // GST_BASIC_TRANFORM_MATRIX




