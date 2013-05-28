/**
 * @file GLJoe/geometry/transform2d.h
 *
 * @brief 2D transform matrices
 * 
 * @author Joachim Valente <joachim.valente@gmail.com
 * @date 6/13/2013
 */

#ifndef DEF_GLJOE_TRANSFORM2D_H
#define DEF_GLJOE_TRANSFORM2D_H

namespace GLJoe
{

/**
 * Non-class versions
 */
 
inline Mat3 Identity2()
{
	return Mat3(
		1, 0, 0,	
		0, 1, 0,
		0, 0, 1
	);
}

inline Mat3 Rotate(const GLfloat theta)
{
	GLfloat angle = Radians(theta);
	Mat3 r = Identity2();
	r[1][1] = r[0][0] = cos(angle);
	r[1][0] = sin(angle);
	r[0][1] = -r[1][0];
	
	return r;
}

inline Mat3 Translate(const GLfloat x, const GLfloat y)
{
	Mat3 t = Identity2();
	t[0][2] = x;
	t[1][2] = y;
	
	return t;
}

inline Mat3 Translate(const Vec2& v)
{
	return Translate(v.x, v.y);
}

inline Mat3 Scale(const GLfloat x, const GLfloat y)
{
	return Mat3(
		x, 0, 0,
		0, y, 0,
		0, 0, 1
	);
}

inline Mat3 Scale(const Vec2& v)
{
	return Scale(v.x, v.y);
}


/**
 * Class Transform
 */

class Transform2D
{
protected:
	Mat3 mat;
	
public:
	Transform2D()
	{
		setIdentity();
	}
	
	Transform2D& operator=(const Transform2D& t2d)
	{
		mat = t2d.mat;
		
		return *this;
	}
	
	Transform2D& operator=(const Mat3& m)
	{
		mat = m;
		
		return *this;
	}
	
	void setIdentity()
	{
		mat = Mat3(
			1, 0, 0,	
			0, 1, 0,
			0, 0, 1
		);
	}
	
	void rotate(const GLfloat theta)
	{
		GLfloat angle = Radians(theta);
		Mat3 r = Identity2();
		r[1][1] = r[0][0] = cos(angle);
		r[1][0] = sin(angle);
		r[0][1] = -r[1][0];
		
		mat *= r;
	}
	
	void translate(const GLfloat x, const GLfloat y)
	{
		mat[0][2] += x;
		mat[1][2] += y;
	}
	
	void translate(const Vec2& v)
	{
		translate(v.x, v.y);
	}
	
	void translate(const Vec3& v)
	{
		translate(v.x, v.y);
	}
	
	void scale(const GLfloat x, const GLfloat y)
	{
		mat *= Mat3(
			x, 0, 0,
			0, y, 0,
			0, 0, 1
		);
	}
	
	void scale(const GLfloat s)
	{
		scale(s, s);
	}
	
	void scale(const Vec2& v)
	{
		scale(v.x, v.y);
	}
	
	const Mat3& t() const
	{
		return mat;
	}
	
	operator const GLfloat*() const
	{
		return (const GLfloat*) mat;
	}
	
	operator GLfloat*()
	{
		return (GLfloat*) mat;
	}
	
};

typedef Stack_<Transform2D> MatrixStack2D;

} // namespace GLJoe

#endif // DEF_GLJOE_TRANSFORM2D_H

