/**
 * @file GLJoe/core/vec2.h
 * 
 * @brief 2D vectors
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_VEC2_H
#define DEF_GLJOE_VEC2_H

namespace GLJoe
{
	
template <class T>
struct Vec2_
{
public:
	T x;
	T y;
	
	/**
	* Constructors
	*/
	
	Vec2_() : x(T()), y(T())
	{}
	
	Vec2_(T a) : x(a), y(a)
	{}
	
	Vec2_(T vx, T vy) : x(vx), y(vy)
	{}
	
	Vec2_(const Vec2_<T>& v) : x(v.x), y(v.y)
	{}
	
	/**
	* Operators
	*/
	
	Vec2_<T>& operator=(const Vec2_<T>& v)
	{
		x = v.x;
		y = v.y;
		
		return *this;
	}
	
	Vec2_<T> operator-() const
	{
		return Vec2_<T>(-x, -y);
	}
	
	// Pointwise operations
	Vec2_<T> operator+(const Vec2_<T>& v) const
	{
		return Vec2_<T>(x + v.x, y + v.y);
	}
	
	Vec2_<T> operator-(const Vec2_<T>& v) const
	{
		return Vec2_<T>(x - v.x, y - v.y);
	}
	
	Vec2_<T> operator*(const Vec2_<T>& v) const
	{
		return Vec2_<T>(x * v.x, y * v.y);
	}
		
	Vec2_<T> operator/(const Vec2_<T>& v) const
	{
		return Vec2_<T>(x / v.x, y / v.y);
	}
	
	// Scalar operations
	Vec2_<T> operator+(const T& a) const
	{
		return Vec2_<T>(x + a, y + a);
	}
	
	Vec2_<T> operator-(const T& a) const
	{
		return Vec2_<T>(x - a, y - a);
	}
	
	Vec2_<T> operator*(const T& a) const
	{
		return Vec2_<T>(x * a, y * a);
	}
	
	Vec2_<T> operator/(const T& a) const
	{
		return Vec2_<T>(x / a, y / a);
	}
	
	friend Vec2_<T> operator*(const T& a, const Vec2_<T>& v)
	{
		return v * a;
	}
	
	// Modifying operations
	Vec2_<T>& operator+=(const Vec2_<T>& v)
	{
		*this = *this + v;
		return *this;
	}	
	
	Vec2_<T>& operator-=(const Vec2_<T>& v)
	{
		*this = *this - v;
		return *this;
	}	
	
	Vec2_<T>& operator*=(const Vec2_<T>& v)
	{
		*this = *this * v;
		return *this;
	}	
	
	Vec2_<T>& operator/=(const Vec2_<T>& v)
	{
		*this = *this / v;
		return *this;
	}	
	
	Vec2_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}	
	
	Vec2_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}	
	
	Vec2_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}	
	
	Vec2_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	// Indexing operator
	const T& operator[](int index) const
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator[](int index)
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	const T& operator()(int index) const
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator()(int index)
	{
		if (index < 0 || index > 1)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Vec2_<T>& v)
	{
		return os << "(" << v.x << ", " << v.y << ")";
	}
	
	friend std::istream& operator>>(std::istream& is, Vec2_<T>& v)
	{
		return is >> v.x >> v.y;
	}
	
	// Conversion operators
	operator const T*() const
	{
		return static_cast<const T*>(&x);
	}
	
	operator T*()
	{
		return static_cast<T*>(&x);
	}
	
	/**
	 * Basic operations
	 */
	friend T dot(const Vec2_<T>& u, const Vec2_<T>& v)
	{
		return u.x * v.x + u.y * v.y;
	}
	
	T norm() const
	{
		return sqrt(dot(*this, *this));
	}
	
	Vec2_<T> normalize() const
	{
		return *this / norm();
	}
};

typedef Vec2_<GLfloat> Vec2;
typedef Vec2_<GLfloat> Vec2f;
typedef Vec2_<GLdouble> Vec2d;
typedef Vec2_<GLint> Vec2i;
	
} // namespace GLJoe


#endif // DEF_GLJOE_VEC2_H

