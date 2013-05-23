/**
 * @file GLJoe/core/mat4.h
 * 
 * @brief 4x4 matrix
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_MAT4_H
#define DEF_GLJOE_MAT4_H

namespace GLJoe
{
	
template <class T>
struct Mat4_
{
protected:
	Vec4_<T> _m[4]; // 4 rows

public:
	/**
	 * Constructors
	 */
	Mat4_<T>()
	{
		_m[0] = Vec4_<T>();
		_m[1] = Vec4_<T>();
		_m[2] = Vec4_<T>();
		_m[3] = Vec4_<T>();
	}
	
	Mat4_<T>(const T& a)
	{
		_m[0] = Vec4_<T>(a);
		_m[1] = Vec4_<T>(a);
		_m[2] = Vec4_<T>(a);
		_m[3] = Vec4_<T>(a);
	}
	
	Mat4_<T>(const Vec4_<T>& u, const Vec4_<T>& v, const Vec4_<T>& w, 
		const Vec4_<T>& t)
	{
		_m[0] = u;
		_m[1] = v;
		_m[2] = w;
		_m[3] = t;
	}
	
	Mat4_<T>(
		const T& m00, const T& m01, const T& m02, const T& m03,
		const T& m10, const T& m11, const T& m12, const T& m13,
		const T& m20, const T& m21, const T& m22, const T& m23,
		const T& m30, const T& m31, const T& m32, const T& m33)		
	{
		_m[0] = Vec4_<T>(m00, m01, m02, m03);
		_m[1] = Vec4_<T>(m10, m11, m12, m13);
		_m[2] = Vec4_<T>(m20, m21, m22, m23);
		_m[3] = Vec4_<T>(m30, m31, m32, m33);
	}
	
	Mat4_<T>(const Mat4_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
		_m[2] = m[2];
		_m[3] = m[3];
	}
	
	/**
	 * Operators
	 */
	
	Mat4_<T>& operator=(const Mat4_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
		_m[2] = m[2];
		_m[3] = m[3];
		
		return *this;
	}
	
	Mat4_<T> operator-() const
	{
		return Mat4_<T>(
			-_m[0],
			-_m[1],
			-_m[2],
			-_m[3]
		);
	}
	
	// Pointwise operations
	Mat4_<T> operator+(const Mat4_<T>& m) const
	{
		return Mat4_<T>(_m[0] + m[0], _m[1] + m[1], _m[2] + m[2], _m[3] + m[3]);
	}
	
	Mat4_<T> operator-(const Mat4_<T>& m) const
	{
		return Mat4_<T>(_m[0] - m[0], _m[1] - m[1], _m[2] - m[2], _m[3] - m[3]);
	}
	
	// Scalar operations
	Mat4_<T> operator+(const T& a) const
	{
		return Mat4_<T>(_m[0] + a, _m[1] + a, _m[2] + a, _m[3] + a);
	}
	
	Mat4_<T> operator-(const T& a) const
	{
		return Mat4_<T>(_m[0] - a, _m[1] - a, _m[2] - a, _m[3] - a);
	}
	
	Mat4_<T> operator*(const T& a) const
	{
		return Mat4_<T>(_m[0] * a, _m[1] * a, _m[2] * a, _m[3] * a);
	}
	
	Mat4_<T> operator/(const T& a) const
	{
		return Mat4_<T>(_m[0] / a, _m[1] / a, _m[2] / a, _m[3] / a);
	}
	
	friend Mat4_<T> operator*(const T& a, const Mat4_<T> m)
	{
		return m * a;
	}
	
	// Matrix multiplication	
	Mat4_<T> operator*(const Mat4_<T>& m) const
	{
		return Mat4_<T>(
			_m[0][0] * m(0, 0) + _m[0][1] * m(1, 0) + _m[0][2] * m(2, 0) + _m[0][3] * m(3, 0),
			_m[0][0] * m(0, 1) + _m[0][1] * m(1, 1) + _m[0][2] * m(2, 1) + _m[0][3] * m(3, 1),
			_m[0][0] * m(0, 2) + _m[0][1] * m(1, 2) + _m[0][2] * m(2, 2) + _m[0][3] * m(3, 2),
			_m[0][0] * m(0, 3) + _m[0][1] * m(1, 3) + _m[0][2] * m(2, 3) + _m[0][3] * m(3, 3),
			_m[1][0] * m(0, 0) + _m[1][1] * m(1, 0) + _m[1][2] * m(2, 0) + _m[1][3] * m(3, 0),
			_m[1][0] * m(0, 1) + _m[1][1] * m(1, 1) + _m[1][2] * m(2, 1) + _m[1][3] * m(3, 1),
			_m[1][0] * m(0, 2) + _m[1][1] * m(1, 2) + _m[1][2] * m(2, 2) + _m[1][3] * m(3, 2),
			_m[1][0] * m(0, 3) + _m[1][1] * m(1, 3) + _m[1][2] * m(2, 3) + _m[1][3] * m(3, 3),
			_m[2][0] * m(0, 0) + _m[2][1] * m(1, 0) + _m[2][2] * m(2, 0) + _m[2][3] * m(3, 0),
			_m[2][0] * m(0, 1) + _m[2][1] * m(1, 1) + _m[2][2] * m(2, 1) + _m[2][3] * m(3, 1),
			_m[2][0] * m(0, 2) + _m[2][1] * m(1, 2) + _m[2][2] * m(2, 2) + _m[2][3] * m(3, 2),
			_m[2][0] * m(0, 3) + _m[2][1] * m(1, 3) + _m[2][2] * m(2, 3) + _m[2][3] * m(3, 3),
			_m[3][0] * m(0, 0) + _m[3][1] * m(1, 0) + _m[3][2] * m(2, 0) + _m[3][3] * m(3, 0),
			_m[3][0] * m(0, 1) + _m[3][1] * m(1, 1) + _m[3][2] * m(2, 1) + _m[3][3] * m(3, 1),
			_m[3][0] * m(0, 2) + _m[3][1] * m(1, 2) + _m[3][2] * m(2, 2) + _m[3][3] * m(3, 2),
			_m[3][0] * m(0, 3) + _m[3][1] * m(1, 3) + _m[3][2] * m(2, 3) + _m[3][3] * m(3, 3)
		);
	}
	
	// Multiplication by a vector
	Vec4_<T> operator*(const Vec4_<T>& v) const
	{
		return Vec4_<T>(
			_m[0][0] * v(0) + _m[0][1] * v(1) + _m[0][2] * v(2) + _m[0][3] * v(3),
			_m[1][0] * v(0) + _m[1][1] * v(1) + _m[1][2] * v(2) + _m[1][3] * v(3),
			_m[2][0] * v(0) + _m[2][1] * v(1) + _m[2][2] * v(2) + _m[2][3] * v(3),
			_m[3][0] * v(0) + _m[3][1] * v(1) + _m[3][2] * v(2) + _m[3][3] * v(3)
		);
	}
	
	// Modifying operations
	Mat4_<T>& operator+=(const Mat4_<T>& m)
	{
		*this = *this + m;
		return *this;
	}
	
	Mat4_<T>& operator-=(const Mat4_<T>& m)
	{
		*this = *this - m;
		return *this;
	}
	
	Mat4_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}
	
	Mat4_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}
	
	Mat4_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}
	
	Mat4_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	Mat4_<T>& operator*=(const Mat4_<T>& m)
	{
		*this = *this * m;
		return *this;
	}
	
	// Indexing operators
	const Vec4_<T>& operator[](int index) const
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	Vec4_<T>& operator[](int index)
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	const T& operator()(int i, int j) const
	{
		if (i < 0 || i > 3 || j < 0 || j > 3)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	T& operator()(int i, int j)
	{
		if (i < 0 || i > 3 || j < 0 || j > 3)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Mat4_<T>& m)
	{
		return os << std::endl << m[0] << std::endl << m[1] << std::endl
			<< m[2] << std::endl << m[3] << std::endl;
	}
	 
	friend std::istream& operator>>(std::istream& is, Mat4_<T>& m)
	{
		return is >> m[0] >> m[1] >> m[2] >> m[3];
	}
	
	// Conversion operators
	operator const T*() const
	{
		return static_cast<const T*>(&_m[0][0]);
	}

	operator T*()
	{
		return static_cast<T*>(&_m[0][0]);
	}
	
	/**
	 * Basic operations
	 */
	T det() const
	{
		T	a = _m[0][0],
			b = _m[0][1],
			c = _m[0][2],
			d = _m[0][3],
			e = _m[1][0],
			f = _m[1][1],
			g = _m[1][2],
			h = _m[1][3],
			i = _m[2][0],
			j = _m[2][1],
			k = _m[2][2],
			l = _m[2][3],
			m = _m[3][0],
			n = _m[3][1],
			o = _m[3][2],
			p = _m[3][3];
			
		return a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n 
			- b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m 
			+ c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m 
			- d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m;
	}
	
	Mat4_<T> transpose() const
	{
		return Mat4_<T>(
			_m[0][0], _m[1][0], _m[2][0], _m[3][0],
			_m[0][1], _m[1][1], _m[2][1], _m[3][1],
			_m[0][2], _m[1][2], _m[2][2], _m[3][2],
			_m[0][3], _m[1][3], _m[2][3], _m[3][3]
		);
	}
	
	Mat4_<T> inverse() const
	{
		T	a = _m[0][0],
			b = _m[0][1],
			c = _m[0][2],
			d = _m[0][3],
			e = _m[1][0],
			f = _m[1][1],
			g = _m[1][2],
			h = _m[1][3],
			i = _m[2][0],
			j = _m[2][1],
			k = _m[2][2],
			l = _m[2][3],
			m = _m[3][0],
			n = _m[3][1],
			o = _m[3][2],
			p = _m[3][3];

		return Mat4_<T>(
			(f*k*p - f*l*o - g*j*p + g*l*n + h*j*o - h*k*n)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(b*k*p - b*l*o - c*j*p + c*l*n + d*j*o - d*k*n)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (b*g*p - b*h*o - c*f*p + c*h*n + d*f*o - d*g*n)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(b*g*l - b*h*k - c*f*l + c*h*j + d*f*k - d*g*j)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(e*k*p - e*l*o - g*i*p + g*l*m + h*i*o - h*k*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (a*k*p - a*l*o - c*i*p + c*l*m + d*i*o - d*k*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(a*g*p - a*h*o - c*e*p + c*h*m + d*e*o - d*g*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (a*g*l - a*h*k - c*e*l + c*h*i + d*e*k - d*g*i)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), (e*j*p - e*l*n - f*i*p + f*l*m + h*i*n - h*j*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(a*j*p - a*l*n - b*i*p + b*l*m + d*i*n - d*j*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (a*f*p - a*h*n - b*e*p + b*h*m + d*e*n - d*f*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(a*f*l - a*h*j - b*e*l + b*h*i + d*e*j - d*f*i)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(e*j*o - e*k*n - f*i*o + f*k*m + g*i*n - g*j*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (a*j*o - a*k*n - b*i*o + b*k*m + c*i*n - c*j*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m), -(a*f*o - a*g*n - b*e*o + b*g*m + c*e*n - c*f*m)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m),  (a*f*k - a*g*j - b*e*k + b*g*i + c*e*j - c*f*i)/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m)
		);
	}
	
};

typedef Mat4_<GLfloat> Mat4;
typedef Mat4_<GLfloat> Mat4f;
typedef Mat4_<GLdouble> Mat4d;
typedef Mat4_<GLint> Mat4i;

} // namespace GLJoe

#endif // DEF_GLJOE_MAT4_H

