#ifndef NEW_TRANSFORM_MATRIX
#define NEW_TRANSFORM_MATRIX

#include <gstBasicTransformMatrix.h>
#include <gstPoint.h>
#include <gstVector.h>
#include <gstDllExport.h>
#include <gstTypedefs.h>
#include <gstErrorHandler.h>

// Homogeneous 3d tranformation matrix class.
// Order of operations is center, scale, rotation, then
// translation unless matrix is user defined.  A
// matrix becomes user defined if one or more entries
// of the 4x4 array have been set explicitly.  If
// a matrix is user defined then no assumptions are
// made about its form.  
class GHOST_DLL_IMPORT_EXPORT gstTransformMatrix {

public:
	
	// Constructor.
	gstTransformMatrix();
	
	// Constructor (makes matrix userDefined).
	gstTransformMatrix(double,double,double,
		double,double,double,
		double,double,double);
	
	// Constructor (makes matrix userDefined).
	gstTransformMatrix(double,double,double,double,
		double,double,double,double,
		double,double,double,double,
		double,double,double,double);

	// Constructor from array of 16 values stored by row.
    gstTransformMatrix(const double *mat);

    // Constructor from gstBasicTransformMatrix
    gstTransformMatrix(const gstBasicTransformMatrix &mat);
	
	friend class gstRigidBody;
	friend class gstRotateManipulator;
	
    //
    // get transpose of matrix
    //
    gstTransformMatrix getTranspose() const;

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
    gstTransformMatrix getInverse();
    gstTransformMatrix getInverse(bool &success);

    //
    // Sets value of matrix to inverse.  
    // Returns TRUE if the matrix was successfully inverted, FALSE if there
    // is no inverse to the matrix (e.g. the matrix is singular).
    // The matrix is not modified (i.e. is unchanged as a result of
    // calling the function) if the function returns FALSE.
    //
	bool invert();
		
	// Set row "i" and column "j" of matrix to "value"
	// (makes matrix userDefined).
	void set(int i, int j, double value);

    // (makes matrix userDefined).
    void set(const double mat[4][4]);

    // (makes matrix userDefined).
    void set(const gstBasicTransformMatrix &mat);
	
	// Get value of row "i" and column "j" of matrix.
	double get(int i, int j) const;

    // Get the matrix as a gstBasicTransformMatrix
    gstBasicTransformMatrix getBasicTransformMatrix() const;

    // Get opengl matrix, for passing to glLoadMatrix for example
    const double *getOpenGLMatrix() const;
	
	// Returns TRUE if user has explicitely set matrix
	// (i.e. by setting elements in the matrix directly instead of using
	// "setCenter", "setScale", "setRotation", or "setTranslation" operations).  
	// "getCenter", "getScale", "getRotation", and "getTranslation"
	// operations will not return valid information if a matrix
	// is user defined and will cause a GST_TRANSFORM_RESET_ERROR
	int getUserDefined() const { return userDefined; }	
	
	// Get x,y,z translation.
	gstPoint getTranslation() const;

	// Get x,y,z translation.
	void getTranslation(gstPoint &translationArg) const;
	
	// Get x,y,z coordinates of center.
	gstPoint getCenter() const;
	
	// Get x,y,z coordinates of center.
	void getCenter(gstPoint &centerArg) const;
	
	// Get 3x3 scale orientation matrix.
	void getScaleOrientationMatrix(gstTransformMatrix &transfMat) const;
	
	// Get scale factors along axis' defined by scale orientation.
	gstPoint getScaleFactor() const;

	// Get scale factors along axis' defined by scale orientation.
	void getScaleFactor(gstPoint &scaleFactor) const;  
	
	// Get 3x3 rotation matrix.
	void getRotationMatrix(gstTransformMatrix &transfMat) const;

	// Get rotation matrix.
	gstTransformMatrix getRotationMatrix() const;
	
	// Get 3x3 rotation matrix.
	void getRotationMatrix(double transfMat[3][3]) const;
	
	// Get equivalent rotation based on successive rotations around x,y,z axes.
	gstPoint getRotationAngles() const;
	
