/**
 * @file GLJoe/core/mat3.h
 * 
 * @brief 3x3 matrix
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_MAT3_H
#define DEF_GLJOE_MAT3_H

namespace GLJoe
{
	
template <class T>
struct Mat3_
{
protected:
	Vec3_<T> _m[3]; // 3 rows

public:
	/**
	 * Constructors
	 */
	Mat3_<T>()
	{
		_m[0] = Vec3_<T>();
		_m[1] = Vec3_<T>();
		_m[2] = Vec3_<T>();
	}
	
	Mat3_<T>(const T& a)
	{
		_m[0] = Vec3_<T>(a);
		_m[1] = Vec3_<T>(a);
		_m[2] = Vec3_<T>(a);
	}
	
	Mat3_<T>(const Vec3_<T>& u, const Vec3_<T>& v, const Vec3_<T>& w)
	{
		_m[0] = u;
		_m[1] = v;
		_m[2] = w;
	}
	
	Mat3_<T>(
		const T& m00, const T& m01, const T& m02,
		const T& m10, const T& m11, const T& m12,
		const T& m20, const T& m21, const T& m22)
	{
		_m[0] = Vec3_<T>(m00, m01, m02);
		_m[1] = Vec3_<T>(m10, m11, m12);
		_m[2] = Vec3_<T>(m20, m21, m22);
	}
	
	Mat3_<T>(const Mat3_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
		_m[2] = m[2];
	}
	
	/**
	 * Operators
	 */
	
	Mat3_<T>& operator=(const Mat3_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
		_m[2] = m[2];
		
		return *this;
	}
	
	Mat3_<T> operator-() const
	{
		return Mat3_<T>(
			-_m[0],
			-_m[1],
			-_m[2]
		);
	}
	
	// Pointwise operations
	Mat3_<T> operator+(const Mat3_<T>& m) const
	{
		return Mat3_<T>(_m[0] + m[0], _m[1] + m[1], _m[2] + m[2]);
	}
	
	Mat3_<T> operator-(const Mat3_<T>& m) const
	{
		return Mat3_<T>(_m[0] - m[0], _m[1] - m[1], _m[2] - m[2]);
	}
	
	// Scalar operations
	Mat3_<T> operator+(const T& a) const
	{
		return Mat3_<T>(_m[0] + a, _m[1] + a, _m[2] + a);
	}
	
	Mat3_<T> operator-(const T& a) const
	{
		return Mat3_<T>(_m[0] - a, _m[1] - a, _m[2] - a);
	}
	
	Mat3_<T> operator*(const T& a) const
	{
		return Mat3_<T>(_m[0] * a, _m[1] * a, _m[2] * a);
	}
	
	Mat3_<T> operator/(const T& a) const
	{
		return Mat3_<T>(_m[0] / a, _m[1] / a, _m[2] / a);
	}
	
	friend Mat3_<T> operator*(const T& a, const Mat3_<T> m)
	{
		return m * a;
	}
	
	// Matrix multiplication	
	Mat3_<T> operator*(const Mat3_<T>& m) const
	{
		return Mat3_<T>(
			_m[0][0] * m(0, 0) + _m[0][1] * m(1, 0) + _m[0][2] * m(2, 0),
			_m[0][0] * m(0, 1) + _m[0][1] * m(1, 1) + _m[0][2] * m(2, 1),
			_m[0][0] * m(0, 2) + _m[0][1] * m(1, 2) + _m[0][2] * m(2, 2),
			_m[1][0] * m(0, 0) + _m[1][1] * m(1, 0) + _m[1][2] * m(2, 0),
			_m[1][0] * m(0, 1) + _m[1][1] * m(1, 1) + _m[1][2] * m(2, 1),
			_m[1][0] * m(0, 2) + _m[1][1] * m(1, 2) + _m[1][2] * m(2, 2),
			_m[2][0] * m(0, 0) + _m[2][1] * m(1, 0) + _m[2][2] * m(2, 0),
			_m[2][0] * m(0, 1) + _m[2][1] * m(1, 1) + _m[2][2] * m(2, 1),
			_m[2][0] * m(0, 2) + _m[2][1] * m(1, 2) + _m[2][2] * m(2, 2)
		);
	}
	
	// Multiplication by a vector
	Vec3_<T> operator*(const Vec3_<T>& v) const
	{
		return Vec3_<T>(
			_m[0][0] * v(0) + _m[0][1] * v(1) + _m[0][2] * v(2),
			_m[1][0] * v(0) + _m[1][1] * v(1) + _m[1][2] * v(2),
			_m[2][0] * v(0) + _m[2][1] * v(1) + _m[2][2] * v(2)
		);
	}
	
	// Modifying operations
	Mat3_<T>& operator+=(const Mat3_<T>& m)
	{
		*this = *this + m;
		return *this;
	}
	
	Mat3_<T>& operator-=(const Mat3_<T>& m)
	{
		*this = *this - m;
		return *this;
	}
	
	Mat3_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}
	
	Mat3_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}
	
	Mat3_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}
	
	Mat3_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	Mat3_<T>& operator*=(const Mat3_<T>& m)
	{
		*this = *this * m;
		return *this;
	}
	
	// Indexing operators
	const Vec3_<T>& operator[](int index) const
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	Vec3_<T>& operator[](int index)
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	const T& operator()(int i, int j) const
	{
		if (i < 0 || i > 2 || j < 0 || j > 2)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	T& operator()(int i, int j)
	{
		if (i < 0 || i > 2 || j < 0 || j > 2)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Mat3_<T>& m)
	{
		return os << std::endl << m[0] << std::endl << m[1] << std::endl
			<< m[2] << std::endl;
	}
	 
	friend std::istream& operator>>(std::istream& is, Mat3_<T>& m)
	{
		return is >> m[0] >> m[1] >> m[2];
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
			d = _m[1][0],
			e = _m[1][1],
			f = _m[1][2],
			g = _m[2][0],
			h = _m[2][1],
			i = _m[2][2];
		return a*e*i + b*f*g + c*d*h - a*f*h - b*d*i - c*e*g;
	}
	
	Mat3_<T> transpose() const
	{
		return Mat3_<T>(
			_m[0][0], _m[1][0], _m[2][0],
			_m[0][1], _m[1][1], _m[2][1],
			_m[0][2], _m[1][2], _m[2][2]
		);
	}
	
	Mat3_<T> inverse() const
	{
		T	a = _m[0][0],
			b = _m[0][1],
			c = _m[0][2],
			d = _m[1][0],
			e = _m[1][1],
			f = _m[1][2],
			g = _m[2][0],
			h = _m[2][1],
			i = _m[2][2];
		
		return Mat3_<T>(
			(e*i - f*h)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g), -(b*i - c*h)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g),  (b*f - c*e)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g), -(d*i - f*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g),  (a*i - c*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g), -(a*f - c*d)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g), (d*h - e*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g), -(a*h - b*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g),  (a*e - b*d)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g)
		);
		
	}
	
};

typedef Mat3_<GLfloat> Mat3;
typedef Mat3_<GLfloat> Mat3f;
typedef Mat3_<GLdouble> Mat3d;
typedef Mat3_<GLint> Mat3i;

} // namespace GLJoe

#endif // DEF_GLJOE_MAT3_H

