/**
 * @file GLJoe/core/mat2.h
 * 
 * @brief 2x2 matrix
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_MAT2_H
#define DEF_GLJOE_MAT2_H

namespace GLJoe
{
	
template <class T>
struct Mat2_
{
protected:
	Vec2_<T> _m[2]; // 2 rows

public:
	/**
	 * Constructors
	 */
	Mat2_<T>()
	{
		_m[0] = Vec2_<T>();
		_m[1] = Vec2_<T>();
	}
	
	Mat2_<T>(const T& a)
	{
		_m[0] = Vec2_<T>(a);
		_m[1] = Vec2_<T>(a);
	}
	
	Mat2_<T>(const Vec2_<T>& u, const Vec2_<T>& v)
	{
		_m[0] = u;
		_m[1] = v;
	}
	
	Mat2_<T>(const T& m00, const T& m01, const T& m10, const T& m11)
	{
		_m[0] = Vec2_<T>(m00, m01);
		_m[1] = Vec2_<T>(m10, m11);
	}
	
	Mat2_<T>(const Mat2_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
	}
	
	/**
	 * Operators
	 */
	
	Mat2_<T>& operator=(const Mat2_<T>& m)
	{
		_m[0] = m[0];
		_m[1] = m[1];
		
		return *this;
	}
	
	Mat2_<T> operator-() const
	{
		return Mat2_<T>(
			-_m[0],
			-_m[1]
		);
	}
	
	// Pointwise operations
	Mat2_<T> operator+(const Mat2_<T>& m) const
	{
		return Mat2_<T>(_m[0] + m[0], _m[1] + m[1]);
	}
	
	Mat2_<T> operator-(const Mat2_<T>& m) const
	{
		return Mat2_<T>(_m[0] - m[0], _m[1] - m[1]);
	}
	
	// Scalar operations
	Mat2_<T> operator+(const T& a) const
	{
		return Mat2_<T>(_m[0] + a, _m[1] + a);
	}
	
	Mat2_<T> operator-(const T& a) const
	{
		return Mat2_<T>(_m[0] - a, _m[1] - a);
	}
	
	Mat2_<T> operator*(const T& a) const
	{
		return Mat2_<T>(_m[0] * a, _m[1] * a);
	}
	
	Mat2_<T> operator/(const T& a) const
	{
		return Mat2_<T>(_m[0] / a, _m[1] / a);
	}
	
	friend Mat2_<T> operator*(const T& a, const Mat2_<T> m)
	{
		return m * a;
	}
	
	// Matrix multiplication	
	Mat2_<T> operator*(const Mat2_<T>& m) const
	{
		return Mat2_<T>(
			_m[0][0] * m(0, 0) + _m[0][1] * m(1, 0),
			_m[0][0] * m(0, 1) + _m[0][1] * m(1, 1),
			_m[1][0] * m(0, 0) + _m[1][1] * m(1, 0),
			_m[1][0] * m(0, 1) + _m[1][1] * m(1, 1)
		);
	}
	
	// Multiplication by a vector
	Vec2_<T> operator*(const Vec2_<T>& v) const
	{
		return Vec2_<T>(
			_m[0][0] * v(0) + _m[0][1] * v(1),
			_m[1][0] * v(0) + _m[1][1] * v(1)			
		);
	}
	
	// Modifying operations
	Mat2_<T>& operator+=(const Mat2_<T>& m)
	{
		*this = *this + m;
		return *this;
	}
	
	Mat2_<T>& operator-=(const Mat2_<T>& m)
	{
		*this = *this - m;
		return *this;
	}
	
	Mat2_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}
	
	Mat2_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}
	
	Mat2_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}
	
	Mat2_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	Mat2_<T>& operator*=(const Mat2_<T>& m)
	{
		*this = *this * m;
		return *this;
	}
	
	// Indexing operators
	const Vec2_<T>& operator[](int index) const
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	Vec2_<T>& operator[](int index)
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
			
		return _m[index];
	}
	
	const T& operator()(int i, int j) const
	{
		if (i < 0 || i > 1 || j < 0 || j > 1)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	T& operator()(int i, int j)
	{
		if (i < 0 || i > 1 || j < 0 || j > 1)
			Error("Index out of bounds");
			
		return _m[i][j];
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Mat2_<T>& m)
	{
		return os << std::endl << m[0] << std::endl << m[1] << std::endl;
	}
	 
	friend std::istream& operator>>(std::istream& is, Mat2_<T>& m)
	{
		return is >> m[0] >> m[1];
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
		return _m[0][0] * _m[1][1] - _m[1][0] * _m[0][1];
	}
	
	Mat2_<T> transpose() const
	{
		return Mat2_<T>(
			_m[0][0], _m[1][0],
			_m[0][1], _m[1][1]
		);
	}
	
	Mat2_<T> inverse() const
	{
		T	a = _m[0][0],
			b = _m[0][1],
			c = _m[1][0],
			d = _m[1][1];
		
		return Mat2_<T>(
			d/(a*d - b*c), -b/(a*d - b*c),
			-c/(a*d - b*c),  a/(a*d - b*c)
		);
	}
	
};

typedef Mat2_<GLfloat> Mat2;
typedef Mat2_<GLfloat> Mat2f;
typedef Mat2_<GLdouble> Mat2d;
typedef Mat2_<GLint> Mat2i;

} // namespace GLJoe

#endif // DEF_GLJOE_MAT2_H