	// Get equivilant rotation of current rotation matrix (orientation) based 
	// on successive rotations around x,y,z axes.  Angles are in radians
	// and use right hand rule. The values returned can be positive or
	// negative rotation angles relative to 0. If you need to compare
	// rotation values, you should first convert them all to positive
	// rotations. For instance:
	//
	//	axes.setx(axes.x() < 0 ? 2 * M_PI + axes.x() : axes.x());
	//	axes.sety(axes.y() < 0 ? 2 * M_PI + axes.y() : axes.y());
	//	axes.setz(axes.z() < 0 ? 2 * M_PI + axes.z() : axes.z());
	//
	void getRotationAngles(gstPoint &axes) const;
	
	// FOR_GHOST_INTERNAL
	// Updates 4x4 homogenous matrix with values from center, scale,
	// rotate, and translate fields.
	void update() const;
	
	// FOR_GHOST_INTERNAL
	// Updates 4x4 inverse matrix with values from center, scale,
	// rotate, and translate fields.
	void updateInverse();
	
	// FOR_GHOST_INTERNAL
	// Sets center, scale, translate, and rotate scales to defaults.
	void resetFields();
	
	// Sets matrix to identity matrix.
	void identity();
	
	// Returns TRUE if the matrix is the identity matrix.
	gstBoolean isIdentity() const {	return _identity; }	
	
	// Set x,y,z components of center.
	void setCenter(const gstPoint &newCenter);
	
	// Set x,y,z components of center.
	void setCenter( double argX, double argY, double argZ );
	
	// Set x,y,z components of scale.
	void setScale(const gstPoint &newScale);
	
	// Set x,y,z components of scale.
	void setScale( double argX, double argY, double argZ );
	
	// Set x,y,z components of translate.
	void setTranslation(const gstPoint &newTranslation);
	
	// Set x,y,z components of translate.
	void setTranslation( double argX, double argY, double argZ );
		
	// Set rotation using vector/axis angle approach.  Axis is in radians.
	void setRotation(const gstVector &, double);
	
	// Set rotation matrix (row 0 : columns 0-2, row 1 : columns 0-2,
	// row 2 : columns 0-2).
	void setRotationMatrix(double r00,double r01, double r02,
						   double r10,double r11, double r12,
						   double r20,double r21, double r22);
	
	// Set rotation matrix to user defined 3x3 matrix.
	void setRotationMatrix(const double r[3][3]);
	
	// Add "newTranslation" to existing translation.
	void translate(const gstPoint &newTranslation);
	
	// add new x,y,z components to exisiting translation.
	void translate( double argX, double argY, double argZ );
	
	// Add "newScale" to existing scale.
	void scale(const gstPoint &newScale);
	
	// Add new x,y,z components to existing scale,
	void scale( double argX, double argY, double argZ );
	
	// Add new rotation specified by vector/axis angle approach
	// to existing rotation.  Angle is in radians.
    // this version right multiplies: newRot = currentRot * givenRot
	void rotateRM(const gstVector &, double radians);

    // Add new rotation specified by vector/axis angle approach
	// to existing rotation.  Angle is in radians.
    // this version left multiplies: newRot = givenRot * currentRot
	void rotateLM(const gstVector &axis, double radians);
    
	// Print values of matrix.
	void printSelf();
	
	// Performs matrix mutiplication, putting the computed matrix into result.
	// NOTE: This operation will fail if result is either matrix a or b.  In
	// those cases, assign the answer to a temporary matrix, then copy that matrix
	// into result.
	friend GHOST_DLL_IMPORT_EXPORT void mulM (
        gstTransformMatrix &result,
        const gstTransformMatrix &a,
        const gstTransformMatrix &b);	
	
	// Transform vector in parent reference frame to local reference
	// frame. Preserves vector length.
	gstVector toLocal(const gstVector &);
	
	// Transform point from local reference frame to parent
	// reference frame.
	gstPoint fromLocal(const gstPoint &p);
	
	// Transform point from local reference frame to parent
	// reference frame.  Preserves vector length.
	gstVector fromLocal(const gstVector &);
	
	// Transform point in parent reference frame to local reference
	// frame.  
	gstPoint toLocal(const gstPoint &p);
		
    //
    // operator() get element (i,j)
    //
    double & operator()(const int i, const int j);
    const double & operator()(const int i, const int j) const;
	
