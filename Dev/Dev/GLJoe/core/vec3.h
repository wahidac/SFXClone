/**
 * @file GLJoe/core/vec3.h
 * 
 * @brief 3D vectors
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_VEC3_H
#define DEF_GLJOE_VEC3_H

namespace GLJoe
{
	
template <class T>
struct Vec3_
{
public:
	T x;
	T y;
	T z;
	
	/**
	 * Constructors
	 */
	
	Vec3_() : x(T()), y(T()), z(T())
	{}
	
	Vec3_(T a) : x(a), y(a), z(a)
	{}
	
	Vec3_(T vx, T vy, T vz) : x(vx), y(vy), z(vz)
	{}
	
	Vec3_(Vec2_<T> v, T vz = T()) : x(v.x), y(v.y), z(vz)
	{}
	
	Vec3_(const Vec3_<T>& v) : x(v.x), y(v.y), z(v.z)
	{}
	
	/**
	 * Operators
	 */
	
	Vec3_<T>& operator=(const Vec3_<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		
		return *this;
	}
	
	Vec3_<T> operator-() const
	{
		return Vec3_<T>(-x, -y, -z);
	}
	
	// Pointwise operations
	Vec3_<T> operator+(const Vec3_<T>& v) const
	{
		return Vec3_<T>(x + v.x, y + v.y, z + v.z);
	}
	
	Vec3_<T> operator-(const Vec3_<T>& v) const
	{
		return Vec3_<T>(x - v.x, y - v.y, z - v.z);
	}
	
	Vec3_<T> operator*(const Vec3_<T>& v) const
	{
		return Vec3_<T>(x * v.x, y * v.y, z * v.z);
	}
		
	Vec3_<T> operator/(const Vec3_<T>& v) const
	{
		return Vec3_<T>(x / v.x, y / v.y, z / v.z);
	}
	
	// Scalar operations
	Vec3_<T> operator+(const T& a) const
	{
		return Vec3_<T>(x + a, y + a, z + a);
	}
	
	Vec3_<T> operator-(const T& a) const
	{
		return Vec3_<T>(x - a, y - a, z - a);
	}
	
	Vec3_<T> operator*(const T& a) const
	{
		return Vec3_<T>(x * a, y * a, z * a);
	}
	
	Vec3_<T> operator/(const T& a) const
	{
		return Vec3_<T>(x / a, y / a, z / a);
	}
	
	friend Vec3_<T> operator*(const T& a, const Vec3_<T>& v)
	{
		return v * a;
	}
	
	// Modifying operations
	Vec3_<T>& operator+=(const Vec3_<T>& v)
	{
		*this = *this + v;
		return *this;
	}	
	
	Vec3_<T>& operator-=(const Vec3_<T>& v)
	{
		*this = *this - v;
		return *this;
	}	
	
	Vec3_<T>& operator*=(const Vec3_<T>& v)
	{
		*this = *this * v;
		return *this;
	}	
	
	Vec3_<T>& operator/=(const Vec3_<T>& v)
	{
		*this = *this / v;
		return *this;
	}	
	
	Vec3_<T>& operator+=(const T& a)
	{
		*this = *this + a;
		return *this;
	}	
	
	Vec3_<T>& operator-=(const T& a)
	{
		*this = *this - a;
		return *this;
	}	
	
	Vec3_<T>& operator*=(const T& a)
	{
		*this = *this * a;
		return *this;
	}	
	
	Vec3_<T>& operator/=(const T& a)
	{
		*this = *this / a;
		return *this;
	}
	
	// Indexing operator
	const T& operator[](int index) const
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator[](int index)
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	const T& operator()(int index) const
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	T& operator()(int index)
	{
		if (index < 0 || index > 2)
			Error("Index out of bounds");
		
		return *(&x + index);
	}
	
	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const Vec3_<T>& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
	
	friend std::istream& operator>>(std::istream& is, Vec3_<T>& v)
	{
		return is >> v.x >> v.y >> v.z;
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
	
	Vec2_<T> yz() const
	{
		return Vec2_<T>(y, z);
	}
	
	/**
	 * Basic operations
	 */
	friend T dot(const Vec3_<T>& u, const Vec3_<T>& v)
	{
		return u.x * v.x + u.y * v.y + u.z * v.z;
	}
	
	T norm() const
	{
		return sqrt(dot(*this, *this));
	}
	
	Vec3_<T> normalize() const
	{
		return *this / norm();
	}
	
	friend Vec3_<T> cross(const Vec3_<T>& u, const Vec3_<T>& v)
	{
		return Vec3_<T>(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x
		);
	}
};

typedef Vec3_<GLfloat> Vec3;
typedef Vec3_<GLfloat> Vec3f;
typedef Vec3_<GLdouble> Vec3d;
typedef Vec3_<GLint> Vec3i;
	
} // namespace GLJoe


#endif // DEF_GLJOE_VEC3_H

