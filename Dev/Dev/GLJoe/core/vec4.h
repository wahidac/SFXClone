/**
 * @file GLJoe/core/vec4.h
 * 
 * @brief 4D vectors
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_VEC4_H
#define DEF_GLJOE_VEC4_H

namespace GLJoe
{
	
template <class T>
struct Vec4_
{
public:
	T x;
	T y;
	T z;
	T w;
	
	/**
	 * Constructors
	 */
	
	Vec4_() : x(T()), y(T()), z(T()), w(T())
	{}
	
	Vec4_(T a) : x(a), y(a), z(a), w(a)
	{}
	
	Vec4_(T vx, T vy, T vz, T vw) : x(vx), y(vy), z(vz), w(vw)
	{}
	
	Vec4_(Vec3_<T> v, T vw = T()) : x(v.x), y(v.y), z(v.z), w(vw)
	{}
	
	Vec4_(const Vec4_<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w)
	{}
	
	/**
	 * Operators
	 */
	
	Vec4_<T>& operator=(const Vec4_<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		
		return *this;
	}
	
	Vec4_<T> operator-() const
	{
		return Vec4_<T>(-x, -y, -z, -w);
	}
	
	// Pointwise operations
	Vec4_<T> operator+(const Vec4_<T>& v) const
	{
		return Vec4_<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	
	Vec4_<T> operator-(const Vec4_<T>& v) const
	{
		return Vec4_<T>(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	
	Vec4_<T> operator*(const Vec4_<T>& v) const
	{
		return Vec4_<T>(x * v.x, y * v.y, z * v.z, w * v.w);
	}
		
	Vec4_<T> operator/(const Vec4_<T>& v) const
	{
		return Vec4_<T>(x / v.x, y / v.y, z / v.z, w / v.w);
	}
	
	// Scalar operations
	Vec4_<T> operator+(const T& a) const
	{
		return Vec4_<T>(x + a, y + a, z + a, w + a);
	}
	
	Vec4_<T> operator-(const T& a) const
	{
		return Vec4_<T>(x - a, y - a, z - a, w - a);
	}
	
	Vec4_<T> operator*(const T& a) const
	{
		return Vec4_<T>(x * a, y * a, z * a, w * a);
	}
	
	Vec4_<T> operator/(const T& a) const
	{
		return Vec4_<T>(x / a, y / a, z / a, z / a);
	}
	
	friend Vec4_<T> operator*(const T& a, const Vec4_<T>& v)
	{
		return v * a;
	}
	
	// Modifying operations
	Vec4_<T>& operator+=(const Vec4_<T>& v)
	{
		*this = *this + v;
		return *this;
	}	
	
	Vec4_<T>& operator-=(const Vec4_<T>& v)
	{
		*this = *this - v;
		return *this;
	}	
	
	Vec4_<T>& operator*=(const Vec4_<T>& v)
	{
		*this = *this * v;
		return *this;
	}	
	
	Vec4_<T>& operator/=(const Vec4_<T>& v)
	{
		*this = *this / v;
		return *this;
	}	
	
	Vec4_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}	
	
	Vec4_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}	
	
	Vec4_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}	
	
	Vec4_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	// Indexing operator
	const T& operator[](int index) const
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator[](int index)
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	const T& operator()(int index) const
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator()(int index)
	{
		if (index < 0 || index > 3)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Vec4_<T>& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w 
			<< ")";
	}
	
	friend std::istream& operator>>(std::istream& is, Vec4_<T>& v)
	{
		return is >> v.x >> v.y >> v.z >> v.w;
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
	 * Subvectors
	 */
	 
	Vec2_<T> xy() const
	{
		return Vec2_<T>(x, y);
	}
	 
	Vec2_<T> xz() const
	{
		return Vec2_<T>(x, z);
	}
	 
	Vec2_<T> xw() const
	{
		return Vec2_<T>(x, w);
	}
	
	Vec2_<T> yz() const
	{
		return Vec2_<T>(y, z);
	}
	
	Vec2_<T> yw() const
	{
		return Vec2_<T>(y, w);
	}
	
	Vec2_<T> zw() const
	{
		return Vec2_<T>(z, w);
	}
	
	Vec3_<T> xyz() const
	{
		return Vec3_<T>(x, y, z);
	}
	
	Vec3_<T> xyw() const
	{
		return Vec3_<T>(x, y, w);
	}
	
	Vec3_<T> xzw() const
	{
		return Vec3_<T>(x, z, w);
	}
	
	Vec3_<T> yzw() const
	{
		return Vec3_<T>(y, z, w);
	}
	
	/**
	 * Basic operations
	 */
	friend T dot(const Vec4_<T>& u, const Vec4_<T>& v)
	{
		return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
	}
	
	T norm() const
	{
		return sqrt(dot(*this, *this));
	}
	
	Vec4_<T> normalize() const
	{
		return *this / norm();
	}
	
	friend Vec3_<T> cross(const Vec4_<T>& u, const Vec4_<T>& v)
	{
		return Vec3_<T>(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x
		);
	}
};

typedef Vec4_<GLfloat> Vec4;
typedef Vec4_<GLfloat> Vec4f;
typedef Vec4_<GLdouble> Vec4d;
typedef Vec4_<GLint> Vec4i;
	
} // namespace GLJoe


#endif // DEF_GLJOE_VEC4_H