    // Same as operator==
    bool compare(
        const gstTransformMatrix& rhs) const;

	bool operator ==(
		const gstTransformMatrix& rhs) const;

	bool operator !=(
		const gstTransformMatrix& rhs) const;

    // A user defined gstTransformMatrix will be reset to identity if
    // a "convenience function" like "translate" or "rotate" is called
    // on it.  By default this reset triggers a GHOST error.  Use
    // setSignalResetError to turn this error reporting on/off.
    static void setSignalResetError(bool signal);

    gstInternal public:

    // Proxy classes used to differentiate read/write to keep
    // correct "user defined" state.  Note this is much slower
    // than using the operator() or set/get, it is best not to
    // used [][] addressing at all for gstTransformMatrix
    class Proxy1D;

    class Proxy
    {
        friend ostream & operator << (ostream& os, const Proxy & e);
        friend class Proxy1D;
        
        public:
        
            GHOST_INLINE Proxy & operator =(const double value);
            GHOST_INLINE Proxy & operator =(const Proxy &proxy);
            GHOST_INLINE Proxy & operator +=(const double value);
            GHOST_INLINE Proxy & operator -=(const double value);
            GHOST_INLINE Proxy & operator /=(const double value);
            GHOST_INLINE Proxy & operator *=(const double value);
        
            GHOST_INLINE operator double() const;
        
        private:
        
            gstTransformMatrix *m_mat;
            int m_i, m_j;
        
            GHOST_INLINE Proxy(gstTransformMatrix *matrix, const int i, const int j);
            GHOST_INLINE Proxy(const gstTransformMatrix *matrix, const int i, const int j);
            GHOST_INLINE Proxy(const Proxy &pElem);
    };

    class Proxy1D
    {       
        friend class gstTransformMatrix;

        public:
        
            GHOST_INLINE const Proxy operator [](const int j) const;
            GHOST_INLINE Proxy operator [](const int j);
            GHOST_INLINE Proxy1D & operator =(const gstPoint value);
            GHOST_INLINE Proxy1D & operator =(const Proxy1D &proxy);
        
            GHOST_INLINE operator gstPoint() const;
        
        private:
        
            gstTransformMatrix * m_mat;
            int m_i;
        
            GHOST_INLINE Proxy1D(gstTransformMatrix *matrix,	const int i);
            GHOST_INLINE Proxy1D(const gstTransformMatrix * matrix, const int i);
            GHOST_INLINE Proxy1D(const Proxy1D &);
    };
    

    GHOST_INLINE const Proxy1D operator [](const int i) const;
    GHOST_INLINE Proxy1D operator [](const int i);

	// FOR_GHOST_INTERNAL
	// Used by system or for creating sub-classes only.
	// Set row "i" and column "j" of matrix to value.
	// Use only if you are on a run of set calls
	// to the same object and you used set first.
	void setQuick(int i, int j, double value) 
	{
		_a[i][j] = value;
	}    
    
    // DEPRECATED: Name changed for consistency
    void inverse() { invert(); }

    // DEPRECATED: Name changed for consistency
    void setTranslate(const gstPoint &newTranslation) {
        setTranslation(newTranslation);
    }

    // DEPRECATED: Name changed for consistency
    void setTranslate(double argX, double argY, double argZ) {
        setTranslation(argX, argY, argZ);
    }

    // DEPRECATED: Name changed for consistency
    void setRotate(const gstVector &v, double a) { setRotation(v, a); }

    // DEPRECATED: Name change for consistency
    void rotate(const gstVector &v, double a) { rotateLM(v, a); }

private:

	gstBasicTransformMatrix _a;
	gstBasicTransformMatrix _aInv;
	double _r[3][3];
	double translateX, translateY, translateZ;
	double centerX, centerY, centerZ;
	double scaleX, scaleY, scaleZ;
	int	  userDefined, current, currentInv;
    int updating;
	gstBoolean _identity;

    static bool signalResetError;

    friend GHOST_DLL_IMPORT_EXPORT ostream&
        operator <<(ostream& os, const gstTransformMatrix &mat);
    friend GHOST_DLL_IMPORT_EXPORT gstPoint
        operator *(const gstPoint &pt, const gstTransformMatrix &mat);
    friend GHOST_DLL_IMPORT_EXPORT gstPoint
        operator *(const gstTransformMatrix &mat, const gstPoint &pt);
    friend GHOST_DLL_IMPORT_EXPORT gstVector
        operator *(const gstVector &vec, const gstTransformMatrix &mat);
    friend GHOST_DLL_IMPORT_EXPORT gstVector
        operator *(const gstTransformMatrix &mat, const gstVector &vec);
};


inline void gstTransformMatrix::getTranslation(
    gstPoint &translationArg) const
{ 
	if (userDefined) {
		translationArg.init(_a[3][0], _a[3][1], _a[3][2]);
		return;
	}
	translationArg.init(translateX, translateY, translateZ);
}


inline gstPoint gstTransformMatrix::getTranslation() const
{
	gstPoint r;
	getTranslation(r);
	return r;
}


inline gstPoint gstTransformMatrix::getCenter() const
{
    gstPoint r;
    getCenter(r);
    return r;
}


inline void gstTransformMatrix::getCenter(gstPoint &centerArg) const
{ 
	if (userDefined) {
		centerArg.init(0.0,0.0,0.0);
		return;
	}
	centerArg.init(centerX, centerY, centerZ);
}


inline gstPoint gstTransformMatrix::getScaleFactor() const
{
	gstPoint r;
	getScaleFactor(r);
	return r;
}


inline gstTransformMatrix gstTransformMatrix::getRotationMatrix() const
{
    gstTransformMatrix cM;
    getRotationMatrix(cM);
    return cM;
}


inline gstPoint gstTransformMatrix::getRotationAngles() const
{
    gstPoint axes;
    getRotationAngles(axes);
    return axes;
}


inline void gstTransformMatrix::set(int i, int j, double value) 
{
    update();
	current = FALSE;
	currentInv = FALSE;
	if (!userDefined) 
	{
		_identity = FALSE;
		userDefined = TRUE;
		resetFields();
	}
	_a[i][j] = value;
}


inline double gstTransformMatrix::get(int i, int j) const
{        
    update();
	return _a[i][j]; 
}

inline const double *gstTransformMatrix::getOpenGLMatrix() const
{
    update();
    return &_a[0][0];
}

inline gstBasicTransformMatrix gstTransformMatrix::getBasicTransformMatrix() const
{
    update();
    return _a;
}

GHOST_INLINE double & gstTransformMatrix::operator()(
    const int i, const int j)
{
    update();
	current = FALSE;
	currentInv = FALSE;
	if (!userDefined) 
	{
		_identity = FALSE;
		userDefined = TRUE;
		resetFields();
	}
    return _a[i][j];
}

inline const double & gstTransformMatrix::operator()(
    const int i, const int j) const
{
    update();
    return _a[i][j];
}

GHOST_INLINE const gstTransformMatrix::Proxy1D gstTransformMatrix::operator [](
	const int i) const
{
	return gstTransformMatrix::Proxy1D(this, i);
}


GHOST_INLINE gstTransformMatrix::Proxy1D gstTransformMatrix::operator [](
	const int i)
{
	return gstTransformMatrix::Proxy1D(this, i);
}


GHOST_INLINE bool gstTransformMatrix::operator !=(
    const gstTransformMatrix& rhs) const
{
    return !(*this == rhs);
}


GHOST_INLINE gstPoint operator *(
	const gstPoint &pt,
	const gstTransformMatrix &mat)
{
    mat.update();
    return mat._a.preMultiply(pt);
}

GHOST_INLINE gstPoint operator *(
	const gstTransformMatrix &mat,
	const gstPoint &pt)
{
    mat.update();
    return mat._a.postMultiply(pt);
}

GHOST_INLINE gstVector operator *(
	const gstVector &vec,
	const gstTransformMatrix &mat)
{
    mat.update();
    return mat._a.preMultiply(vec);
}

GHOST_INLINE gstVector operator *(
	const gstTransformMatrix &mat,
	const gstVector &vec)
{
    mat.update();
    return mat._a.postMultiply(vec);
}


GHOST_DLL_IMPORT_EXPORT ostream& operator<< (
    ostream& os,
    const gstTransformMatrix &mat);

GHOST_INLINE GHOST_DLL_IMPORT_EXPORT gstTransformMatrix operator *(
    gstTransformMatrix& a,
    gstTransformMatrix& b)
{
    gstTransformMatrix result;
    mulM(result,a,b);
    return result;
}

// we don't care about success return value
GHOST_INLINE gstTransformMatrix gstTransformMatrix::getInverse()
{
    bool success;
    return getInverse(success);
}

GHOST_INLINE bool gstTransformMatrix::operator ==(
    const gstTransformMatrix& rhs) const
{
    return compare(rhs);
}

GHOST_INLINE gstTransformMatrix::Proxy::Proxy(
	gstTransformMatrix *matrix,
	const int i,
	const int j):

	m_mat(matrix),
	m_i(i),
	m_j(j)
{

}


GHOST_INLINE gstTransformMatrix::Proxy::Proxy(
	const gstTransformMatrix *matrix,
	const int i,
	const int j):

	m_mat((gstTransformMatrix *) matrix),
	m_i(i),
	m_j(j)
{

}


GHOST_INLINE gstTransformMatrix::Proxy::Proxy(
		const gstTransformMatrix::Proxy &pElem)
{
    m_mat = pElem.m_mat;
    m_i = pElem.m_i;
    m_j = pElem.m_j;
}

GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator =(
	const double value)
{
	m_mat->set(m_i, m_j, value);
	return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator =(
	const gstTransformMatrix::Proxy &proxy)
{
	m_mat->set(m_i, m_j, proxy);
	return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator +=(
    const double value)
{
    m_mat->set(m_i, m_j, m_mat->get(m_i, m_j) + value);
    return *this;
}

GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator -=(
    const double value)
{
    m_mat->set(m_i, m_j, m_mat->get(m_i, m_j) - value);
    return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator /=(
    const double value)
{
    m_mat->set(m_i, m_j, m_mat->get(m_i, m_j) / value);
    return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy & gstTransformMatrix::Proxy::operator *=(
    const double value)
{
    m_mat->set(m_i, m_j, m_mat->get(m_i, m_j) * value);
    return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy::operator double() const
{
	return(m_mat->get(m_i,m_j));
}


GHOST_INLINE ostream & operator << (ostream& os, const gstTransformMatrix::Proxy & e)
{
    os << e.m_mat->get(e.m_i, e.m_j);

    return os;
}



GHOST_INLINE gstTransformMatrix::Proxy1D::Proxy1D(
	gstTransformMatrix *matrix,
	const int i):

	m_mat(matrix),
	m_i(i)
{

}



GHOST_INLINE gstTransformMatrix::Proxy1D::Proxy1D(
	const gstTransformMatrix *matrix,
	const int i):

	m_mat((gstTransformMatrix *) matrix),
	m_i(i)
{

}


GHOST_INLINE gstTransformMatrix::Proxy1D::Proxy1D(
    const gstTransformMatrix::Proxy1D &proxy)
{
    m_mat = proxy.m_mat;
    m_i = proxy.m_i;
}


GHOST_INLINE const gstTransformMatrix::Proxy gstTransformMatrix::Proxy1D::operator [](
	const int j) const
{
	return gstTransformMatrix::Proxy(m_mat, m_i, j);
}


GHOST_INLINE gstTransformMatrix::Proxy gstTransformMatrix::Proxy1D::operator [](
	const int j)
{
	return gstTransformMatrix::Proxy(m_mat, m_i, j);
}


GHOST_INLINE gstTransformMatrix::Proxy1D & gstTransformMatrix::Proxy1D::operator =(
	const gstPoint value)
{
	m_mat->set(m_i, 0, value[0]);
	m_mat->set(m_i, 1, value[1]);
	m_mat->set(m_i, 2, value[2]);
	return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy1D & gstTransformMatrix::Proxy1D::operator =(
    const gstTransformMatrix::Proxy1D &proxy)
{
	m_mat->set(m_i, 0, proxy[0]);
	m_mat->set(m_i, 1, proxy[1]);
	m_mat->set(m_i, 2, proxy[2]);
	return *this;
}


GHOST_INLINE gstTransformMatrix::Proxy1D::operator gstPoint() const
{
	return gstPoint(m_mat->get(m_i, 0), m_mat->get(m_i, 1), m_mat->get(m_i, 2));
}

#endif
